

#include <stdio.h>
#include <stdint.h>

#include "cm3_mcu.h"

#include "project_config.h"

#include "uart_drv.h"

#include "sysfun.h"
#include "ringbuffer.h"

#ifndef DBG_PRINT_UART_PORT
#define DBG_PRINT_UART_PORT     (0)
#endif

#define txbufsize              256

#define rxbufsize              128

#define txbuf_mask            (txbufsize -1)

#define rxbuf_mask            (rxbufsize -1)

#define  STATE_SEND_IDLE            0
#define  STATE_SEND_BUSY            1

static ring_buf_t         sendbuf;
static volatile uint8_t   send_state;
static uint16_t           send_num;

static ring_buf_t         recvbuf;
static uint8_t            tempbuffer[4];

static void printf_callback(uint32_t event, void *p_context)
{
    uint32_t   wr_index, rd_index;
    uint16_t   data_in_fifo;

    /*Except UART_EVENT_TX_DONE, this callback function does NOT care any other event*/
    /*This is ISR, so it is already in priority mode*/

    if (event & UART_EVENT_TX_DONE)
    {

        sendbuf.rd_idx += send_num;
        send_num = 0;

        /*check is there data in buffer need to be send?*/

        rd_index = sendbuf.rd_idx & txbuf_mask;
        wr_index = sendbuf.wr_idx & txbuf_mask;

        if (sendbuf.rd_idx == sendbuf.wr_idx)
        {
            /* buffer empty ---
               except ISR printf, sendbuf.wr_idx will not changed */
            send_state = STATE_SEND_IDLE;
        }
        else
        {
            if (rd_index <= wr_index)
            {
                /* if fact, rd_index will be less than wr_index
                 * equal is empty
                 */
                send_num = wr_index - rd_index;
            }
            else
            {
                send_num = txbufsize - rd_index;
            }
            uart_tx(DBG_PRINT_UART_PORT, &(sendbuf.ring_buf[rd_index]), (uint32_t) send_num);
        }
    }

    if (event & UART_EVENT_RX_DONE)
    {

        data_in_fifo = (recvbuf.wr_idx - recvbuf.rd_idx) & recvbuf.bufsize_mask;

        if (data_in_fifo < rxbuf_mask)
        {
            recvbuf.ring_buf[(recvbuf.wr_idx & recvbuf.bufsize_mask)] = tempbuffer[0];
            recvbuf.wr_idx++;      /*so now wr_idex will pointer to next free bytes*/
        }
        else
        {
            /*drop this input buffer... because no space for input*/
            /*recv buffer full.. so we need to drop data ---
              if RT570 care this data, it should get data from recvbuffer ASAP
              or more buffer for received buffer. default is 128 bytes.*/
        }
        uart_rx(DBG_PRINT_UART_PORT, tempbuffer, 1);
    }

    return ;
}

/**
  Initialize stdout

  \return          STATUS_SUCCESS on success, or  error.
*/
uint32_t console_drv_init(uart_baudrate_t baudrate)
{

    uart_config_t   debug_console_drv_config;
    uint32_t        status;
    static uint8_t  send_buf[txbufsize];
    static uint8_t  recv_buf[rxbufsize];


    /*init ringbuf for TX*/
    sendbuf.ring_buf = send_buf;
    sendbuf.bufsize_mask = txbuf_mask;
    sendbuf.wr_idx = sendbuf.rd_idx = 0;
    send_state = STATE_SEND_IDLE;

    send_num = 0;

    recvbuf.ring_buf = recv_buf;
    recvbuf.bufsize_mask = rxbuf_mask;
    recvbuf.wr_idx = recvbuf.rd_idx = 0;


    /*init debug console uart0, 8bits 1 stopbit, none parity, no flow control.*/
    debug_console_drv_config.baudrate = baudrate;
    debug_console_drv_config.databits = UART_DATA_BITS_8;
    debug_console_drv_config.hwfc     = UART_HWFC_DISABLED;
    debug_console_drv_config.parity   = UART_PARITY_NONE;

    /* Important: p_contex will be the second parameter in uart callback.
     * In this example, we do NOT use p_context, (So we just use NULL)
     * but you can use it for whatever you want.
     */
    debug_console_drv_config.p_context = (void *) NULL;

    debug_console_drv_config.stopbit  = UART_STOPBIT_ONE;
    debug_console_drv_config.interrupt_priority = IRQ_PRIORITY_LOW;

    status = uart_init(DBG_PRINT_UART_PORT, &debug_console_drv_config, printf_callback);

    if (status != STATUS_SUCCESS)
    {
        /*almost impossible for this error...*/
        return  status;
    }

    /*uart device is auto power on in uart_init function */
    uart_rx(DBG_PRINT_UART_PORT, tempbuffer, 1);


#if defined(__GNUC__)
    setbuf(stdout, NULL);
#endif

    return (STATUS_SUCCESS);
}

static void uart_send(int ch)
{
    uint16_t   data_in_fifo, free_space_in_fifo;

    uint16_t   rd_index;

    /* this uart_send has two version:
     * 1. for only single thread/FSM version: printf is only called by one task at any time.
     * 2. there are multiple tasks version: printf maybe called for any thread at the same time.
     */

wait_free_space:

#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
    enter_critical_section();
#endif

    data_in_fifo = (sendbuf.wr_idx - sendbuf.rd_idx) & txbuf_mask;
    free_space_in_fifo = txbuf_mask - data_in_fifo;

    if (free_space_in_fifo)
    {
        sendbuf.ring_buf[(sendbuf.wr_idx & txbuf_mask)] = (uint8_t) ch;
        sendbuf.wr_idx++;
    }
    else
    {
        /*no space for printf buffer, relese critical section first*/

#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
        leave_critical_section();
#endif

        if (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk)
        {
            /*running in interrupt mode*/
            return ;        /*no space left, so drop*/
        }
        else
        {
            /*For multi-tasking environment, it can release CPU here*/
            goto wait_free_space;
        }
    }

#if (MODULE_ENABLE(SUPPORT_MULTITASKING))

    /* check current TX state ---
     * in multiple task environment, this function is protected by disable interrupt.
     */
    if (send_state == STATE_SEND_IDLE)
    {
        /*check rd position, wr position*/

        rd_index = sendbuf.rd_idx & txbuf_mask;
        send_num = 1;

        send_state = STATE_SEND_BUSY;

        leave_critical_section();

        uart_tx(DBG_PRINT_UART_PORT, &(sendbuf.ring_buf[rd_index]), (uint32_t) send_num);

    }
    else
    {
        /*uart0 is busy in transmit, so data will send next interrupt done.*/
        leave_critical_section();
    }

#else
    /*check current TX state*/
    if (send_state == STATE_SEND_IDLE)
    {
        /*check rd position, wr position*/

        /* for some special case, what we write into fifo already been sent
         * because current system is very busy... too many interrupts happen
         * so in this case, rd_idx already been increased.
         */
        if (sendbuf.wr_idx != sendbuf.rd_idx)
        {

            rd_index = sendbuf.rd_idx & txbuf_mask;
            send_num = 1;

            send_state = STATE_SEND_BUSY;

            uart_tx(DBG_PRINT_UART_PORT, &(sendbuf.ring_buf[rd_index]), (uint32_t) send_num);
        }
    }

#endif

    return;
}

/*This function is for sleep function*/
void console_sleep(void)
{
    enter_critical_section();

    uart_tx_abort(DBG_PRINT_UART_PORT);

    /*reinit console.*/
    sendbuf.wr_idx = sendbuf.rd_idx = 0;
    send_state = STATE_SEND_IDLE;
    send_num = 0;

    leave_critical_section();
}

uint32_t check_uart0_rx_buffer_data_num(void)
{
    /*
     * return "current number of character" in rx buffer, it could be changed anytime
     * But in most time, application just want to know is there data in rx buffer?
     */
    return ((recvbuf.wr_idx - recvbuf.rd_idx) & recvbuf.bufsize_mask);
}

#if defined(__CC_ARM)

/**
  Put a character to the stdout

  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int fputc(int ch, FILE *p_file)
{

    uart_send(ch);

    if (ch == 0x0A)
    {
        /*patch for "\n"*/
        uart_send(0x0D);
    }

    return (ch);
}


int fgetc(FILE *p_file)
{
    uint8_t input;

    /*
     * Remak: This function is designed for single thread
     * It does NOT consider multiple threads race condition issue.
     */

    while (recvbuf.wr_idx == recvbuf.rd_idx)
        ;       /*block to until host send data*/

    input = recvbuf.ring_buf[(recvbuf.rd_idx & recvbuf.bufsize_mask)];
    recvbuf.rd_idx++;

    return input;
}

int stdout_char(int ch)
{
    int     rval = 0;

    uart_send(ch);
    return rval;
}

int stdout_string(char *str, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        uart_send(str[i]);
    }
    return length ;
}

int stdin_str(char *pBuf, int length)
{
    int         byte_cnt = 0;
    uint16_t    rd_pos = recvbuf.rd_idx;
    uint16_t    wr_pos = recvbuf.wr_idx;

    while (1)
    {
        if ( rd_pos == wr_pos )
        {
            break;
        }
        if ( length == byte_cnt )
        {
            break;
        }
        pBuf[byte_cnt++] = recvbuf.ring_buf[rd_pos];
        rd_pos = (rd_pos + 1) % (recvbuf.bufsize_mask + 1);
    }
    recvbuf.rd_idx = rd_pos;
    return byte_cnt;
}

#elif defined(__GNUC__)

int _write(int fd, char *ptr, int len)
{
    int  i = len;

    while (i > 0)
    {

        uart_send(*ptr);

        if (*ptr == 0x0A)
        {
            uart_send(0x0D);
        }

        ptr++;
        i--;
    }

    return len;
}


int _read(int fd, char *ptr, int len)
{

    while (recvbuf.wr_idx == recvbuf.rd_idx)
        ;       /*block to until host send data*/

    *ptr = recvbuf.ring_buf[(recvbuf.rd_idx & recvbuf.bufsize_mask)];
    recvbuf.rd_idx++;

    return 1;
}





#endif

