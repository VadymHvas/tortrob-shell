/*
 * Based on: https://github.com/VadymHvas/tortrob-malloc
 * Original version used libc, this version modified for no libc environment
 * 
 * CHANGES:
 * - Replaced mmap() with custom trb_mmap()
 * - Replaced size_t type with int  
 * - Discontinuation of stdbool.h (true and false are now 1 and 0)
 */
#include "lib/trbmem.h"
#include "lib/trbstdio.h"
#include "asm/trbsyscall.h"

#define HEAP_SIZE            4096

#define HEADER_SIZE          sizeof(struct blk_header_t)
#define ALIGNMENT            (sizeof(int) * 2)

#define GET_BLK_SIZE(blk)    (blk->size + HEADER_SIZE)
#define HEADER_TO_DATA(blk)  ((char *)blk + HEADER_SIZE)
#define DATA_TO_HEADER(ptr)  ((struct blk_header_t *)((char *)ptr - HEADER_SIZE))

/**
 *
 * Each allocated or free block of memory begins with this header.
 * It stores bookkeeping information that allows the allocator to
 * manage memory efficiently.
 * 
 * The data itself is stored at the address 
 * ((char *)blk_header + sizeof(struct blk_header_t)) or macro HEADER_TO_DATA, 
 * relative to the beginning of the block header
 * 
 * Example:
 * _________________________________________________
 * |     Header      |                             |
 * | size      n     |                             |
 * | free      false |             DATA            |
 * | next      addr  |                             |
 * | prev      addr  |                             |
 * |________________ |_____________________________|
 * 
 */
struct blk_header_t {
    int size;                 // Size of the data area (does not include the header itself).
    int free;                   // Indicates whether this block is free (true 1) or allocated (false 0).
    struct blk_header_t *next;   // Pointer to the next block in the memory list.
    struct blk_header_t *prev;   // Pointer to the previous block in the memory list.
};

static int heap_initialized = 0;  // Tracks whether the heap has been set up already.
static void *heap_start;               // Pointer to the beginning of the managed heap memory.
static struct blk_header_t *free_list; // Head of the free list (linked list of available blocks).

/**
 * 
 * @brief Initializes the heap allocator.
 * 
 * This function allocates memory using mmap for the heap,
 * and initializes the free list.
 */
static int heap_init()
{
    struct mmap_args mmap_args = {
        .addr   = (unsigned long)NULL,
        .len    = HEAP_SIZE,
        .prot   = PROT_READ | PROT_WRITE,
        .flags  = MAP_ANONYMOUS | MAP_PRIVATE,
        .fd     = -1,
        .offset = 0
    };

    heap_start = trb_mmap(&mmap_args);

    if (heap_start == (void *)-1) 
        return -1;

    free_list = (struct blk_header_t *)heap_start; // The free list will be placed at the beginning of the heap.
    free_list->size = HEAP_SIZE - HEADER_SIZE;     // The size of the block in the header does not include the size of the header itself.
    free_list->next = NULL;
    free_list->prev = NULL;
    free_list->free = 1;

    heap_initialized = 1;

    return 0;
}

/**
 * 
 * @brief Adds header to free list.
 * 
 * Adds a header to the top of the free list
 */
static void add_to_free_list(struct blk_header_t *blk_header)
{
    blk_header->prev = NULL;
    blk_header->next = free_list;

    if (free_list != NULL)
        free_list->prev = blk_header;

    free_list = blk_header;
}

/**
 * 
 * @brief Removes header from free list.
 */
static void remove_from_free_list(struct blk_header_t *blk_header)
{
    struct blk_header_t *next = blk_header->next;
    struct blk_header_t *prev = blk_header->prev;

    if (free_list == blk_header)
        free_list = next;

    if (next != NULL)
        next->prev = prev;

    if (prev != NULL)
        prev->next = next;
}

/**
 * 
 * @brief Finds a free block in the free list.
 * 
 * Finds a free block, whose size is larger than aligned size.
 */
static struct blk_header_t *find_free_blk(int aligned_size)
{
    struct blk_header_t *current = free_list;

    while (current) {
        if (current->free == 1 && current->size >= aligned_size)
            return current;
        
        current = current->next;
    }

    return NULL;
}

/**
 * @brief Splits a memory block into two parts if it is larger than needed.
 *
 * This function takes a block header and an aligned size, then checks
 * if the block is large enough to be split into two blocks: one of the
 * requested size and another for the remaining space. If so, it updates
 * the current block size, creates a new block header for the leftover
 * memory, marks it as free, and adds it to the free list.
 */
static struct blk_header_t *split_blk(struct blk_header_t *blk_header, int aligned_size)
{
    if (GET_BLK_SIZE(blk_header) < aligned_size + HEADER_SIZE + ALIGNMENT)
        return NULL;

    int split_size = blk_header->size - aligned_size;

    blk_header->size = aligned_size;

    struct blk_header_t *split_header = (struct blk_header_t *)(HEADER_TO_DATA(blk_header) + blk_header->size);

    split_header->size = split_size;
    split_header->free = 1;

    add_to_free_list(split_header);

    return split_header;
}

/**
 * @brief Coalesces (merges) adjacent free memory blocks.
 *
 * This function checks whether the given block has free neighbors
 * (previous or next). If so, it merges them into a single larger block
 * by adjusting sizes and pointers in the free list. This helps reduce
 * fragmentation and improves allocation efficiency.
 */
static struct blk_header_t *coalesce(struct blk_header_t *blk_header)
{
    // Check if the previous block exists and is free
    if (blk_header->prev && blk_header->prev->free == 1) {
        struct blk_header_t *prev = blk_header->prev;

        // Merge current block into the previous block
        prev->size += blk_header->size;
        prev->next = blk_header->next;

        // Fix backward link if a next block exists
        if (blk_header->next) {
            blk_header->next->prev = prev;
        }

        // Update blk_header to point to the newly merged block
        blk_header = prev;
    }

    // Check if the next block exists and is free
    if (blk_header->next && blk_header->next->free == 1) {
        struct blk_header_t *next = blk_header->next;

        // Merge next block into the current block
        blk_header->size += next->size;
        blk_header->next = next->next;

        // Fix backward link if a following block exists
        if (next->next) {
            next->next->prev = blk_header;
        }
    }

    return blk_header;
}

/**
 * @brief Allocates a block of memory from the custom heap.
 *
 * This function behaves similarly to malloc(), but uses a custom
 * allocator with block headers and a free list. It ensures alignment,
 * finds a suitable free block, splits it if needed, removes it from
 * the free list, and returns a pointer to the usable memory region.
 */
void *trb_malloc(int size)
{
    if (size == 0)
        return NULL;

    if (heap_initialized == 0 && heap_init() == -1)
        return NULL;

    int aligned_size = (size + ALIGNMENT - 1) & -ALIGNMENT;
    struct blk_header_t *blk_header = find_free_blk(aligned_size);

    if (blk_header == NULL)
        return NULL;

    if (split_blk(blk_header, aligned_size) == NULL)
        return NULL;

    remove_from_free_list(blk_header);

    blk_header->free = 0;
    return HEADER_TO_DATA(blk_header);
}

/**
 * 
 * @brief Frees a previously allocated block of memory.
 *
 * This function behaves similarly to free(), but works with the
 * custom allocator. It marks the block as free, reinserts it
 * into the free list, and attempts to coalesce adjacent free
 * blocks to reduce fragmentation.
 */
void trb_free(void *ptr)
{
    if (ptr == NULL)
        return;

    struct blk_header_t *blk_header = DATA_TO_HEADER(ptr);

    blk_header->free = 1;
    add_to_free_list(blk_header);

    coalesce(blk_header);
}