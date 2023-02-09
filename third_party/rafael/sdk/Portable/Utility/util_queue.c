/**
 * Copyright (c) 2021  All Rights Reserved.
 */
/** @file util_queue.c
 *
 * @author Rex
 * @version 0.1
 * @date 2021/07/08
 * @license
 * @description
 */

//=============================================================================
//                Include
//=============================================================================
#include "util_queue.h"
//=============================================================================
//                Private Definitions of const value
//=============================================================================

//=============================================================================
//                Private ENUM
//=============================================================================

//=============================================================================
//                Private Struct
//=============================================================================

//=============================================================================
//                Private Function Declaration
//=============================================================================

//=============================================================================
//                Private Global Variables
//=============================================================================

//=============================================================================
//                Public Global Variables
//=============================================================================

//=============================================================================
//                Private Definition of Compare/Operation/Inline funciton/
//=============================================================================

//=============================================================================
//                Functions
//=============================================================================
void queue_init(queue_t *p_queue)
{
    p_queue->p_front = NULL;
    p_queue->p_back  = NULL;
}

void queue_push(queue_t *p_queue, queue_elem_t *p_elem)
{

    p_elem->p_next = NULL;

    if (p_queue->p_front == NULL)
    {
        p_queue->p_front = p_elem;
    }
    else
    {
        p_queue->p_back->p_next = p_elem;
    }
    p_queue->p_back = p_elem;
}

queue_elem_t *queue_pop(queue_t *p_queue)
{
    queue_elem_t *p_elem = p_queue->p_front;

    if (p_elem != NULL)
    {
        p_queue->p_front = p_elem->p_next;
    }

    if (p_queue->p_front == NULL)
    {
        p_queue->p_back = NULL;
    }

    return p_elem;
}

queue_elem_t *queue_peek(const queue_t *p_queue)
{
    return p_queue->p_front;
}

void queue_merge(queue_t *p_dst, queue_t *p_src)
{
    if (queue_peek(p_src) == NULL)
    {
        // p_src is empty, do nothing.
    }
    else if (queue_peek(p_dst) == NULL)
    {
        // p_dst is empty
        *p_dst = *p_src;
        queue_init(p_src);
    }
    else
    {
        // both queues have elements
        p_dst->p_back->p_next = p_src->p_front;
        p_dst->p_back = p_src->p_back;
        queue_init(p_src);
    }
}

void queue_iterator_elem_remove(queue_elem_iterator_t *p_it)
{
    /* If we're explicitly removing p_queue->p_back, we need to translate that iterator to a normal
     * p_front iterator and remove that, as p_back doesn't provide any information about the
     * second-to-last element, making it impossible to cleanly terminate the back of the queue: */
    if (p_it->pp_elem == &p_it->p_queue->p_back)
    {
        QUEUE_FOREACH(p_it->p_queue, it)
        {
            if (*it.pp_elem == *p_it->pp_elem)
            {
                /* The local iterator now points to the same element as the requested element, but it
                 * also contains information about the second-to-last entry, which is required to
                 * cleanly remove the element. Use this iterator instead. */
                queue_iterator_elem_remove(&it);
                return;
            }
        }
    }

    if (*p_it->pp_elem == p_it->p_queue->p_back)
    {
        if (p_it->pp_elem == &p_it->p_queue->p_front)
        {
            p_it->p_queue->p_back = NULL;
        }
        else
        {
            p_it->p_queue->p_back = PARENT_BY_FIELD_GET(queue_elem_t, p_next, p_it->pp_elem);
        }
    }
    *p_it->pp_elem = (*p_it->pp_elem)->p_next;
    p_it->repeat = true;
}

void queue_iterator_elem_insert(queue_elem_iterator_t *p_it, queue_elem_t *p_elem)
{
    p_elem->p_next = *p_it->pp_elem;
    *p_it->pp_elem = p_elem;
}

void queue_iterator_iterate(queue_elem_iterator_t *p_it)
{
    if (p_it->repeat)
    {
        p_it->repeat = false;
    }
    else
    {
        p_it->pp_elem = &(*p_it->pp_elem)->p_next;
    }
}
