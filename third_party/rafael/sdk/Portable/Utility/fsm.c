

#include "fsm.h"
#include "util_log.h"
#include <stddef.h>


/**@brief Invalid transition index value.
 */
#define FSM_INVALID_INDEX   0xFF

static void fsm_any_state_find(fsm_t *p_fsm);
static bool fsm_event_post_try(fsm_t *p_fsm,
                               fsm_event_id_t event_id,
                               void *p_data,
                               uint8_t start_table_idx);
static bool fsm_transition_perform_try(fsm_t *p_fsm,
                                       const fsm_transition_t *p_transition,
                                       void *p_data);


static void fsm_any_state_find(fsm_t *p_fsm)
{
    const fsm_const_descriptor_t *p_fsm_const = p_fsm->fsm_const_desc;
    const fsm_transition_t        *p_transition_table = p_fsm_const->transition_table;
    uint8_t i;
    uint8_t found_idx = FSM_INVALID_INDEX;

    for (i = 0; i < p_fsm_const->transitions_count; i++)
    {
        if (found_idx != FSM_INVALID_INDEX)
        {
            if (p_transition_table[i].event_id == FSM_ANY_STATE ||
                    p_transition_table[i].event_id & FSM_STATE_FLAG)
            {
                // Another one "any state declaration block" found or
                // another one state declaration block found after "any state declaration block".
                while (1);
            }
        }

        if (p_transition_table[i].event_id == FSM_ANY_STATE)
        {
            found_idx = i;
            // But continue lookup to verify this is the only any state declaration,
            // and there are no more state declarations after this state declaration.
        }
    }

    if (found_idx != FSM_INVALID_INDEX)
    {
        if (found_idx >= p_fsm_const->transitions_count)
        {
            // Empty state declaration block (without transitions).
            // This is allowed for convenience.
            found_idx = FSM_INVALID_INDEX;
        }
    }
    p_fsm->any_state_transitions_index = found_idx;
}


static bool fsm_event_post_try(fsm_t *p_fsm,
                               fsm_event_id_t event_id,
                               void *p_data,
                               uint8_t start_table_idx)
{
    const fsm_const_descriptor_t *p_fsm_const;
    const fsm_transition_t *p_transition_table;
    const fsm_transition_t *p_transition_table_end;
    const fsm_transition_t *p_transition;
    fsm_state_id_t           current_state;

    current_state = p_fsm->current_state;
    p_fsm_const = p_fsm->fsm_const_desc;
    p_transition_table = p_fsm_const->transition_table;
    p_transition_table_end = p_transition_table + p_fsm_const->transitions_count;

    if (start_table_idx == 0 && p_fsm->any_state_transitions_index != 0)
    {
        // look for the beginning of the current state's transitions
        for (p_transition = p_transition_table; p_transition < p_transition_table_end; p_transition++)
        {
            if ((p_transition->event_id ^ FSM_STATE_FLAG) == current_state)
            {
                break;
            }
        }
    }
    else
    {
        p_transition = p_transition_table + start_table_idx;
    }

    // continue lookup - look for the occurred event in the current state's transitions
    for (p_transition++; p_transition < p_transition_table_end; p_transition++)
    {
        // check this transition is for the given event
        if (p_transition->event_id == event_id)
        {
            //printf("cs %d, e %d, a %d\n", current_state, event_id, p_transition->action_id);

            // try to perform the transition (will be performed, if guard check is true)
            if (fsm_transition_perform_try(p_fsm, p_transition, p_data))
            {
                // only the first satisfying transition is performed.
                return true;
            }
        } // check the beginning of the next state's transitions list
        else if (p_transition->event_id & FSM_STATE_FLAG)
        {
            return false;
        }
    }
    return false;
}


static bool fsm_transition_perform_try(fsm_t *p_fsm,
                                       const fsm_transition_t *p_transition,
                                       void *p_data)
{
    fsm_guard_t p_guard;
    fsm_action_t p_action;

    // perform the guard check, if guard condition is specified
    if (p_transition->guard_id != FSM_NO_GUARD)
    {
        p_guard = p_fsm->fsm_const_desc->guard;

        // check transition guard condition
        if (!(*p_guard)(p_transition->guard_id, p_data))
        {
#if FSM_DEBUG
            util_log(UTIL_LOG_INFO, "%s: G: %s\n", p_fsm->fsm_const_desc->fsm_name,
                     p_fsm->fsm_const_desc->guard_lookup[p_transition->guard_id]);
#endif
            // transition hasn't been done
            return false;
        }
    }

    // perform the action, if action is specified
    if (p_transition->action_id != FSM_NO_ACTION)
    {
        p_action = p_fsm->fsm_const_desc->action;

#if FSM_DEBUG
        util_log(UTIL_LOG_INFO, "%s: A: %s\n", p_fsm->fsm_const_desc->fsm_name,
                 p_fsm->fsm_const_desc->action_lookup[p_transition->action_id]);
#endif
        // perform transition action before changing current_state to new_state_id
        (*p_action)(p_transition->action_id, p_data);
    }

    // change state, if the new state is specified
    if (p_transition->new_state_id != FSM_SAME_STATE)
    {
        /* If any event causes a change in state, we shouldn't be in a recursive state. */

#if FSM_DEBUG
        util_log(UTIL_LOG_INFO, "%s: state %s -> %s\n", p_fsm->fsm_const_desc->fsm_name,
                 p_fsm->fsm_const_desc->state_lookup[p_fsm->current_state],
                 p_fsm->fsm_const_desc->state_lookup[p_transition->new_state_id]);
#endif

        // switch to the new state
        p_fsm->current_state = p_transition->new_state_id;
    }

    // transition has been done
    return true;
}

void fsm_init(fsm_t *p_fsm, const fsm_const_descriptor_t *p_fsm_const)
{

    p_fsm->fsm_const_desc       = p_fsm_const;
    p_fsm->current_state        = p_fsm_const->initial_state;
    p_fsm->recursion_protection = 0;

#if FSM_DEBUG


    util_log(UTIL_LOG_INFO, "%s: init\n", p_fsm->fsm_const_desc->fsm_name);
#endif

    fsm_any_state_find(p_fsm);
}

void fsm_event_post(fsm_t *p_fsm, fsm_event_id_t event_id, void *p_data)
{
#if FSM_DEBUG
    util_log(UTIL_LOG_INFO, "%s: E: %s\n", p_fsm->fsm_const_desc->fsm_name,
             p_fsm->fsm_const_desc->event_lookup[event_id]);
#endif

    p_fsm->recursion_protection++;
    //printf("fp, c %d, e %d\n", p_fsm->current_state, event_id);
    if (!fsm_event_post_try(p_fsm, event_id, p_data, 0))
    {
        if (p_fsm->any_state_transitions_index != FSM_INVALID_INDEX)
        {
            (void) fsm_event_post_try(p_fsm, event_id, p_data, p_fsm->any_state_transitions_index);
        }
    }

    p_fsm->recursion_protection--;
}

bool fsm_is_processing(const fsm_t *p_fsm)
{
    return p_fsm->recursion_protection > 0;
}
