/**************************************************************************//**
 * @file     I2C_MASTER.c
 * @version
 * @brief    I2C Master driver
 *
 * @copyright
 ******************************************************************************/

#include "cm3_mcu.h"

/*
 * HERE We assume there are multiple i2c devices attached with one I2C master.
 * It is possible for multitask to access the I2C master at same time.
 * So we need critical section to protect.
 *
 */

#define I2C_STATE_UNINIT         0
#define I2C_STATE_IDLE           1

#define I2C_STATE_WRITE          2
#define I2C_STATE_WRITE_DATA     3

#define I2C_STATE_READ           4
#define I2C_STATE_READ_WADDR     5
#define I2C_STATE_READ_RDATA     6

/*Please do NOT change the order of these define.*/
#define I2C_STOP_MASK           16     /*BIT5 as stop MASK*/
#define I2C_STATE_NOACK         16
#define I2C_STATE_STOP          17
#define I2C_STATE_ESTOP         18

#define I2C_STATE_HOST_INIT     32     /*this mode only in i2c_init, avoid multi-task i2c_init at same time*/

/*i2c_state is critical data*/
static uint16_t i2c_state = I2C_STATE_UNINIT;       /*current i2c host state*/

static uint16_t i2c_substate;

/*local data used in interrupt service routine.*/
static uint16_t  remain_data_length;        /*the maximum data length is 1K*/
static uint8_t   *pdata;
static i2cm_proc_cb_t   notify_cb;
static const i2c_slave_data_t *read_slave;

#define  HIGH_SPEED_DIV        (20-1)
#define  MEDIUM_SPEED_DIV      (40-1)
#define  NORMAL_SPEED_DIV      (80-1)
#define  SUPER_HIGH_SPEED_DIV  (10-1)

#define  I2C_MODE_READ          1
#define  I2C_MODE_WRITE         0

#define  I2C_MAX_DATASIZE      (1024)       /*max data packet size for one transfer*/



uint32_t i2c_preinit(uint32_t SCL_pin, uint32_t SDA_pin)
{
    uint32_t  times = 0;

    /*check SCL and SDA pin setting*/
    if ((SCL_pin != 22) && (SCL_pin != 20) && (SCL_pin != 18))
    {
        return STATUS_INVALID_PARAM;
    }

    if ((SDA_pin != 23) && (SDA_pin != 21) && (SDA_pin != 19))
    {
        return STATUS_INVALID_PARAM;
    }

    /*clear I2C FIFO*/
    I2CM->STATUS = I2CM_STATUS_FIFO_CLR;

    while (I2CM->STATUS  & I2CM_STATUS_FIFO_CLR)
        ;

    I2CM->CONTROL = 0;      /*I2C disable first*/

    /*clear pending interrupt*/
    NVIC_ClearPendingIRQ(I2cm_IRQn);

    pin_set_mode(SCL_pin, MODE_GPIO);     /*set SCL output*/
    pin_set_mode(SDA_pin, MODE_GPIO);     /*set SDA  output*/

    gpio_pin_set(SCL_pin);               /*SCL high*/
    gpio_pin_set(SDA_pin);               /*SDA high*/

    gpio_cfg_output(SCL_pin);
    gpio_cfg_output(SDA_pin);

    __NOP();
    __NOP();
    __NOP();
    __NOP();

    for (times = 0; times < 9; times++)
    {
        gpio_pin_clear(SCL_pin);

        __NOP();            /*Is __NOP enough? */
        __NOP();
        __NOP();
        __NOP();

        gpio_pin_set(SCL_pin);

        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }

    /*set  SCL,  SDA  I2C mode.*/
    pin_set_mode(SCL_pin, MODE_I2C);
    pin_set_mode(SDA_pin, MODE_I2C);

    /*set SCL, SDA as open drain mode.*/
    enable_pin_opendrain(SCL_pin);
    enable_pin_opendrain(SDA_pin);

    return STATUS_SUCCESS;
}

/*
*If no multitask to acess i2c device, maybe we can forget "critical_section".
*/

uint32_t i2c_init(uint32_t I2C_SPEED)
{
    uint32_t  div;

    enter_critical_section();
    /*We can set i2c host only when i2c_state in idle/uninit mode*/
    if (i2c_state > I2C_STATE_IDLE)
    {
        leave_critical_section();
        return STATUS_INVALID_REQUEST;
    }

    /*i2c host in UNINIT or IDLE mode now*/
    i2c_state = I2C_STATE_HOST_INIT;    /*this is temp state, only avoid multi-task call i2c_init*/

    leave_critical_section();

    if (I2C_SPEED == I2C_CLOCK_400K)
    {
        div = HIGH_SPEED_DIV;
    }
    else if (I2C_SPEED == I2C_CLOCK_200K)
    {
        div = MEDIUM_SPEED_DIV;
    }
    else if (I2C_SPEED == I2C_CLOCK_800K)
    {
        div = SUPER_HIGH_SPEED_DIV;
    }
    else
    {
        /*I2C default is 100K*/
        div = NORMAL_SPEED_DIV;
    }


    I2CM->PRESCALE_H = (div >> I2CM_CLK_PRESCALE_SHIFT);
    /*I2C Master use external clock */
    I2CM->CONTROL = (div & I2CM_CTRL_CLK_DIV_MASK) | I2CM_CTRL_CLK_SRC_EXT;

    /* Notice: We don't enable I2C right now..
     * We will enable it in read/write transfer*/

    /*we will use interrupt mode*/
    /*until now, we don't want any interrupt generated.*/
    I2CM->INT_ENABLE = 0;

    /*clear I2C interrupt status register */
    I2CM->INT_STATUS = 0xF;

    /*interrupt priority set */
    /*enable NVIC I2C interrrupt */
    NVIC_EnableIRQ(I2cm_IRQn);

    enter_critical_section();
    i2c_state = I2C_STATE_IDLE;         /*OK, right now we can use I2C host to transfer data*/
    leave_critical_section();

    return STATUS_SUCCESS;
}

/*
* We write this function to avoid multi-tasking access for i2c host
*
*/
static uint32_t i2c_check_state(uint16_t next_state)
{
    enter_critical_section();

    if (i2c_state != I2C_STATE_IDLE)
    {
        leave_critical_section();
        if (i2c_state != I2C_STATE_UNINIT)
        {
            return STATUS_EBUSY;    /*I2C host in read/write state... busy..*/
        }
        else
        {
            return STATUS_NO_INIT;
        }
    }
    i2c_state = next_state;        /*so other task can not read/write now.*/

    leave_critical_section();

    return STATUS_SUCCESS;
}

/*
* this function is write "dev_addr_write+ Read/Write" "register_addr"
*
*/
static void i2c_write_addr_reg(const i2c_slave_data_t *slave, uint32_t bReadFlag)
{
    I2CM->COMMAND = I2CM_CMD_STRT;      /*send Start condition*/
    I2CM->COMMAND = I2CM_CMD_WDAT8;

    if (bReadFlag)
    {
        I2CM->COMMAND = (slave->dev_addr << 1) | 1;    /*I2C read bit8 is 1*/
    }
    else
    {
        I2CM->COMMAND = (slave->dev_addr << 1);    /*I2C write bit8 is 0*/
    }

    I2CM->COMMAND = I2CM_CMD_VACK;

    if (bReadFlag)
    {
        return;    /*Read operation do NOT have register address*/
    }

    if (slave->bFlag_16bits)
    {
        /*If slave device need 16bits register address*/
        I2CM->COMMAND = I2CM_CMD_WDAT8;
        I2CM->COMMAND = (slave->reg_addr >> 8);     /*High register address*/
        I2CM->COMMAND = I2CM_CMD_VACK;
    }

    I2CM->COMMAND = I2CM_CMD_WDAT8;
    I2CM->COMMAND = (slave->reg_addr & 0xFF);    /*Low register address*/
    I2CM->COMMAND = I2CM_CMD_VACK;

    //dprintf("Check status A %08x \n", I2CM->STATUS);

    return;
}

uint32_t i2c_write(
    const i2c_slave_data_t  *slave,
    uint8_t *data,
    uint32_t len,
    i2cm_proc_cb_t  endproc_cb
)
{
    uint32_t status;

    assert_param(slave);           /*slave data should not be NULL*/
    assert_param(data);            /*data should not be NULL*/
    assert_param(endproc_cb);      /*endproc_cb hook should not be NULL*/

    if (len > I2C_MAX_DATASIZE)
    {
        return STATUS_INVALID_PARAM;    /*packet data is > 1K bytes...*/
    }

    status = i2c_check_state(I2C_STATE_WRITE);

    if (status)
    {
        return status;
    }

    pdata = data;
    remain_data_length = len;
    notify_cb = endproc_cb;

    /*Here Command FIFO should be empty.*/
    if (!(I2CM->STATUS & I2CM_STATUS_CMDFIFO_EMPTY))
    {
        dprintf0("Warning Check why I2C CMDFIFO is not empty \n");
    }

    i2c_write_addr_reg(slave, I2C_MODE_WRITE);
    i2c_substate = I2C_STATE_WRITE_DATA;

    /*Enable I2C_ENABLE bit in control register*/
    I2CM->CONTROL |= I2CM_CTRL_ENABLE;
    /*enable interrupt*/
    I2CM->INT_ENABLE = (I2CM_INT_CMDFIFO_EMPTY | I2CM_INT_ERROR);

    return STATUS_SUCCESS;
}

uint32_t i2c_read(
    const i2c_slave_data_t  *slave,
    uint8_t  *data,
    uint32_t len,
    i2cm_proc_cb_t  endproc_cb
)
{
    uint32_t status;

    assert_param(slave);           /*slave data should not be NULL*/
    assert_param(data);            /*data should not be NULL*/
    assert_param(endproc_cb);      /*endproc_cb hook should not be NULL*/

    if ((len > I2C_MAX_DATASIZE) || (len == 0))
    {
        return STATUS_INVALID_PARAM;    /*packet data is > 1K bytes. or zero bytes?*/
    }

    status = i2c_check_state(I2C_STATE_READ);

    if (status)
    {
        return status;
    }

    pdata = data;
    remain_data_length = len;
    notify_cb = endproc_cb;

    /*Here Command FIFO should be empty.*/
    if (!(I2CM->STATUS & I2CM_STATUS_CMDFIFO_EMPTY))
    {
        dprintf0("Warning Check why I2C CMDFIFO is not empty \n");
    }

    read_slave = slave;

    i2c_substate = I2C_STATE_READ_WADDR;
    i2c_write_addr_reg(slave, I2C_MODE_WRITE);

    /*Enable I2C_ENABLE bit in control register*/
    I2CM->CONTROL |= I2CM_CTRL_ENABLE;
    /* enable interrupt, we don't enable DATAREADY interrupt
     * but we will check it in ISR when command fifo empty.
     * Becaue we don't want too many interrupts happen.
     */
    I2CM->INT_ENABLE = (I2CM_INT_CMDFIFO_EMPTY | I2CM_INT_ERROR);

    return STATUS_SUCCESS;
}


static void i2c_idle(void)
{
    enter_critical_section();
    i2c_state = I2C_STATE_IDLE;
    leave_critical_section();
    I2CM->INT_ENABLE = 0;       /*disable all interrupt*/



    /* Notice: IF you in read operation, I2CM->STATUS
     * will get 0x5 for the end transfer, but bit2 is not
     * error, it just "NAK" that host send.
     */
    //dprintf("STOP  STATUS %08x \n",  I2CM->STATUS);
}

/*I2C Master Interrupt*/
void i2cm_handler(void)
{
    uint32_t status;

    status = I2CM->INT_STATUS;
    //dprintf("INT STATUS %08x \n", status);
    I2CM->INT_STATUS = status;      /*clear interrupt*/

    if (status & I2CM_INT_ERROR)
    {
        /*this is highest priority --- No ACK*/
        /*in most case, this error should not happen*/

        if (!(I2CM->STATUS & I2CM_STATUS_CMDFIFO_EMPTY))
        {
            /*we should clear FIFO when error */
            I2CM->STATUS = (I2CM_STATUS_I2C_ERR_CLEAR | I2CM_STATUS_FIFO_CLR);
            /*1. We don't do FIFO clear check.. because it is very fast.*/
        }

        if (i2c_substate < I2C_STATE_NOACK)
        {
            /*we can not send CMD_STOP here.. because it be influenced by FIFO_CLR.*/
            remain_data_length = 0;         /*stop to send/receive any data. because error*/
            i2c_substate = I2C_STATE_NOACK;
        }
    }

    /*check data in recv fifo... read phase.*/
    while ( I2CM->STATUS & I2CM_STATUS_DATA_READY)
    {
        /* Only i2c_substate I2C_STATE_READ_RDATA or I2C_STATE_STOP (the last)
         * will enter this read loop*/
        /*read one data in Recv FIFO*/
        *pdata++ = I2CM->RD_DATA;

        /*then try to send command for next one byte
         *If required.
         */
        if (remain_data_length)
        {
            if (I2CM->STATUS & I2CM_STATUS_CMDFIFO_OVERFL)
            {
                /*this is check status... should not happen*/
                dprintf0("Error CMDFIFO overflow \n");
            }

            I2CM->COMMAND = I2CM_CMD_RDAT8;

            remain_data_length--;
            if (remain_data_length)
            {
                I2CM->COMMAND = I2CM_CMD_WDAT0;
            }
            else
            {
                I2CM->COMMAND = I2CM_CMD_WDAT1;
                /*the last one byte, so we add stop condition.*/
                I2CM->COMMAND = I2CM_CMD_STOP;
                i2c_substate = I2C_STATE_STOP;
            }
        }
    }

    if (status & I2CM_INT_CMDFIFO_EMPTY)
    {
        /*command fifo empty, ok we can stuff next data*/

        if (I2CM->STATUS & I2CM_STATUS_CMDFIFO_EMPTY)
        {
            if (i2c_substate == I2C_STATE_WRITE_DATA)
            {
                if (remain_data_length > 0)
                {
                    int i;
                    /*we will write 8 bytes into FIFO*/
                    for (i = 0; i < 8; i++)
                    {

                        I2CM->COMMAND = I2CM_CMD_WDAT8;
                        I2CM->COMMAND = *pdata++;
                        I2CM->COMMAND = I2CM_CMD_VACK;

                        remain_data_length--;

                        if (remain_data_length == 0)
                        {
                            I2CM->COMMAND = I2CM_CMD_STOP;  /*send stop to terminate the transfer.*/
                            i2c_substate = I2C_STATE_STOP;
                            break;
                        }

                    }
                }
            }
            else if (i2c_substate == I2C_STATE_READ_WADDR)
            {
                i2c_substate = I2C_STATE_READ_RDATA;
                i2c_write_addr_reg(read_slave, I2C_MODE_READ);

                /*send read data request*/
                if (remain_data_length > 0)
                {
                    int i;
                    /*we will read maximum 12 bytes into FIFO*/
                    for (i = 0; i < 12; i++)
                    {
                        I2CM->COMMAND = I2CM_CMD_RDAT8;

                        remain_data_length--;
                        if (remain_data_length)
                        {
                            I2CM->COMMAND = I2CM_CMD_WDAT0;
                        }
                        else
                        {
                            I2CM->COMMAND = I2CM_CMD_WDAT1;
                            /*the last one byte, so we add stop condition.*/
                            I2CM->COMMAND = I2CM_CMD_STOP;
                            i2c_substate = I2C_STATE_STOP;
                            break;
                        }
                    }
                }
            }
            else if (i2c_substate & I2C_STOP_MASK)
            {
                if (i2c_substate == I2C_STATE_NOACK)
                {
                    /*send stop for NOACK case... this is error case.*/
                    I2CM->COMMAND = I2CM_CMD_STOP;  /*send stop to terminate the transfer.*/
                    /*avoid to send CMD_STOP TWICE. (next cmd fifo empty, don't enter here)*/
                    i2c_substate = I2C_STATE_ESTOP;
                }
                /*we will want to know STOP send finish*/
                I2CM->INT_ENABLE = I2CM_INT_COMPLETE;
            }
        }
        else
        {
            /* Because we stuff some command into fifo, like
             * CMD_STOP to fifo. so we see this state.
             * Ignore this case.
             */
            return;
        }

    }

    if (i2c_substate >= I2C_STATE_STOP)
    {
        /*only need to know the last STOP condition in BUS?*/
        if (!(I2CM->STATUS & I2CM_STATUS_CMDFIFO_EMPTY))
        {
            return;    /*STOP still in command FIFO*/
        }

        if (status & I2CM_INT_COMPLETE)
        {
            uint32_t ret_status;

            i2c_idle();
            if (i2c_substate == I2C_STATE_STOP)
            {
                ret_status = I2C_STATUS_OK;
            }
            else
            {
                ret_status = I2C_STATUS_ERR_NOACK;
            }

            notify_cb(ret_status);     /*let application hook to know error*/
        }
    }

    return;
}




