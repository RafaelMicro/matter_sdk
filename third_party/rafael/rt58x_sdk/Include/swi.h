/** @file swi.h
 *
 * @brief Software interrupt driver header file.
 *
 */
/**
 * @defgroup swi_group SWI
 * @ingroup peripheral_group
 * @{
 * @brief Define SWI definitions, structures, and functions.
 */

#ifndef __SWI_H__
#define __SWI_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

typedef enum
{
    SWI_ID_0          = 0,    /**< ID 0. */
    SWI_ID_1          = 1,    /**< ID 1. */
    SWI_ID_2          = 2,    /**< ID 2. */
    SWI_ID_3          = 3,    /**< ID 3. */
    SWI_ID_4          = 4,    /**< ID 4. */
    SWI_ID_5          = 5,    /**< ID 5. */
    SWI_ID_6          = 6,    /**< ID 6. */
    SWI_ID_7          = 7,    /**< ID 7. */
    SWI_ID_8          = 8,    /**< ID 8. */
    SWI_ID_9          = 9,    /**< ID 9. */
    SWI_ID_10         = 10,   /**< ID 10. */
    SWI_ID_11         = 11,   /**< ID 11. */
    SWI_ID_12         = 12,   /**< ID 12. */
    SWI_ID_13         = 13,   /**< ID 13. */
    SWI_ID_14         = 14,   /**< ID 14. */
    SWI_ID_15         = 15,   /**< ID 15. */
    SWI_ID_16         = 16,   /**< ID 16. */
    SWI_ID_17         = 17,   /**< ID 17. */
    SWI_ID_18         = 18,   /**< ID 18. */
    SWI_ID_19         = 19,   /**< ID 19. */
    SWI_ID_20         = 20,   /**< ID 20. */
    SWI_ID_21         = 21,   /**< ID 21. */
    SWI_ID_22         = 22,   /**< ID 22. */
    SWI_ID_23         = 23,   /**< ID 23. */
    SWI_ID_24         = 24,   /**< ID 24. */
    SWI_ID_25         = 25,   /**< ID 25. */
    SWI_ID_26         = 26,   /**< ID 26. */
    SWI_ID_27         = 27,   /**< ID 27. */
    SWI_ID_28         = 28,   /**< ID 28. */
    SWI_ID_29         = 29,   /**< ID 29. */
    SWI_ID_30         = 30,   /**< ID 30. */
    SWI_ID_31         = 31,   /**< ID 31. */
    MAX_NUMBER_OF_SWI = 32,  /**< Max SWI Number 32. */
} swi_id_sel_t;

/**
 * @brief User cb handler prototype.
 *
 * This function is called when the requested number of samples has been processed.
 *
 * @param[in] swi_id Software Interrupt ID
 *              \arg SWI_ID_0 ~ SWI_ID_31
 */
typedef void (*swi_isr_handler_t)(uint32_t swi_id);

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#define SWI_INT_ENABLE(para_set)                 (REMAP->SW_IRQ_EN_SET = para_set)      /**< Enable the software interrupt*/
#define SWI_INT_ENABLE_GET()                     (REMAP->SW_IRQ_EN_SET)                 /**< Return the software interrupt enable status*/
#define SWI_INT_DISABLE(para_set)                (REMAP->SW_IRQ_EN_CLR = para_set)      /**< Disable the software interrupt*/
#define SWI_INT_CLEAR(para_set)                  (REMAP->SW_IRQ_CLR = para_set)         /**< Clear the software interrupt status*/
#define SWI_INT_STATUS_SET(para_set)             (REMAP->SW_IRQ_SET = para_set)         /**< Set the software interrupt status*/
#define SWI_INT_STATUS_GET()                     (REMAP->SW_IRQ_SET)                    /**< Return the software interrupt status*/

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief Clear all SWI interrupt service routine callback.
 * @return None
 */
void Swi_Int_Callback_Clear(void);

/**
 * @brief Enable the specified software interrupts
 * @param[in] swi_id Software Interrupt ID
 *              \arg SWI_ID_0 ~ SWI_ID_31
 * @param[in] swi_int_callback Software interrupt callback handler
 * @return None
 */
void Swi_Int_Enable(swi_id_sel_t swi_id, swi_isr_handler_t swi_int_callback);

/**
 * @brief Disable software interrupt(s)
 * @param[in] swi_id Software interrupt ID
 *              \arg SWI_ID_0 ~ SWI_ID_31
 * @return None
 */
void Swi_Int_Disable(swi_id_sel_t swi_id);

/**
 * @brief Trigger software interrupt(s)
 * @param[in] swi_id Software interrupt ID
 *            \arg SWI_ID_0 ~ SWI_ID_31
 * @return None
 */
void Swi_Int_Trigger(swi_id_sel_t swi_id);


#endif /* End of _SWI_H_ */

/** @} */ /* End of Peripheral Group */

