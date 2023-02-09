/* OS Wrapper APIs*/
#include "sys_arch.h"

#include "bsp_uart.h"
/* +-------- Packet in IAP memory -----+-----+---------+-----------+
 * | 0      |  1     |  2    |  3      | ... | n+4     |     n+5   |
 * |--------+--------+-------+---------+-----+---------+-----------+
 * |  start | number | !num  | data[0] | ... | data[n] | checksum  |
 * +--------+--------+-------+---------+-----+---------+-----------+
 * the first byte is left unused for memory alignment reasons                 */

#define TARGET_IMAGE_INFO_SAVE_ADDR     0x0007C000
#define TARGET_IMGAE_SAVE_ADDR          (0x0007C100)

#define SOH                     ((uint8_t)0x01)  /* start of 128-byte data packet */
#define STX                     ((uint8_t)0x02)  /* start of 1024-byte data packet */
#define EOT                     ((uint8_t)0x04)  /* end of transmission */
#define ACK                     ((uint8_t)0x06)  /* acknowledge */
#define NAK                     ((uint8_t)0x15)  /* negative acknowledge */
#define CAN                     ((uint32_t)0x18) /* two of these in succession aborts transfer */
#define CRC16                   ((uint8_t)0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  ((uint8_t)0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  ((uint8_t)0x61)  /* 'a' == 0x61, abort by user */

#define PACKET_HEADER_SIZE      ((uint32_t)3)
#define PACKET_DATA_INDEX       ((uint32_t)3)
#define PACKET_START_INDEX      ((uint32_t)0)
#define PACKET_NUMBER_INDEX     ((uint32_t)1)
#define PACKET_CNUMBER_INDEX    ((uint32_t)2)
#define PACKET_TRAILER_SIZE     ((uint32_t)1)
#define PACKET_OVERHEAD_SIZE    (PACKET_HEADER_SIZE + PACKET_TRAILER_SIZE - 1)

#define PACKET_SIZE             ((uint32_t)128)

#define FRAME_TIMEOUT           ((uint32_t)1000)
#define TX_TIMEOUT              ((uint32_t)100)
#define RX_TIMEOUT              ((uint32_t)100)

#define MAX_ERRORS              ((uint8_t)160)

static uint8_t u8_flash_data[0x100];
static uint8_t PacketData[PACKET_SIZE + PACKET_HEADER_SIZE + PACKET_TRAILER_SIZE];
static uint32_t crc_error = 0;
static uint32_t u32_total_size = 0;


static int _fc_write_raw(uintptr_t buf_addr, uintptr_t flash_addr, uint32_t bytes)
{
    int ret = 0;
    uint8_t w_buf[0x100];
    uint32_t remain_bytes = bytes;
    uint32_t wr_bytes = 0;

    do
    {
        memset(w_buf, 0xff, 0x100);

        if (remain_bytes > 0x100)
        {
            wr_bytes = 0x100;
        }
        else
        {
            wr_bytes = remain_bytes;
        }

        memcpy(w_buf, (uint8_t *)buf_addr, wr_bytes);
        while (flash_check_busy());
        flash_write_page((uint32_t)w_buf, flash_addr);
        remain_bytes -= wr_bytes;
        flash_addr += wr_bytes;
        buf_addr += wr_bytes;

    } while (remain_bytes);
    return ret;
}

/**
  * @brief  Calculate Check sum for YModem Packet
  * @param  p_data Pointer to input data
  * @param  size length of input data
  * @retval uint8_t checksum value
  */
uint8_t CalcChecksum(const uint8_t *p_data, uint32_t size)
{
    uint32_t sum = 0;
    const uint8_t *p_data_end = p_data + size;

    while (p_data < p_data_end )
    {
        sum += *p_data++;
    }

    return (sum & 0xffu);
}

static int ReceivePacket(uint8_t *p_data, uint32_t *p_length)
{
    uint32_t crc;
    uint16_t packet_size = 0;
    int status = -1;
    uint32_t retry_cnt = 0;

    while (bsp_uart_recv(&p_data[PACKET_START_INDEX], 1) == 0);

    switch (p_data[PACKET_START_INDEX])
    {
    case SOH:
        packet_size = bsp_uart_recv(&p_data[PACKET_NUMBER_INDEX], PACKET_SIZE + PACKET_OVERHEAD_SIZE);
_chk:
        if (retry_cnt > 0xFFFF)
        {
            break;
        }
        if (packet_size ==  (PACKET_SIZE + PACKET_OVERHEAD_SIZE))
        {
            if (p_data[PACKET_NUMBER_INDEX] == ((p_data[PACKET_CNUMBER_INDEX]) ^ 0xFF))
            {
                crc = p_data[packet_size - PACKET_OVERHEAD_SIZE + PACKET_DATA_INDEX];
                if (CalcChecksum(&p_data[PACKET_DATA_INDEX], (packet_size - PACKET_OVERHEAD_SIZE)) == crc)
                {

                    status = 0;
                }
                else
                {
                    crc_error++;
                }
                packet_size -= PACKET_OVERHEAD_SIZE;
            }
        }
        else
        {
            packet_size += bsp_uart_recv(&p_data[PACKET_NUMBER_INDEX + packet_size], (PACKET_SIZE + PACKET_OVERHEAD_SIZE - packet_size));
            retry_cnt++;
            goto _chk;
        }
        //pkt_num_error++;
        break;
    case EOT:
        status = 0;
        break;
    default:
        break;
    }

    *p_length = packet_size;
    return status;
}

int xmodem_receive(void)
{
    uint32_t packet_length = 0, file_done = 0, errors = 0;
    uint32_t flashdestination = TARGET_IMGAE_SAVE_ADDR;
    uint8_t packets_received = 1;
    int result = 0;

    bsp_uart_stdout_char(NAK);
    //vTaskDelay(1);
    while ((file_done == 0) && (result == 0))
    {
        result = ReceivePacket(PacketData, &packet_length);

        if (result == 0)
        {
            errors = 0;
            if (packet_length == 0)
            {
                bsp_uart_stdout_char(ACK);
                file_done = 1;
                _fc_write_raw((uintptr_t)&u32_total_size, TARGET_IMAGE_INFO_SAVE_ADDR, 4);
                while (flash_check_busy());
                flash_write_page((uint32_t)u8_flash_data, flashdestination);
                info("~\n");
            }
            else
            {
                if (PacketData[PACKET_NUMBER_INDEX] != packets_received)
                {
                    bsp_uart_stdout_char(NAK);
                }
                else
                {
                    info("#");

                    memcpy(&u8_flash_data[u32_total_size % 0x100], &PacketData[PACKET_DATA_INDEX], packet_length);
                    u32_total_size += packet_length;
                    if ((u32_total_size % 0x100 == 0))
                    {
                        while (flash_check_busy());
                        flash_write_page((uint32_t)u8_flash_data, flashdestination);
                        flashdestination += 0x100;
                    }
                    bsp_uart_stdout_char(ACK);
                    packets_received ++;
                }
            }
        }
        else
        {
            if (packets_received > 0)
            {
                errors ++;
            }
            if (errors > MAX_ERRORS)
            {
                /* Abort communication */
                bsp_uart_stdout_char(CAN);
                result = -1;
            }
            else
            {
                bsp_uart_flush();
                /* Ask for a resend packet */
                bsp_uart_stdout_char(NAK);
                result = 0;
            }
        }
        //vTaskDelay(3);
    }

    if (result == -1)
    {
        info("\r\n==Receive Package TIMEOUT==\n");
        bsp_uart_stdout_char(EOT);
    }

    return result;
}

void xmodem_recv_init(void)
{

    uint8_t i;

    //Delay_Init();

    for (i = 0; i < 0x74; i++)
    {
        while (flash_check_busy());
        flash_erase(FLASH_ERASE_PAGE, TARGET_IMAGE_INFO_SAVE_ADDR + (0x1000 * i));
    }

    //NVIC_SetPriority(Uart0_IRQn, 0);
    //vPortEnterCritical();
    u32_total_size = 0;
    xmodem_receive();
    //vPortExitCritical();
    //NVIC_SetPriority(Uart0_IRQn, 6);
    info("total size = %d\n", u32_total_size);
}

void show_cnt(void)
{
    info("crc_error = %d\n", crc_error);

}
