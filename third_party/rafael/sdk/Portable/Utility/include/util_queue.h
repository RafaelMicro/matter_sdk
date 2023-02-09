/**
 * @file util_queue.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup Queue Queue API Definition
* Define Queue definitions, structures, and functions.
* @ingroup Utility
* @{
*/
#ifndef __UTIL_QUEUE_H__
#define __UTIL_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PARENT_BY_FIELD_GET(STRUCT_TYPE, FIELD_NAME, FIELD_POINTER) \
    ((STRUCT_TYPE *) (((uint8_t *)FIELD_POINTER) - offsetof(STRUCT_TYPE, FIELD_NAME)))

typedef struct queue_elem
{
    void *p_data;               /**< Pointer to data representing the queue element. Never changed by the queue module. */
    struct queue_elem *p_next;  /**< Pointer to the next element in the queue. Set and used by the queue module. */
} queue_elem_t;

/**
 * @brief Queue instance. All manipulation of struct internals should be left to the queue module.
 *
 */
typedef struct
{
    queue_elem_t *p_front; /**< Pointer to the front of the queue, where the queue elements are popped. */
    queue_elem_t *p_back;  /**< Pointer to the back of the queue, where the queue elements are pushed. */
} queue_t;

/**
 * Initializes a queue instance.
 *
 * @param[in,out] p_queue Pointer to an uninitialized queue instance.
 */
void queue_init(queue_t *p_queue);

/**
 * Pushes a single queue element to the back of the given queue instance.
 *
 * @warning: The user has to set the @c p_data pointer to reference the desired
 * data of the element before pushing it to the queue. This function will
 * assert if @c p_data is NULL. The @c p_data member will never be changed by
 * the queue module.
 *
 * @param[in,out] p_queue The queue instance to push to.
 * @param[in] p_elem Pointer to a statically allocated queue element to push to the back of the queue.
 */
void queue_push(queue_t *p_queue, queue_elem_t *p_elem);

/**
 * Pops the element at the front of the queue, and removes it from the queue.
 *
 * @param[in] p_queue Queue instance to pop from.
 *
 * @returns The @c p_data pointer of the element at the front of the queue, or NULL if the queue is empty.
 */
queue_elem_t *queue_pop(queue_t *p_queue);

/**
 * Peeks at the element at the front of the queue, but does not remove it.
 *
 * @param[in] p_queue Queue instance to peek from.
 *
 * @returns The @c p_data pointer of the element at the front of the queue, or NULL if the queue is empty.
 */
queue_elem_t *queue_peek(const queue_t *p_queue);

/**
 * Pops all elements of @p p_src and adds them at the end of @p p_dst.
 *
 * After this operation, @p p_dst will contain all of its own elements (in their original order), then
 * all the elements of @p p_src (in their original order). @p p_src will be empty.
 *
 * @param[in,out] p_dst Destination queue for all the elements.
 * @param[in,out] p_src Source queue for all the elements.
 */
void queue_merge(queue_t *p_dst, queue_t *p_src);

/** Queue iterator structure */
typedef struct
{
    queue_t *p_queue;  /**< Queue this iterator is iterating over. */
    queue_elem_t **pp_elem;  /**< Double pointer to the current iterator element. */
    bool repeat; /**< Whether to repeat the same element again. */
} queue_elem_iterator_t;

/**
 * Iterator init value for the beginning of the queue.
 *
 * @param[in] p_QUEUE Queue to iterate over.
 */
#define QUEUE_ITERATOR_BEGIN(p_QUEUE)   \
    {                                   \
        .p_queue = (p_QUEUE),           \
        .pp_elem = &(p_QUEUE)->p_front, \
        .repeat = false                 \
    }
/**
 * Iterator init value for the end of the queue.
 *
 * @param[in] p_QUEUE Queue to iterate over.
 */
#define QUEUE_ITERATOR_END(p_QUEUE)   \
    {                                   \
        .p_queue = (p_QUEUE),           \
        .pp_elem = &(p_QUEUE)->p_back, \
        .repeat = false                 \
    }

/**
 * Iterates over a queue like for loop.
 *
 * @warning This function is not thread-safe. It should only be used on queues that operate in a
 * single IRQ level.
 *
 * @param[in] p_QUEUE Queue to iterate over.
 * @param[in] ITERATOR Iterator name.
 */
#define QUEUE_FOREACH(p_QUEUE, ITERATOR)                                 \
    for (queue_elem_iterator_t ITERATOR = QUEUE_ITERATOR_BEGIN(p_QUEUE); \
         (*((ITERATOR).pp_elem) != NULL);                                \
         queue_iterator_iterate(&ITERATOR))

/**
 * Removes the element the iterator is at.
 *
 * The iterator will be moved to the next element immediately, instead of being moved at the next
 * iteration call. This allows the user to remove items in the middle of an iteration without
 * skipping any elements:
 *
 * @code{.c}
 * QUEUE_FOREACH(p_queue, it)
 * {
 *     if (should_delete_elem(&it))
 *     {
 *         queue_iterator_elem_remove(&it);
 *     }
 * }
 * @endcode
 *
 * In this code snippet, the @p should_delete_elem call will be executed on every queue element,
 * even though the @c queue_iterator_elem_remove call removes elements.
 *
 * Example of removing node B (with the iterator location):
 * @code
 * Before:
 *  A -> B -> C -> D
 *       ^it
 * After:
 *  A -> C -> D
 *       ^it
 * After next iterate:
 *  A -> C -> D
 *       ^it
 * @endcode
 *
 * @warning This function is not thread-safe. It should only be used on queues that operate in a
 * single IRQ level.
 *
 * @param[in,out] p_it Iterator to remove element at.
 */
void queue_iterator_elem_remove(queue_elem_iterator_t *p_it);

/**
 * Inserts an element at the current iterator location.
 *
 * Inserts the new element at the position of the iterator, before the element the iterator is
 * currently pointing at. The iterator will stay at the new entry, and will move to the same entry
 * as it was on prior to the insertion on the next iteration.
 *
 * Example of inserting node D (with the iterator location):
 * @code
 * Before:
 *  A -> B -> C
 *       ^it
 * After:
 *  A -> D -> B -> C
 *       ^it
 * After next iterate:
 *  A -> D -> B -> C
 *            ^it
 * @endcode
 *
 * @warning This function is not thread-safe. It should only be used on queues that operate in a
 * single IRQ level.
 *
 * @param[in,out] p_it Iterator to insert an element at.
 * @param[in,out] p_elem Element to insert.
 */
void queue_iterator_elem_insert(queue_elem_iterator_t *p_it, queue_elem_t *p_elem);

/**
 * Iterates to the next element.
 *
 * @warning This function is not thread-safe. It should only be used on queues that operate in a
 * single IRQ level.
 *
 * @param[in,out] p_it Iterator to iterate.
 */
void queue_iterator_iterate(queue_elem_iterator_t *p_it);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __UTIL_QUEUE_H__ */
