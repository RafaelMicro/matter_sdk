/** @file util_list.c
 *
 * @license
 * @description
 */

//=============================================================================
//                Include
//=============================================================================
#include "util_list.h"

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
void list_init(link_list_t *entry)
{
    entry->next = entry;
    entry->prev = entry;
}

// if the list is head, this function equal list_attach_head
void list_attach_next(link_list_t *list, link_list_t *entry)
{
    entry->prev = list;
    entry->next = list->next;
    list->next->prev = entry;
    list->next = entry;
}

// if the list is tail, this function equal list_attach_tail
void list_attach_prev(link_list_t *list, link_list_t *entry)
{
    entry->next = list;
    entry->prev = list->prev;
    list->prev->next = entry;
    list->prev = entry;
}

void list_detach(link_list_t *entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = entry->next = entry;
}

// if the list is header, this function equal list_detach_head
link_list_t *list_detach_next(link_list_t *list)
{
    link_list_t *entry = list->next;

    if (entry == list)
    {
        return NULL;
    }

    list_detach(entry);

    return entry;
}

// if the list is header, this function equal list_detach_tail
link_list_t *list_detach_prev(link_list_t *list)
{
    link_list_t *entry = list->prev;

    if (entry == list)
    {
        return NULL;
    }

    list_detach(entry);

    return entry;
}
