/**
 * @file util_list.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __UTIL_LIST_H__
#define __UTIL_LIST_H__
//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stddef.h>

//=============================================================================
//                Public Definitions of const value
//=============================================================================
#define list_attach_head(list, entry)   list_attach_next(list, entry)
#define list_attach_tail(list, entry)   list_attach_prev(list, entry)
#define list_detach_head(list)          list_detach_next(list)
#define list_detach_tail(list)          list_detach_prev(list)

#ifndef offsetof
#define offsetof(type, member) ((long)&((type *)0)->member)
#endif

#define list_entry(item, type, member) \
    ((type *)((char *)item - offsetof(type, member)))

#define list_first(list, type, member) \
    (list_empty((list)) ? NULL : \
     list_entry((list)->next, type, member))

#define list_last(list, type, member) \
    (list_empty((list)) ? NULL : \
     list_entry((list)->prev, type, member))

#define list_for_each(item, list, type, member) \
    for (item = list_entry((list)->next, type, member); \
         &item->member != (list); \
         item = list_entry(item->member.next, type, member))

#define list_for_each_safe(item, n, list, type, member) \
    for (item = list_entry((list)->next, type, member), \
         n = list_entry(item->member.next, type, member); \
         &item->member != (list); \
         item = n, n = list_entry(n->member.next, type, member))

#define list_for_each_reverse(item, list, type, member) \
    for (item = list_entry((list)->prev, type, member); \
         &item->member != (list); \
         item = list_entry(item->member.prev, type, member))

#define DEFINE_DL_LIST(name) \
    link_list_t name = { &(name), &(name) }

//=============================================================================
//                Public ENUM
//=============================================================================

//=============================================================================
//                Public Struct
//=============================================================================
typedef struct linked_list
{
    struct linked_list *prev;
    struct linked_list *next;
} link_list_t;

//=============================================================================
//                Public Function Declaration
//=============================================================================
void list_init(link_list_t *entry);
void list_attach_next(link_list_t *list, link_list_t *entry);
void list_attach_prev(link_list_t *list, link_list_t *entry);
void list_detach(link_list_t *entry);
link_list_t *list_detach_next(link_list_t *list);
link_list_t *list_detach_prev(link_list_t *list);
static inline int list_empty(link_list_t *list)
{
    return list->next == list;
}

#endif /* __UTIL_LIST_H__ */
