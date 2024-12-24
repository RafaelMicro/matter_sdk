/**************************************************************************//**
 * @file     flashds.c
 * @version
 * @brief
 *
 * @copyright
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "cm3_mcu.h"
#include "mp_sector.h"

static volatile ds_t                                    ds_record_table[DS_TABLE_TYPE_MAX];
static volatile ds_info_t                                   dsinfo;
static uint32_t ds_end_address = 0;

uint32_t ds_initinal(ds_config_t ds_init)
{
    uint32_t page_size = 0, status;

    status = ds_load_page_end_address_max();

    if (status != STATUS_SUCCESS)
    {
        dsinfo.invaild = DS_INVAILD;
        return status;
    }

    if ((ds_init.end_address > ds_end_address) || (ds_init.end_address < ds_init.start_address))
    {
        dsinfo.invaild = DS_INVAILD;
        return STATUS_INVALID_PARAM;
    }

    dsinfo.page_number = (ds_init.end_address - ds_init.start_address) / DATA_PAGE_SIZE_4K;

    if ((dsinfo.page_number == DS_PAGE_0) || (dsinfo.page_number % 2) != DS_PAGE_0)
    {
        dsinfo.invaild = DS_INVAILD;
        return STATUS_INVALID_PARAM;
    }

    page_size = (ds_init.end_address - ds_init.start_address) >> 1;
    dsinfo.start_address = ds_init.start_address;
    dsinfo.end_address = ds_init.end_address;
    dsinfo.middle_address = ds_init.start_address + page_size;

    ds_pagex_check();

    return STATUS_SUCCESS;
}


uint32_t ds_pagex_check()
{

    uint32_t page1_satatus, page2_satatus, satatus = STATUS_SUCCESS;
    uint32_t ds_page1_addr = dsinfo.start_address;
    uint32_t ds_page2_addr = dsinfo.middle_address;
    uint32_t ds_page1_offset = 0;
    uint32_t ds_page2_offset = 0;

    uint32_t ds_page1_sn, ds_page2_sn;
    ds_search_t ds_search_pagex;

    ds_search_pagex.address = ds_page1_addr;
    ds_search_pagex.end_address = dsinfo.middle_address;
    ds_search_pagex.offset = DS_START_OFFSET;
    ds_search_pagex.type = DS_VAILD_TYPE;
    ds_search_pagex.flag = DS_VAILD_TYPE_SEARCH;

    page1_satatus = ds_erase_check(&ds_search_pagex);

    ds_page1_addr = ds_search_pagex.address;
    ds_page1_offset = ds_search_pagex.offset;

    ds_search_pagex.address = ds_page2_addr;
    ds_search_pagex.end_address = dsinfo.end_address;

    page2_satatus = ds_erase_check(&ds_search_pagex);

    ds_page2_addr = ds_search_pagex.address;
    ds_page2_offset = ds_search_pagex.offset;

    if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus == STATUS_SUCCESS))                 //Page1 and Page 2 All 0xFF
    {
        dsinfo.current_page = DS_PAGE_0;
        dsinfo.current_offset = DS_START_OFFSET;
        dsinfo.current_address = ds_page1_addr;
        dsinfo.current_sn = DS_SERAIL_NUMBER_START;
    }
    else if ((page1_satatus != STATUS_SUCCESS) && (page2_satatus == STATUS_SUCCESS))            //Page2 All 0xFF,Page1 has record data
    {

        satatus = ds_page_vaild_address(ds_page1_addr, ds_page1_offset);                                        //check page 1 data
        dsinfo.current_sn += 1;

        if (satatus != STATUS_SUCCESS)                                                          //Page1 invaild data
        {
            dsinfo.current_page = DS_PAGE_1;
            dsinfo.current_offset = DS_START_OFFSET;
            dsinfo.current_address = ds_page2_addr;
            ds_migration(DS_PAGE_0);
            ds_migration_erase(DS_PAGE_0);
        }
    }
    else if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus != STATUS_SUCCESS))             //Page1 All 0xFF, Page2 has record data
    {
        satatus = ds_page_vaild_address(ds_page2_addr, ds_page2_offset);                                          //check page 1 data
        dsinfo.current_sn += 1;

        if (satatus != STATUS_SUCCESS)                                                           //Page2 invaild data
        {
            dsinfo.current_page = DS_PAGE_0;
            dsinfo.current_offset = DS_START_OFFSET;
            dsinfo.current_address = ds_page1_addr;
            ds_migration(DS_PAGE_1);
            ds_migration_erase(DS_PAGE_1);
        }
    }
    else
    {
        page1_satatus = ds_page_vaild_address(ds_page1_addr, ds_page1_offset);
        ds_page1_sn = dsinfo.current_sn;

        page2_satatus = ds_page_vaild_address(ds_page2_addr, ds_page2_offset);
        ds_page2_sn = dsinfo.current_sn;

        dsinfo.current_sn += 1;

        if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus == STATUS_SUCCESS))             //All page data is okay
        {
            if (ds_page1_sn > ds_page2_sn)
            {
                ds_page_vaild_address(ds_page1_addr, ds_page1_offset);
                ds_migration_erase(DS_PAGE_1);
                ds_migration(DS_PAGE_0);
                ds_migration_erase(DS_PAGE_0);
                ds_page_vaild_address(ds_page2_addr, ds_page2_offset);
            }
            else
            {
                ds_page_vaild_address(ds_page2_addr, ds_page2_offset);
                ds_migration_erase(DS_PAGE_0);
                ds_migration(DS_PAGE_1);
                ds_migration_erase(DS_PAGE_1);
                ds_page_vaild_address(ds_page1_addr, ds_page1_offset);
            }
        }
        else if ((page1_satatus == STATUS_INVALID_PARAM) && (page2_satatus == STATUS_SUCCESS))
        {
            ds_page_vaild_address(ds_page2_addr, ds_page2_offset);
            ds_migration_erase(DS_PAGE_0);
            dsinfo.current_page = DS_PAGE_1;
        }
        else if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus == STATUS_INVALID_PARAM))
        {
            ds_page_vaild_address(ds_page1_addr, ds_page1_offset);
            ds_migration_erase(DS_PAGE_1);
            dsinfo.current_page = DS_PAGE_0;
        }
        else if ((page1_satatus == STATUS_INVALID_PARAM) && (page2_satatus == STATUS_INVALID_PARAM))
        {
            ds_reset_to_default();
        }
    }


    return STATUS_SUCCESS;
}


uint32_t ds_erase_check(ds_search_t *ds_search)
{
    uint8_t dsbufP1[DS_DATA_CHECK_SIZE];
    uint8_t dsbufP2[DS_DATA_CHECK_SIZE];
    uint32_t i = 0, j = 0;
    uint32_t end_address = 0, start_address = 0;

    end_address = ds_search->end_address;
    start_address = ds_search->address;

    memset(dsbufP2, DS_VAILD_TYPE_FF, sizeof(DS_DATA_CHECK_SIZE));

    for (i = start_address; i < end_address; i += DS_DATA_CHECK_SIZE)
    {
        flash_read_page_syncmode((uint32_t)dsbufP1, i);

        if (memcmp(dsbufP1, dsbufP2, sizeof(DS_DATA_CHECK_SIZE)) != 0)
        {
            for (j = 0; j < DS_DATA_CHECK_SIZE; j++)
            {
                if (dsbufP1[j] != DS_VAILD_TYPE_FF)
                {
                    ds_search->offset = j;
                    ds_search->address = i;
                    return STATUS_ERROR;
                }
            }
        }
    }

    return STATUS_SUCCESS;
}


uint32_t ds_page_vaild_address(uint32_t ds_start_address, uint32_t ds_start_offset)
{
    uint32_t ds_status = 0;
    ds_search_t ds_vaild_search;

    ds_vaild_search.address = ds_start_address;
    ds_vaild_search.offset = ds_start_offset;
    ds_vaild_search.type = DS_VAILD_TYPE;
    ds_vaild_search.flag = DS_VAILD_TYPE_SEARCH;

    if (ds_start_address >= dsinfo.middle_address)
    {
        dsinfo.current_page  = DS_PAGE_1;
        ds_vaild_search.end_address = dsinfo.end_address;
    }
    else
    {
        dsinfo.current_page  = DS_PAGE_0;
        ds_vaild_search.end_address = dsinfo.middle_address;
    }

    ds_status = ds_vaild_address_search(&ds_vaild_search);

    return ds_status;
}


uint32_t ds_vaild_address_search(ds_search_t *dssearch)
{
    uint32_t    ds_status = STATUS_INVALID_PARAM;
    uint32_t    address = 0, end_address = dssearch->end_address;
    ds_t        dspack;

    do
    {
        dspack = (*(ds_t *)(dssearch->address + dssearch->offset));

        if (address > end_address)
        {
            ds_status = STATUS_INVALID_PARAM;
            break;
        }

        ds_status = ds_vaild(dssearch, &dspack);

        dssearch->offset += DS_HEADER_OFFSET + dspack.len + DS_TAIL_OFFSET;

        address = (dssearch->address + dssearch->offset);

    } while (ds_status != STATUS_SUCCESS);

    return ds_status;
}



uint32_t ds_vaild(ds_search_t *dssearch, ds_t *ds_vaild)
{
    static uint8_t              crc_result = 0, crc = 0;
    uint16_t                    magic = 0;
    uint32_t                    address = 0;
    ds_header_t                 dspack_header;
    ds_tail_t                   dspack_tail;
    ds_t                                ds_table;
    ds_rw_t dscrc;

    dspack_header = (*(ds_header_t *)(dssearch->address + dssearch->offset));

    if ((dspack_header.type != DS_VAILD_TYPE) && (dspack_header.type != DS_INVAILD_TYPE_00))
    {

        address = (dssearch->address + dssearch->offset);

        ds_vaild->type = dspack_header.type;
        ds_vaild->len = dspack_header.len;
        ds_vaild->sn = dspack_header.sn;

        address += DS_HEADER_OFFSET;
        ds_vaild->buf_addr = address;

        address += dspack_header.len;
        dspack_tail = *(ds_tail_t *)(address);

        crc = dspack_tail.crc;
        magic = dspack_tail.magic;

        dscrc.type = ds_vaild->type;
        dscrc.address = ds_vaild->buf_addr;
        dscrc.len = ds_vaild->len;

        crc_result = ds_cal_crc(&dscrc, dspack_header.sn);

        if (dssearch->type != DS_INVAILD_TYPE_00)
        {
            if (crc_result != crc)
            {
                return STATUS_INVALID_PARAM;
            }

            ds_vaild->crc = crc;
            ds_vaild->magic = magic;
        }

        if (dssearch->flag == DS_VAILD_TYPE_SEARCH)
        {
            ds_update_type(dspack_header.type);
            ds_update_crrent_sn(dspack_header.sn);
        }

        memcpy(&ds_table, ds_vaild, sizeof(ds_t));

        ds_update_table(ds_table);

    }
    else if (dspack_header.type == DS_VAILD_TYPE)
    {
        dsinfo.current_address = dssearch->address + dssearch->offset;
        dsinfo.current_offset = dssearch->offset;

        return STATUS_SUCCESS;
    }
    else if ((dspack_header.type == DS_VAILD_TYPE) || (dspack_header.type == DS_INVAILD_TYPE_00))
    {
        address = (dssearch->address + dssearch->offset);

        ds_vaild->type = dspack_header.type;
        ds_vaild->len = dspack_header.len;
        ds_vaild->sn = dspack_header.sn;

        address += DS_HEADER_OFFSET;
        ds_vaild->buf_addr = address;

        address += dspack_header.len;
        dspack_tail = *(ds_tail_t *)(address);

        crc = dspack_tail.crc;
        magic = dspack_tail.magic;

        dscrc.type = ds_vaild->type;
        dscrc.address = ds_vaild->buf_addr;
        dscrc.len = ds_vaild->len;

        crc_result = ds_cal_crc(&dscrc, dspack_header.sn);

        if (dssearch->type != DS_INVAILD_TYPE_00)
        {
            if (crc_result != crc)
            {
                return STATUS_INVALID_PARAM;
            }

            ds_vaild->crc = crc;
            ds_vaild->magic = magic;
        }

        if (dssearch->flag == DS_VAILD_TYPE_SEARCH)
        {
            ds_update_type(dspack_header.type);
            ds_update_crrent_sn(dspack_header.sn);
        }

        memcpy(&ds_table, ds_vaild, sizeof(ds_t));

        ds_update_table(ds_table);
    }

    return STATUS_INVALID_REQUEST;
}


uint32_t ds_get_current_page()
{
    return  dsinfo.current_page;
}


uint32_t ds_set_current_page(uint8_t set_page)
{

    if ((set_page > dsinfo.page_number))
    {
        return STATUS_INVALID_PARAM;
    }

    dsinfo.current_page = set_page;

    if (ds_get_current_page() == DS_PAGE_0)
    {
        dsinfo.current_address = dsinfo.start_address;
    }
    else
    {
        dsinfo.current_address = dsinfo.middle_address;
    }

    return STATUS_SUCCESS;
}

uint32_t ds_get_current_page_offset()
{
    return dsinfo.current_offset;
}

uint32_t ds_set_current_page_offset(uint32_t set_page_offset)
{
    uint32_t dsaddress = 0, endaddress = 0;

    if (ds_get_current_page() == DS_PAGE_0)
    {
        dsaddress = dsinfo.current_address + set_page_offset;
        endaddress = dsinfo.middle_address;
    }
    else
    {
        dsaddress = dsinfo.current_address + set_page_offset;
        endaddress = dsinfo.end_address;
    }

    if ((dsaddress > endaddress))
    {
        return STATUS_INVALID_PARAM;
    }

    dsinfo.current_address += set_page_offset;

    return STATUS_SUCCESS;
}

uint32_t ds_read(ds_rw_t *ds_read)
{
    uint32_t        ds_status = STATUS_INVALID_PARAM;
    uint8_t         index = (ds_read->type - 1);
    uint16_t        ds_table_type;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    ds_table_type = ds_record_table[index].type;

    if ((ds_table_type > DS_INVAILD_TYPE_00) && (ds_table_type < DS_INVAILD_TYPE_FF))
    {
        ds_read->address = ds_record_table[index].buf_addr;
        ds_read->len = ds_record_table[index].len;

        ds_status = STATUS_SUCCESS;
    }
    else
    {
        ds_read->address = 0;
        ds_read->len = 0;
    }

    return ds_status;
}


uint32_t ds_write(ds_rw_t *ds_write)
{

    uint8_t     crc_result = 0, over_page_flag = 0;
    uint32_t    i = 0, j = 0, wr_buf_index = 0, value = 0, addr_mod = 0;
    uint32_t    dswraddress = 0, dsmaxaddress = 0, address = ds_write->address;
    uint16_t    magic_number = 0, len = ds_write->len, type = ds_write->type;
    ds_t        ds_table_update;
    uint8_t     wr_buf[256];


    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if ((type == DS_INVAILD_TYPE_00) || (type == DS_INVAILD_TYPE_FF))
    {
        return STATUS_INVALID_REQUEST;
    }

    if (ds_get_current_page() == DS_PAGE_0)
    {
        dsmaxaddress =  dsinfo.middle_address;
    }
    else
    {
        dsmaxaddress =  dsinfo.end_address;
    }

    dswraddress = dsinfo.current_address  + DS_HEADER_OFFSET + len + DS_TAIL_OFFSET;


    if ((dswraddress > dsmaxaddress))
    {

        if (ds_get_current_page() == DS_PAGE_1)
        {
            ds_migration_erase(DS_PAGE_0);
            ds_migration(DS_PAGE_1);
            ds_migration_erase(DS_PAGE_1);
            dsinfo.current_page = DS_PAGE_0;
            dsmaxaddress =  dsinfo.middle_address;
            dswraddress = dsinfo.migration_address + DS_HEADER_OFFSET + len + DS_TAIL_OFFSET;
            dsinfo.current_address = dsinfo.migration_address;
        }
        else
        {
            ds_migration_erase(DS_PAGE_1);
            ds_migration(DS_PAGE_0);
            ds_migration_erase(DS_PAGE_0);
            dsinfo.current_page = DS_PAGE_1;
            dsmaxaddress = dsinfo.end_address;
            dswraddress = dsinfo.migration_address + DS_HEADER_OFFSET + len + DS_TAIL_OFFSET;
            dsinfo.current_address = dsinfo.migration_address;
        }

        if ((dswraddress > dsmaxaddress))
        {
            return STATUS_INVALID_REQUEST;
        }
    }


    dswraddress = dsinfo.current_address;

    addr_mod =  dswraddress % DS_DATA_CHECK_SIZE;

    ds_update_type(type);

    flash_disable_suspend();

    if (dsinfo.current_sn == DS_SERAIL_NUMBER_MAX)
    {
        dsinfo.current_sn = DS_SERAIL_NUMBER_START;
    }

    memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);

    wr_buf[0] = type;
    wr_buf[1] = (type >> 8);
    wr_buf[2] = len;
    wr_buf[3] = (len >> 8);
    wr_buf[4] = (dsinfo.current_sn);
    wr_buf[5] = (dsinfo.current_sn >> 8);
    wr_buf[6] = (dsinfo.current_sn >> 16);
    wr_buf[7] = (dsinfo.current_sn >> 24);

    wr_buf_index = 8;

    for (i = 0; i < len; i++)
    {
        value = (*(uint32_t *)(address + i));
        wr_buf[wr_buf_index] = value;
        wr_buf_index += 1;

        if (wr_buf_index == DS_DATA_CHECK_SIZE)
        {
            over_page_flag = 1;

            if ((addr_mod < DS_DATA_CHECK_SIZE) && (addr_mod != 0x00))
            {
                for (j = 0; j < (DS_DATA_CHECK_SIZE - addr_mod); j++)
                {
                    flash_write_byte((dswraddress + j), wr_buf[j]);
                    while (flash_check_busy()) {;}
                }

                memmove(wr_buf, (wr_buf + j), (DS_DATA_CHECK_SIZE - j));
                memset((wr_buf + (DS_DATA_CHECK_SIZE - j)), DS_VAILD_TYPE_FF, j);

                wr_buf_index = DS_DATA_CHECK_SIZE - j;
                dswraddress += j;
                addr_mod = dswraddress % DS_DATA_CHECK_SIZE;
            }
            else
            {
                flash_write_page((uint32_t)wr_buf, dswraddress);
                dswraddress += DS_DATA_CHECK_SIZE;
                wr_buf_index = 0;
                flush_cache();
                memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);
            }
        }
    }

    crc_result = ds_cal_crc(ds_write, dsinfo.current_sn);
    wr_buf[wr_buf_index] = crc_result;
    wr_buf_index += 1;

    if (wr_buf_index <= 0xFD)
    {
        magic_number = DS_MAGIC_NUMBER;
        wr_buf[wr_buf_index] =   magic_number;
        wr_buf_index += 1;

        wr_buf[wr_buf_index] =   magic_number >> 8;
        wr_buf_index += 1;
    }
    else if (wr_buf_index == 0xFE)
    {

        magic_number = DS_MAGIC_NUMBER;
        wr_buf[wr_buf_index] =   magic_number;
        wr_buf_index += 1;

        flash_write_page((uint32_t)wr_buf, dswraddress);
        dswraddress += wr_buf_index;
        wr_buf_index = 0;

        memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);

        wr_buf[wr_buf_index] =   magic_number >> 8;
        wr_buf_index += 1;
    }
    else if (wr_buf_index == 0xFF)
    {

        flash_write_page((uint32_t)wr_buf, dswraddress);
        dswraddress += wr_buf_index;
        wr_buf_index = 0;

        memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);

        magic_number = DS_MAGIC_NUMBER;
        wr_buf[wr_buf_index] =    magic_number;
        wr_buf_index += 1;

        wr_buf[wr_buf_index] =    magic_number >> 8;
        wr_buf_index += 1;

    }

    if (over_page_flag == 1)
    {
        flash_write_page((uint32_t)wr_buf, dswraddress);
        dswraddress += wr_buf_index;
    }
    else
    {
        for (j = 0; j < wr_buf_index; j++)
        {
            flash_write_byte((dswraddress + j), wr_buf[j]);
            while (flash_check_busy()) {;}
        }

        dswraddress += wr_buf_index;
    }

    flash_enable_suspend();

    ds_table_update.type = type;
    ds_table_update.len = len;
    ds_table_update.sn = dsinfo.current_sn;
    ds_table_update.buf_addr = (dsinfo.current_address + DS_HEADER_OFFSET);
    ds_table_update.crc = crc_result;
    ds_table_update.magic = magic_number;

    ds_update_table(ds_table_update);

    dsinfo.current_address = dswraddress;
    dsinfo.current_offset = dswraddress;
    dsinfo.current_sn += 1;

    flush_cache();



    return STATUS_SUCCESS;

}

uint32_t ds_migration(ds_page_t page)
{
    uint32_t        i = 0;
    ds_rw_t         dswr;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if (page == DS_PAGE_0)
    {
        dsinfo.migration_address = dsinfo.middle_address;
    }
    else
    {
        dsinfo.migration_address = dsinfo.start_address;
    }

    for (i = 0; i < DS_TABLE_TYPE_MAX; i++)
    {
        if ((ds_record_table[i].type != DS_INVAILD_TYPE_00))
        {
            dswr.type = ds_record_table[i].type;
            dswr.len = ds_record_table[i].len;
            dswr.address = ds_record_table[i].buf_addr;
            ds_migration_write(page, &dswr);
        }
    }

    return STATUS_SUCCESS;
}
uint32_t ds_migration_write(ds_page_t page, ds_rw_t *ds_write)
{
    uint32_t    i = 0, j = 0, over_page_flag = 0;
    uint32_t    dswraddress = 0, address = ds_write->address, wr_buf_index = 0, addr_mod = 0;
    uint8_t     value = 0, crc_result = 0;
    uint16_t    magic_number, len = ds_write->len, type = ds_write->type;
    ds_t        ds_table;
    uint8_t     wr_buf[256];

    dswraddress = dsinfo.migration_address;
    dsinfo.current_sn += 1;

    flash_disable_suspend();

    addr_mod = dswraddress % DS_DATA_CHECK_SIZE;

    memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);

    wr_buf[0] = type;
    wr_buf[1] = (type >> 8);
    wr_buf[2] = len;
    wr_buf[3] = (len >> 8);
    wr_buf[4] = (dsinfo.current_sn);
    wr_buf[5] = (dsinfo.current_sn >> 8);
    wr_buf[6] = (dsinfo.current_sn >> 16);
    wr_buf[7] = (dsinfo.current_sn >> 24);

    wr_buf_index = 8;

    for (i = 0; i < len; i++)
    {
        value = (*(uint32_t *)(address + i));
        wr_buf[wr_buf_index] = value;
        wr_buf_index += 1;

        if (wr_buf_index == DS_DATA_CHECK_SIZE)
        {
            over_page_flag = 1;
            if ((addr_mod < DS_DATA_CHECK_SIZE) && (addr_mod != 0x00))
            {
                for (j = 0; j < (DS_DATA_CHECK_SIZE - addr_mod); j++)
                {
                    flash_write_byte((dswraddress + j), wr_buf[j]);
                    while (flash_check_busy()) {;}
                }

                memmove(wr_buf, (wr_buf + j), (DS_DATA_CHECK_SIZE - j));
                memset((wr_buf + (DS_DATA_CHECK_SIZE - j)), DS_VAILD_TYPE_FF, j);

                wr_buf_index = DS_DATA_CHECK_SIZE - j;
                dswraddress += j;
                addr_mod = dswraddress % DS_DATA_CHECK_SIZE;
            }
            else
            {
                flash_write_page((uint32_t)wr_buf, dswraddress);
                dswraddress += DS_DATA_CHECK_SIZE;
                wr_buf_index = 0;
                flush_cache();
                memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);
            }
        }
    }

    crc_result = ds_cal_crc(ds_write, dsinfo.current_sn);
    wr_buf[wr_buf_index] = crc_result;
    wr_buf_index += 1;

    if (wr_buf_index <= 0xFD)
    {
        magic_number = DS_MAGIC_NUMBER;
        wr_buf[wr_buf_index] =   magic_number;
        wr_buf_index += 1;

        wr_buf[wr_buf_index] =   magic_number >> 8;
        wr_buf_index += 1;
    }
    else if (wr_buf_index == 0xFE)
    {

        magic_number = DS_MAGIC_NUMBER;
        wr_buf[wr_buf_index] =   magic_number;
        wr_buf_index += 1;

        flash_write_page((uint32_t)wr_buf, dswraddress);
        dswraddress += wr_buf_index;
        wr_buf_index = 0;

        memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);

        wr_buf[wr_buf_index] =   magic_number >> 8;
        wr_buf_index += 1;
    }
    else if (wr_buf_index == 0xFF)
    {

        flash_write_page((uint32_t)wr_buf, dswraddress);
        dswraddress += wr_buf_index;
        wr_buf_index = 0;

        memset(wr_buf, DS_VAILD_TYPE_FF, DS_DATA_CHECK_SIZE);

        magic_number = DS_MAGIC_NUMBER;
        wr_buf[wr_buf_index] =    magic_number;
        wr_buf_index += 1;

        wr_buf[wr_buf_index] =    magic_number >> 8;
        wr_buf_index += 1;


    }

    if (over_page_flag == 1)
    {
        flash_write_page((uint32_t)wr_buf, dswraddress);
        dswraddress += wr_buf_index;
    }
    else
    {
        for (j = 0; j < wr_buf_index; j++)
        {
            flash_write_byte((dswraddress + j), wr_buf[j]);
            while (flash_check_busy()) {;}
        }

        dswraddress += wr_buf_index;
    }

    flash_enable_suspend();

    ds_table.type = type;
    ds_table.len = len;
    ds_table.sn = dsinfo.current_sn;
    ds_table.buf_addr = dsinfo.migration_address + DS_HEADER_OFFSET;
    ds_table.crc = crc_result;
    ds_table.magic = magic_number;

    ds_update_table(ds_table);

    flush_cache();

    dsinfo.migration_address = dswraddress;

    return STATUS_SUCCESS;
}

uint32_t ds_migration_erase(ds_page_t page)
{
    uint32_t i = 0, end_addr = 0;
    uint32_t  start_addr = 0;

    if ((dsinfo.invaild == DS_INVAILD) || (page > DS_PAGE_1))
    {
        return STATUS_INVALID_PARAM;
    }

    if (page == DS_PAGE_0)
    {
        start_addr = dsinfo.start_address;
        end_addr = dsinfo.middle_address;
    }
    else
    {
        start_addr = dsinfo.middle_address;
        end_addr = dsinfo.end_address;
    }

    flash_disable_suspend();

    for (i = start_addr; i < end_addr; i += DATA_SET_ERASE_SIZE)
    {
        flash_erase(FLASH_ERASE_SECTOR, i);
    }

    flash_enable_suspend();

    return STATUS_SUCCESS;
}

uint32_t ds_reset_to_default()
{
    uint32_t i = 0, end_addr = 0;
    uint32_t  start_addr = 0;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    start_addr = dsinfo.start_address;
    end_addr = dsinfo.end_address;

    for (i = start_addr; i < end_addr; i += DATA_SET_ERASE_SIZE)
    {
        flash_erase(FLASH_ERASE_SECTOR, i);
    }

    dsinfo.current_page = DS_PAGE_0;
    dsinfo.current_offset = DS_START_OFFSET;
    dsinfo.current_address = start_addr;
    dsinfo.current_sn = DS_SERAIL_NUMBER_START;

    return STATUS_SUCCESS;

}

uint32_t ds_delete_type(uint16_t type)
{
    uint32_t        address = 0;
    uint32_t        index = (type - 1);

    if ((type > dsinfo.type_max) || (type == DS_INVAILD_TYPE_00) || (type == DS_INVAILD_TYPE_FF) || (dsinfo.invaild == DS_INVAILD) || (type >= DS_TABLE_TYPE_MAX))
    {
        return STATUS_INVALID_PARAM;
    }

    if (ds_record_table[index].type == type)
    {
        address = (ds_record_table[index].buf_addr - DS_HEADER_OFFSET);

        flash_write_byte(address, 0x00);
        flash_write_byte((address + 1), 0x00);
        while (flash_check_busy()) {;} //clear type

        ds_record_table[index].type = 0;
        ds_record_table[index].len = 0x0000;
        ds_record_table[index].sn = 0;
        ds_record_table[index].buf_addr = 0;
        ds_record_table[index].crc = 0;
        ds_record_table[index].magic = 0;

        return STATUS_SUCCESS;
    }

    return STATUS_INVALID_REQUEST;
}

uint8_t ds_cal_crc(ds_rw_t *ds_crc, uint32_t sn)
{
    uint8_t value = 0, crc_result = 0;
    uint16_t len = ds_crc->len, type = ds_crc->type;
    uint32_t i = 0, address = ds_crc->address;


    crc_result ^= (type & 0xFF);
    crc_result ^= (type >> 8);
    crc_result ^= (len & 0xFF);
    crc_result ^= (len >> 8);

    crc_result ^= (sn & 0xFF);
    crc_result ^= (sn >> 8);
    crc_result ^= (sn >> 16);
    crc_result ^= (sn >> 24);

    for (i = 0; i < len; i++)
    {
        value = (*(uint32_t *)(address + i));
        crc_result ^= value;
    }

    return crc_result;
}

uint32_t ds_update_type( uint16_t type)
{
    if (type >= dsinfo.type_max)
    {
        dsinfo.type_max = type;
    }

    return STATUS_SUCCESS;
}

uint32_t ds_update_crrent_sn( uint32_t serial_number)
{
    if (serial_number >= dsinfo.current_sn)
    {
        dsinfo.current_sn = serial_number;
    }

    return STATUS_SUCCESS;
}

uint32_t ds_update_table(ds_t ds_table)
{
    uint8_t index = (ds_table.type - 1);
    uint16_t type = ds_table.type;

    if ((type > dsinfo.type_max) || (type == DS_INVAILD_TYPE_00) || (type == DS_INVAILD_TYPE_FF) || (dsinfo.invaild == DS_INVAILD))
    {
        return STATUS_INVALID_PARAM;
    }

    if (ds_record_table[index].type == DS_INVAILD_TYPE_00)
    {
        ds_record_table[index].type = ds_table.type;
        ds_record_table[index].len = ds_table.len;
        ds_record_table[index].sn = ds_table.sn;
        ds_record_table[index].buf_addr = ds_table.buf_addr;
        ds_record_table[index].crc = ds_table.crc;
        ds_record_table[index].magic = ds_table.magic;
    }
    else if ((ds_record_table[index].type == type) && (ds_record_table[index].sn < ds_table.sn))
    {
        ds_record_table[index].type = ds_table.type;
        ds_record_table[index].len = ds_table.len;
        ds_record_table[index].sn = ds_table.sn;
        ds_record_table[index].buf_addr = ds_table.buf_addr;
        ds_record_table[index].crc = ds_table.crc;
        ds_record_table[index].magic = ds_table.magic;
    }

    return STATUS_SUCCESS;
}

uint32_t ds_load_page_end_address_max()
{

    uint32_t  flash_size_id = flash_size();

#if (DS_CUSTOMER_CONFIG==1)

    if (flash_size_id == FLASH_512K)
    {
        ds_end_address = DS_512K_MAX_ADDRESS;
    }
    else if ( flash_size_id == FLASH_1024K)
    {
        ds_end_address = DS_1M_MAX_ADDRESS;
    }
    else if ( flash_size_id == FLASH_2048K)
    {
        ds_end_address = DS_2M_MAX_ADDRESS;
    }
    else
    {
        ds_end_address = 0;
        return STATUS_INVALID_PARAM;
    }

#else

    if (flash_size_id == FLASH_512K)
    {
        ds_end_address = (MP_SECTOR_BASE_512K  + 0x4000);
    }
    else if ( flash_size_id == FLASH_1024K)
    {
        ds_end_address = (MP_SECTOR_BASE_1024K + 0x4000);
    }
    else if ( flash_size_id == FLASH_2048K)
    {
        ds_end_address = (MP_SECTOR_BASE_2048K + 0x4000);
    }
    else
    {
        ds_end_address = 0;
        return STATUS_INVALID_PARAM;
    }
#endif


    return STATUS_SUCCESS;
}
