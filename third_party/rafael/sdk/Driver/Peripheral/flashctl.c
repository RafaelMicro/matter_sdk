/**************************************************************************/ /**
                                                                              * @file     flashctl.c
                                                                              * @version
                                                                              * @brief
                                                                              *
                                                                              * @copyright
                                                                              *****************************************************************************/

#include "cm3_mcu.h"
#include "project_config.h"

#define FLASH_UNLOCK_PATTER 0x52414254

#if LPWR_FLASH_PROTECT_ENABLE == 1

#define VBAT_VALUE 2500
#define ms_sec(N) (N * 1000)

static volatile sadc_convert_state_t sadc_convert_vbat_status = SADC_CONVERT_IDLE;
static sadc_value_t sadc_convert_vbat_value;
static uint32_t flash_protect = FALSE;

uint32_t Vbat_Comp_Config(comp_config_t * p_config);
void Vbat_Comp_Int_Callback_Handler(void);

uint32_t Sadc_VBAT_value(void);
void Sadc_Int_vbat_Callback_Handler(sadc_cb_t * p_cb);

void Wdt_Reset_Init(void)
{
    wdt_config_mode_t wdt_mode;
    wdt_config_tick_t wdt_cfg_ticks;

    Wdt_Reset_Event_Clear();

    wdt_mode.int_enable   = 0;               /*wdt interrupt enable field*/
    wdt_mode.reset_enable = 1;               /*wdt reset enable field*/
    wdt_mode.lock_enable  = 1;               /*wdt lock enable field*/
    wdt_mode.prescale     = WDT_PRESCALE_32; /*SYS_CLK(32MHz)/32 = 1MHz*/

    wdt_cfg_ticks.wdt_ticks     = ms_sec(1);
    wdt_cfg_ticks.int_ticks     = ms_sec(0);
    wdt_cfg_ticks.wdt_min_ticks = ms_sec(0);

    Wdt_Start(wdt_mode, wdt_cfg_ticks, NULL); /*wdt reset time = 50ms, window min 1ms*/
}

void Vbat_Comp_Int_Callback_Handler(void)
{
    if (COMP_OUT_GET())
    {
    }
    else
    {
        Wdt_Reset_Init();
        while (1)
            ;
    }
}

uint32_t Vbat_Comp_Config(comp_config_t * p_config)
{
    if (p_config == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    p_config->comp_selref   = COMP_CONFIG_SELREF_INTERNAL;   /*Select the Comparator input N source: BG 0.6V*/
    p_config->comp_ref_sel  = COMP_CONFIG_REFSEL_CHANNEL_0;  /*Select the Comparator N input for comparision: Channel 0*/
    p_config->comp_selinput = COMP_CONFIG_SELINPUT_INTERNAL; /*Select the Comparator input P source: Internal vdd div*/
    p_config->comp_ch_sel   = COMP_CONFIG_CHSEL_CHANNEL_0;   /*Select the Comparator P input for comparision: Channel 0*/
    p_config->comp_swdiv    = COMP_CONFIG_SWDIV_RES;         /*Switch the Comparator vdd div type: RES div*/
    p_config->comp_v_sel = COMP_CONFIG_VOLTAGE_10; /*Select the Comparator internal vdd div voltage for Power Fail: By MP setting*/
    p_config->comp_int_pol   = COMP_CONFIG_INT_POL_BOTH;     /*Set the Comparator interrupt polarity: Both edge*/
    p_config->comp_ds_wakeup = COMP_CONFIG_DS_WAKEUP_ENABLE; /*Select the Comparator wakeup in DeepSleep: Enable*/
    p_config->comp_ds_inv =
        COMP_CONFIG_DS_INVERT_DISABLE; /*Select the invert of the Comparator output for waking up from DeepSleep: Disable*/
    p_config->comp_pw      = COMP_CONFIG_PW_LARGEST; /*Select the Comparator current: Largest*/
    p_config->comp_hys_sel = COMP_CONFIG_HYS_80;     /*Select the Comparator hys window: 130mV*/

    return STATUS_SUCCESS;
}

void Sadc_Int_vbat_Callback_Handler(sadc_cb_t * p_cb)
{
    if (p_cb->type == SADC_CB_SAMPLE)
    {
        sadc_convert_vbat_value  = p_cb->data.sample.value;
        sadc_convert_vbat_status = SADC_CONVERT_DONE;
    }
}

uint32_t Sadc_Read_VBAT_value()
{
    uint32_t value = 0;

    sadc_convert_vbat_status = SADC_CONVERT_IDLE;

    while (1)
    {
        if (sadc_convert_vbat_status == SADC_CONVERT_DONE)
        {
            value = sadc_convert_vbat_value;
            break;
        }
        else if (sadc_convert_vbat_status == SADC_CONVERT_IDLE)
        {
            sadc_convert_vbat_status = SADC_CONVERT_START;

            if (Sadc_Channel_Read_Vbat(SADC_CH_VBAT) != STATUS_SUCCESS)
            {
                sadc_convert_vbat_status = SADC_CONVERT_IDLE;
            }
        }
    }

    sadc_convert_vbat_status = SADC_CONVERT_IDLE;

    return value;
}

uint32_t Sadc_VBAT_value()
{
    uint32_t value = 0;

    sadc_convert_vbat_status = SADC_CONVERT_IDLE;

    while (1)
    {
        if (sadc_convert_vbat_status == SADC_CONVERT_DONE)
        {
            if (sadc_convert_vbat_value < VBAT_VALUE)
            {
                sadc_convert_vbat_status = SADC_CONVERT_IDLE;
            }
            else
            {
                break;
            }
        }
        else if (sadc_convert_vbat_status == SADC_CONVERT_IDLE)
        {
            sadc_convert_vbat_status = SADC_CONVERT_START;

            if (Sadc_Channel_Read_Vbat(SADC_CH_VBAT) != STATUS_SUCCESS)
            {
                sadc_convert_vbat_status = SADC_CONVERT_IDLE;
            }
        }
    }

    sadc_convert_vbat_status = SADC_CONVERT_IDLE;

    return value;
}

void flash_cmp_protect_init(void)
{
    comp_config_t p_comp_config;
    Vbat_Comp_Config(&p_comp_config);
    Comp_Init(&p_comp_config, Vbat_Comp_Int_Callback_Handler); /*Init Comparator*/
    Comp_Enable();                                             /*Enable Comparator*/
}

uint32_t flash_vbat_read(void)
{ /*Enable SADC Read VBAT*/
    Sadc_Config_Enable_Vbat(SADC_RES_12BIT, SADC_OVERSAMPLE_0, Sadc_Int_vbat_Callback_Handler);
    Sadc_Compensation_Init(1);
    return Sadc_Read_VBAT_value();
}

void flash_vbat_protect_init(void)
{ /*Enable SADC Read VBAT*/
    Sadc_Config_Enable_Vbat(SADC_RES_12BIT, SADC_OVERSAMPLE_0, Sadc_Int_vbat_Callback_Handler);
    Sadc_Compensation_Init(1);
    Sadc_VBAT_value();
}
#endif

void flash_suspend_check(void)
{
    flash_status_t flash_status;

    flash_status.require_mode = (FLASH_STATUS_RW2);

    flash_get_status_reg(&flash_status);

    if ((flash_status.status2 & FLASH_SUSPEND_STATUS) == FLASH_SUSPEND_STATUS) // suspend status
    {
        FLASH->COMMAND     = CMD_MANUAL_MODE;                                  // manual mode
        FLASH->FLASH_INSTR = CMD_FLASH_RESET_ENABLE;                           // enable reset
        FLASH->PATTERN     = FLASH_UNLOCK_PATTER;
        FLASH->START       = STARTBIT;
        while (flash_check_busy())
        {
            ;
        }                                     // wait busy finish

        FLASH->FLASH_INSTR = CMD_FLASH_RESET; // reset
        FLASH->PATTERN     = FLASH_UNLOCK_PATTER;
        FLASH->START       = STARTBIT;
        while (flash_check_busy())
        {
            ;
        }             // wait busy finish

        Delay_ms(25); // Delay time
    }
}

uint32_t flash_get_deviceinfo(void)
{
    return (FLASH->FLASH_INFO);
}

flash_size_t flash_size(void)
{
    uint32_t flash_size_id;

    flash_size_id = ((flash_get_deviceinfo() >> FLASH_SIZE_ID_SHIFT) & 0xFF);

    if (flash_size_id == FLASH_512K)
    {
        return FLASH_512K;
    }
    else if (flash_size_id == FLASH_1024K)
    {
        return FLASH_1024K;
    }
    else if (flash_size_id == FLASH_2048K)
    {
        return FLASH_2048K;
    }
    else
    {
        return FLASH_NOT_SUPPORT;
    }
}

uint32_t flash_check_address(uint32_t flash_address, uint32_t length)
{
#if (!MODULE_ENABLE(FLASH_CHECK_ADDRESS_DISABLE))

    uint16_t flash_size_id;

    // get flash size id
    flash_size_id = ((flash_get_deviceinfo() >> FLASH_SIZE_ID_SHIFT) & 0xFF);

    if ((flash_address < BOOT_LOADER_END_PROTECT_ADDR) || ((flash_address + (length - 1)) >= FLASH_END_ADDR(flash_size_id)))
    {
        return STATUS_INVALID_PARAM;
    }

#endif
    return STATUS_SUCCESS;
}
uint32_t flash_check_busy()
{
    return ((FLASH->START) & BUSYBIT) ? 1 : 0;
}
/*
 * flash_read_page :
 *   read one page. One page is 256 bytes, so buf_addr should have 256 bytes available.
 *   this is non_block mode... so user should wait flash finish read outside this function.
 */
uint32_t flash_read_page(uint32_t buf_addr, uint32_t read_page_addr)
{

#if (RAFAEL_BOARD == RT582)
    if (read_page_addr >= 0x100000)
        read_page_addr ^= 0x100000;
#endif
    if (flash_check_address(read_page_addr, LENGTH_PAGE) == STATUS_INVALID_PARAM)
    {
        return STATUS_INVALID_PARAM; // invalid addres range
    }

    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_READPAGE;
    FLASH->FLASH_ADDR = read_page_addr;
    FLASH->MEM_ADDR   = buf_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    return STATUS_SUCCESS; /*remember to wait flash to finish read outside the caller*/
}

/*
 * flash_read_page_syncmode :
 *   read one page. One page is 256 bytes, so buf_addr should have 256 bytes available.
 *   This is block mode. when user call this function, system will wait all data in buf_addr
 *   the return.
 */
uint32_t flash_read_page_syncmode(uint32_t buf_addr, uint32_t read_page_addr)
{
#if (RAFAEL_BOARD == RT582)
    if (read_page_addr >= 0x100000)
        read_page_addr ^= 0x100000;
#endif

    if (flash_check_address(read_page_addr, LENGTH_PAGE) == STATUS_INVALID_PARAM)
    {
        return STATUS_INVALID_PARAM; // invalid addres range
    }

    if (flash_check_busy())
    {
        return STATUS_EBUSY; /*flash busy.. please call this function again*/
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_READPAGE;
    FLASH->FLASH_ADDR = read_page_addr;
    FLASH->MEM_ADDR   = buf_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    while (flash_check_busy())
    {
        ;
    }

    return STATUS_SUCCESS; /*all data in register buffer now*/
}

uint8_t flash_read_byte(uint32_t read_addr)
{
#if (RAFAEL_BOARD == RT582)
    if (read_addr >= 0x100000)
        read_addr ^= 0x100000;
#endif

    /*this is not a good idea to block function here....*/
    while (flash_check_busy())
    {
        ;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_READBYTE;
    FLASH->FLASH_ADDR = read_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    while (flash_check_busy())
    {
        ;
    }

    return FLASH->FLASH_DATA >> 8;
}

uint32_t flash_read_byte_check_addr(uint32_t * buf_addr, uint32_t read_addr)
{
#if (RAFAEL_BOARD == RT582)
    if (read_addr >= 0x100000)
        read_addr ^= 0x100000;
#endif

    if (flash_check_address(read_addr, LENGTH_BYTE) == STATUS_INVALID_PARAM)
    {
        return STATUS_INVALID_PARAM; // invalid addres range
    }

    if (flash_check_busy())
    {
        return STATUS_EBUSY; /*flash busy.. please call this function again*/
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_READBYTE;
    FLASH->FLASH_ADDR = read_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    while (flash_check_busy())
    {
        ;
    }

    *buf_addr = (FLASH->FLASH_DATA >> 8) & 0xFF;

    return STATUS_SUCCESS;
}

uint32_t flash_erase(flash_erase_mode_t mode, uint32_t flash_addr)
{
    if (mode > FLASH_ERASE_SECURE)
    {
        return STATUS_INVALID_PARAM;
    }

#if (RAFAEL_BOARD == RT582)
    if (flash_addr >= 0x100000)
        flash_addr ^= 0x100000;
#endif
    /* For Safety reason, we don't implement
     * erase chip command here. */
    switch (mode)
    {
    case FLASH_ERASE_PAGE: {
        if ((flash_get_deviceinfo() & 0xFF) != PUYA_MANU_ID)
        {
            return STATUS_INVALID_PARAM; // invalid flash id
        }

        if (flash_check_address(flash_addr, LENGTH_PAGE) == STATUS_INVALID_PARAM)
        {
            return STATUS_INVALID_PARAM; // invalid addres range
        }

        FLASH->COMMAND = CMD_ERASEPAGE;
        break;
    }
    case FLASH_ERASE_SECTOR: {
        if (flash_check_address(flash_addr, LENGTH_4KB) == STATUS_INVALID_PARAM)
        {
            return STATUS_INVALID_PARAM; // invalid addres range
        }

        FLASH->COMMAND = CMD_ERASESECTOR;
        break;
    }
    case FLASH_ERASE_32K: {
        if (flash_check_address(flash_addr, LENGTH_32KB) == STATUS_INVALID_PARAM)
        {
            return STATUS_INVALID_PARAM; // invalid addres range
        }

        FLASH->COMMAND = CMD_ERASE_BL32K;
        break;
    }
    case FLASH_ERASE_64K: {
        if (flash_check_address(flash_addr, LENGTH_64KB) == STATUS_INVALID_PARAM)
        {
            return STATUS_INVALID_PARAM; // invalid addres range
        }

        FLASH->COMMAND = CMD_ERASE_BL64K;
        break;
    }
    case FLASH_ERASE_SECURE: {
        /*This is special command for erase secure register*/
        FLASH->COMMAND = CMD_ERASE_SEC_PAGE;
        break;
    }
    default:
        return STATUS_INVALID_PARAM;
    }

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    enter_critical_section();

    FLASH->FLASH_ADDR = flash_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    while (flash_check_busy())
    {
        ;
    }

    leave_critical_section();

    return STATUS_SUCCESS;
}

void flash_set_timing(flash_timing_mode_t * timing_cfg)
{
    FLASH->DPD     = timing_cfg->deep_pd_timing;
    FLASH->RDPD    = timing_cfg->deep_rpd_timing;
    FLASH->SUSPEND = timing_cfg->suspend_timing;
    FLASH->RESUME  = timing_cfg->resume_timing;
    return;
}

uint32_t flash_write_page(uint32_t buf_addr, uint32_t write_page_addr)
{
#if (RAFAEL_BOARD == RT582)
    if (write_page_addr >= 0x100000)
        write_page_addr ^= 0x100000;
#endif

    if (flash_check_address(write_page_addr, LENGTH_PAGE) == STATUS_INVALID_PARAM)
    {
        return STATUS_INVALID_PARAM; // invalid addres range
    }

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_WRITEPAGE;
    FLASH->FLASH_ADDR = write_page_addr;
    FLASH->MEM_ADDR   = buf_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    while (flash_check_busy())
    {
        ;
    }

    leave_critical_section();

    return STATUS_SUCCESS;
}

uint32_t flash_verify_page(uint32_t read_page_addr)
{
#if (RAFAEL_BOARD == RT582)
    if (read_page_addr >= 0x100000)
        read_page_addr ^= 0x100000;
#endif

    if (flash_check_address(read_page_addr, LENGTH_PAGE) == STATUS_INVALID_PARAM)
    {
        return STATUS_INVALID_PARAM; // invalid addres range
    }

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_READVERIFY;
    FLASH->FLASH_ADDR = read_page_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    return STATUS_SUCCESS;
}

uint32_t flash_write_byte(uint32_t write_addr, uint8_t singlebyte)
{
#if (RAFAEL_BOARD == RT582)
    if (write_addr >= 0x100000)
        write_addr ^= 0x100000;
#endif

    if (flash_check_address(write_addr, LENGTH_BYTE) == STATUS_INVALID_PARAM)
    {
        return STATUS_INVALID_PARAM; // invalid addres range
    }

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_WRITEBYTE;
    FLASH->FLASH_ADDR = write_addr;
    FLASH->FLASH_DATA = singlebyte;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    return STATUS_SUCCESS;
}

/*get Flash status register*/
uint32_t flash_get_status_reg(flash_status_t * status)
{
    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    if ((status->require_mode) & FLASH_STATUS_RW1)
    {
        enter_critical_section();

        FLASH->COMMAND = CMD_READ_STATUS1;
        FLASH->PATTERN = FLASH_UNLOCK_PATTER;
        FLASH->START   = STARTBIT;

        leave_critical_section();

        /*this check_busy is very short... it just send command then to receive data*/
        while (flash_check_busy())
        {
            ;
        }
        status->status1 = (uint8_t) ((FLASH->FLASH_DATA) >> 8);
    }

    if (status->require_mode & FLASH_STATUS_RW2)
    {
        enter_critical_section();

        FLASH->COMMAND = CMD_READ_STATUS2;
        FLASH->PATTERN = FLASH_UNLOCK_PATTER;
        FLASH->START   = STARTBIT;

        leave_critical_section();

        while (flash_check_busy())
        {
            ;
        }
        status->status2 = (uint8_t) ((FLASH->FLASH_DATA) >> 8);
    }

    /*2022/01/18: GD does NOT have status bytes3.*/

    return STATUS_SUCCESS;
}

/*set Flash status register*/
uint32_t flash_set_status_reg(const flash_status_t * status)
{

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }
    /*
     * 2022/01/18: GD only have status bytes1 and bytes2.
     * GD only support command 0x01. So if you want to write
     *
     */
    if (status->require_mode == FLASH_STATUS_RW1_2)
    {
        enter_critical_section();
        /*GD write status2 must two bytes */
        FLASH->COMMAND = CMD_WRITE_STATUS;
        FLASH->STATUS  = (uint32_t) (status->status1) | (uint32_t) ((status->status2) << 8);
        FLASH->PATTERN = FLASH_UNLOCK_PATTER;
        FLASH->START   = STARTBIT;

        leave_critical_section();

        while (flash_check_busy())
        {
            ;
        }
    }
    else if (status->require_mode == FLASH_STATUS_RW1)
    {
        enter_critical_section();

        FLASH->COMMAND = CMD_WRITE_STATUS1;
        FLASH->STATUS  = (status->status1);
        FLASH->PATTERN = FLASH_UNLOCK_PATTER;
        FLASH->START   = STARTBIT;

        leave_critical_section();

        while (flash_check_busy())
        {
            ;
        }
    }

    return STATUS_SUCCESS;
}

/* program secure page data
 * Note: write_page_addr must be alignment
 */
uint32_t flash_write_sec_register(uint32_t buf_addr, uint32_t write_reg_addr)
{
    uint32_t addr;
    /*first we should check write_reg_addr*/
    addr = write_reg_addr >> 12;

    if ((addr > 3) || (write_reg_addr & 0xFF))
    {
        /*only support 3 secureity register.*/
        /*We need secure register write to be 256 bytes alignment*/
        return STATUS_INVALID_PARAM;
    }

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_WRITE_SEC_PAGE;
    FLASH->FLASH_ADDR = write_reg_addr;
    FLASH->MEM_ADDR   = buf_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    return STATUS_SUCCESS;
}

/* read secure register data.
 * Note: read_page_addr must be alignment
 */
uint32_t flash_read_sec_register(uint32_t buf_addr, uint32_t read_reg_addr)
{
    uint32_t addr;
    /*first we should check read_reg_addr*/
    addr = read_reg_addr >> 12;

    /*2022/04/28 add, Device busy. try again.*/
    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    // if((addr>3)|| (read_reg_addr & 0xFF)) {
    if (addr > 3)
    {
        /*We need secure register read to be 256 bytes alignment*/
        return STATUS_INVALID_PARAM;
    }

    enter_critical_section();

    FLASH->COMMAND    = CMD_READ_SEC_PAGE;
    FLASH->FLASH_ADDR = read_reg_addr;
    FLASH->MEM_ADDR   = buf_addr;
    FLASH->PATTERN    = FLASH_UNLOCK_PATTER;
    FLASH->START      = STARTBIT;

    leave_critical_section();

    while (flash_check_busy())
    {
        ;
    }

    return STATUS_SUCCESS;
}

/* read flash unique ID
 *  flash ID is 128bits/16 bytes.
 *  if buf_length <16, it will return required length data only.
 *  if buf_length >16, it will return 16 bytes only.
 *  if buf_length = 0 , this function will return STATUS_INVALID_PARAM
 *
 */
uint32_t flash_get_unique_id(uint32_t flash_id_buf_addr, uint32_t buf_length)
{
    uint32_t i;
    uint8_t temp[16], *ptr;

    if (flash_check_busy())
    {
        return STATUS_EBUSY;
    }

    /*
     * Notice: we don't check flash_id_buf_addr value here..
     * it should be correct address in SRAM!
     */
    if (buf_length == 0)
    {
        return STATUS_INVALID_PARAM;
    }
    else if (buf_length > 16)
    {
        buf_length = 16;
    }

    enter_critical_section();

    FLASH->COMMAND        = CMD_READUID;
    FLASH->PAGE_READ_WORD = 0xF;
    FLASH->MEM_ADDR       = (uint32_t) temp;
    FLASH->PATTERN        = FLASH_UNLOCK_PATTER;
    FLASH->START          = STARTBIT;

    leave_critical_section();

    ptr = (uint8_t *) flash_id_buf_addr; /*set address*/

    while (flash_check_busy())
    {
        ;
    }

    FLASH->PAGE_READ_WORD = 0xFF; /*restore read one page length by default*/

    /*move unique number from stack to assign buffer*/
    for (i = 0; i < buf_length; i++)
    {
        ptr[i] = temp[i];
    }

    return STATUS_SUCCESS;
}

void flash_timing_init(void)
{
    uint32_t clk_mode, sys_clk;
    uint16_t tdp, tres, tsus, trs, flash_type_id;

    flash_timing_mode_t flash_timing;
    /*change AHB clock also need change flash timing.*/
    flash_type_id = flash_get_deviceinfo() & FLASH_TYPE_ID_MAKS;

    clk_mode = Get_Ahb_System_Clk();

    /*check flash type to adjust flash timing*/
    if (flash_type_id == GDWQ_ID)
    {
        tdp  = GDWQ_FLASH_TDP;
        tres = GDWQ_FLASH_TRES1;
        tsus = GDWQ_FLASH_TSUS;
        trs  = GDWQ_FLASH_TRS;
    }

    if (flash_type_id == GDLQ_ID)
    {
        tdp  = GDLQ_FLASH_TDP;
        tres = GDLQ_FLASH_TRES1;
        tsus = GDLQ_FLASH_TSUS;
        trs  = GDLQ_FLASH_TRS;
    }

    if (flash_type_id == PUYA_ID)
    {
        tdp  = PUYA_FLASH_TDP;
        tres = PUYA_FLASH_TRES1;
        tsus = PUYA_FLASH_TSUS;
        trs  = PUYA_FLASH_TRS;
    }
    //(0+2)*16=32
    //(1+2)*16=48
    //(2+2)*16=64
    sys_clk = (clk_mode + 2) << 4;

    flash_timing.deep_pd_timing  = tdp * sys_clk + 2;
    flash_timing.deep_rpd_timing = tres * sys_clk + 2;
    flash_timing.suspend_timing  = tsus * sys_clk + 2;
    flash_timing.resume_timing   = trs * sys_clk + 2;

    flash_set_timing(&flash_timing);
}

uint32_t flash_read_otp_sec_page(uint32_t buf_addr)
{
    switch (flash_get_deviceinfo()) // check flash device
    {
    case RT581_FLASH_TYPE:          // 0x2000
    case RT582P512_FLASH_TYPE:      // 0x2000
        if (flash_read_sec_register((uint32_t) buf_addr, FLASH_SECREG_R2_P0))
        {
            return STATUS_INVALID_PARAM;
        }
        break;
    case RT582_FLASH_TYPE:    // 0x0000
    case RT582_2M_FLASH_TYPE: // 0x0000
        if (flash_read_sec_register((uint32_t) buf_addr, FLASH_SECREG_R0_P0))
        {
            return STATUS_INVALID_PARAM;
        }
    }

    return STATUS_SUCCESS;
}
