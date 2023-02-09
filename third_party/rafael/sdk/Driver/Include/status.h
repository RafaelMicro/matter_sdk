/**************************************************************************//**
 * @file     status.h
 * @version
 * @brief    define return status value
 *
 * @copyright
 ******************************************************************************/

#ifndef ___STATUS_H__
#define ___STATUS_H__


/* Define return Status */
#define STATUS_SUCCESS            (0UL)       ///
#define STATUS_INVALID_PARAM      (1UL)       /// Invalid Parameter.
#define STATUS_INVALID_REQUEST    (2UL)       /// Invalid Request. 
#define STATUS_EBUSY              (3UL)       /// Device is busy now. 
#define STATUS_NO_INIT            (4UL)       /// Device should be init first.
#define STATUS_ERROR              (5UL)       /// ERROR
#define STATUS_TIMEOUT            (6UL)       /// TIMEOUT


#endif
