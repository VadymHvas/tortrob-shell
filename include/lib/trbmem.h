#pragma once
/**
 * Adapted from: https://github.com/VadymHvas/tortrob-malloc
 * Modified for no libc environment.
 * 
 * @author Vadym Hvas
 */

/**
 * @brief Allocates a memory block of specified size.
 *
 * Allocates a contiguous memory block in the heap of 'bytes'  size.
 * Behavior is similar to standart malloc(), but implemented to work without libc
 * 
 * @param bytes Size of memory block to allocate in bytes
 * @return void* Pointer to allocated memory block
 * 
 * @retval NULL - if memory allocation failed
 * @retval !NULL - pointer to the start of block
 */
void *trb_malloc(int bytes);

/**
 * @brief Deallocates previosly allocated block.
 * 
 * Releases memory previously allocated by trb_malloc() back to the heap.
 * Behavior is similar to standart free(), but implemented to work without libc.
 * 
 * @param ptr Pointer to memory block to deallocate.
 */
void trb_free(void *ptr);