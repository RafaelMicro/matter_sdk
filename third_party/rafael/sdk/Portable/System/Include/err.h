#ifndef __ERR_H__
#define __ERR_H__

#include "stdint.h"

typedef int32_t err_t;

/* Definitions for error constants. */

#define ERR_OK          0    /* No error, everything OK. */
#define ERR_MEM        -1    /* Out of memory error.     */
#define ERR_BUF        -2    /* Buffer error.            */
#define ERR_TIMEOUT    -3    /* Timeout.                 */
#define ERR_RTE        -4    /* Routing problem.         */
#define ERR_INPROGRESS -5    /* Operation in progress    */
#define ERR_VAL        -6    /* Illegal value.           */
#define ERR_WOULDBLOCK -7    /* Operation would block.   */
#define ERR_USE        -8    /* Address in use.          */
#define ERR_ALREADY    -9    /* Already connecting.      */
#define ERR_ISCONN     -10   /* Conn already established.*/

#define ERR_CONN       -11   /* Not connected.           */
#define ERR_IS_FATAL(e) ((e) < ERR_CONN)

#define ERR_ABRT       -12   /* Connection aborted.      */
#define ERR_RST        -13   /* Connection reset.        */
#define ERR_CLSD       -14   /* Connection closed.       */

#define ERR_ARG        -15   /* Illegal argument.        */

#define ERR_IF         -16   /* Low-level netif error    */

#define lwip_strerr(x) ""

#endif
