/**
 * @file global_functions.c
 * @brief Contains function definitions for the global functions
 * 
 * @author Zach Fraser
 * @date 2024-08-06
 */

#include "definitions.h"

/**
 * @brief Insert bubble/no_bubble into bubble queue
 * @param bubble_queue Pointer to the bubble queue
 * @param value 1 for bubble, 0 for no_bubble
 */
void insert_bubble(bubble_queue_t *bubble_queue, int value)
{
    if(value)
    {
        bubble_queue->bubble_flag |= (1 << bubble_queue->size);
    }
    else
    {
        bubble_queue->bubble_flag &= ~(1 << bubble_queue->size);
    }
    bubble_queue->size++;
}

/**
 * @brief Remove bubble/no_bubble from bubble queue
 * @param bubble_queue Pointer to the bubble queue
 * @return int 1 for bubble, 0 for no_bubble
 */
int remove_bubble(bubble_queue_t *bubble_queue)
{
    int value = (bubble_queue->bubble_flag & 1);
    bubble_queue->bubble_flag >>= 1;
    bubble_queue->size--;
    return value;
}

/**
 * @brief Clear bubble queue
 * @param bubble_queue Pointer to the bubble queue
 */
void clear_bubble_queue(bubble_queue_t *bubble_queue)
{
    bubble_queue->bubble_flag = 0;
    bubble_queue->size = 0;
}