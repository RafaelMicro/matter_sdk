/**************************************************************************/ /**
                                                                              * @file     flashds.c
                                                                              * @version
                                                                              * @brief
                                                                              *
                                                                              * @copyright
                                                                              *****************************************************************************/
#include "cm3_mcu.h"
#include <stdio.h>
#include <string.h>

static ds_info_t dsinfo;

uint32_t ds_initinal(ds_config_t ds_init)
{
    uint32_t page_size = 0;

    if (ds_init.ds_page_mode == DS_ONE_PAGE_MODE)
    {
        if (ds_init.end_address > DS_ONE_PAGE_END_ADDRESS_MAX)
        {
            dsinfo.invaild = DS_INVAILD;
            return STATUS_INVALID_PARAM;
        }

        dsinfo.page_number = (ds_init.end_address - ds_init.start_address) / DATA_PAGE_SIZE_4K;

        if (dsinfo.page_number < DS_PAGE_1 || (dsinfo.page_number % 2) != 0)
        {
            dsinfo.invaild = DS_INVAILD;
            return STATUS_INVALID_PARAM;
        }

        page_size             = (ds_init.end_address - ds_init.start_address) >> 1;
        dsinfo.start_address  = ds_init.start_address;
        dsinfo.middle_address = ds_init.start_address + page_size;
        dsinfo.end_address    = ds_init.end_address;
        dsinfo.page_mode      = DS_ONE_PAGE_MODE;
    }
    else if (ds_init.ds_page_mode == DS_TWO_PAGE_MODE)
    {

        if (ds_init.end_address > DS_TWO_PAGE_END_ADDRESS_MAX)
        {
            dsinfo.invaild = DS_INVAILD;
            return STATUS_INVALID_PARAM;
        }

        page_size = (ds_init.end_address - ds_init.start_address) >> 1;

        if ((page_size == DATA_PAGE_SIZE_8K) || (page_size == DATA_PAGE_SIZE_4K))
        {
            dsinfo.start_address  = ds_init.start_address;
            dsinfo.end_address    = ds_init.end_address;
            dsinfo.middle_address = ds_init.start_address + page_size;
            dsinfo.page_number    = ((ds_init.end_address - ds_init.start_address) / page_size) >> 1;
            dsinfo.page_mode      = DS_TWO_PAGE_MODE;
        }
        else
        {
            dsinfo.invaild = DS_INVAILD;
            return STATUS_INVALID_PARAM;
        }
    }
    else
    {
        dsinfo.invaild = DS_INVAILD;
        return STATUS_INVALID_PARAM;
    }

    ds_pagex_check();

    return STATUS_SUCCESS;
}

uint32_t ds_pagex_check()
{

    uint32_t page1_satatus, page2_satatus, satatus = STATUS_SUCCESS;
    uint32_t ds_page1_addr = dsinfo.start_address;
    uint32_t ds_page2_addr = dsinfo.middle_address;

    uint32_t ds_page1_sn, ds_page2_sn;
    ds_search_t ds_search_pagex;
    ds_t *dspack1, *dspack2;

    ds_search_pagex.address     = ds_page1_addr;
    ds_search_pagex.end_address = dsinfo.middle_address;
    ds_search_pagex.offset      = DS_START_OFFSET;
    ds_search_pagex.type        = DS_VAILD_TYPE;
    ds_search_pagex.flag        = DS_VAILD_TYPE_SEARCH;

    page1_satatus = ds_erase_check(&ds_search_pagex);

    if ((dsinfo.page_mode == DS_TWO_PAGE_MODE) || (dsinfo.page_mode == DS_ONE_PAGE_MODE))
    {
        ds_search_pagex.address     = ds_page2_addr;
        ds_search_pagex.end_address = dsinfo.end_address;

        page2_satatus = ds_erase_check(&ds_search_pagex);

        if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus == STATUS_SUCCESS)) // Page1 and Page 2 All 0xFF
        {
            dsinfo.current_page    = DS_PAGE_0;
            dsinfo.current_offset  = DS_START_OFFSET;
            dsinfo.current_address = ds_page1_addr;
            dsinfo.current_sn      = DS_SERAIL_NUMBER_START;
        }
        else if ((page1_satatus != STATUS_SUCCESS) && (page2_satatus == STATUS_SUCCESS)) // Page2 All 0xFF,Page1 has record data
        {
            satatus = ds_page_vaild_address(ds_page1_addr, dspack1); // check page 1 data

            if (satatus == STATUS_INVALID_PARAM) // Page1 invaild data
            {
                dsinfo.current_page    = DS_PAGE_1;
                dsinfo.current_offset  = DS_START_OFFSET;
                dsinfo.current_address = ds_page2_addr;
                ds_migration(DS_PAGE_0);
                ds_migration_erase(DS_PAGE_0);
            }
        }
        else if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus != STATUS_SUCCESS)) // Page1 All 0xFF, Page2 has record data
        {
            satatus = ds_page_vaild_address(ds_page2_addr, dspack2); // check page 1 data

            if (satatus == STATUS_INVALID_PARAM) // Page2 invaild data
            {
                dsinfo.current_page    = DS_PAGE_0;
                dsinfo.current_offset  = DS_START_OFFSET;
                dsinfo.current_address = ds_page1_addr;
                ds_migration(DS_PAGE_1);
                ds_migration_erase(DS_PAGE_1);
            }
        }
        else
        {
            page1_satatus = ds_page_vaild_address(ds_page1_addr, dspack1);
            ds_page1_sn   = dsinfo.current_sn;

            page2_satatus = ds_page_vaild_address(ds_page2_addr, dspack2);
            ds_page2_sn   = dsinfo.current_sn;

            if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus == STATUS_SUCCESS)) // all page data is okay
            {
                if (ds_page1_sn > ds_page2_sn)
                {
                    ds_migration_erase(DS_PAGE_1);
                    ds_migration(DS_PAGE_0);
                    ds_migration_erase(DS_PAGE_0);
                    ds_page_vaild_address(ds_page2_addr, dspack2);
                }
                else
                {
                    ds_migration_erase(DS_PAGE_0);
                    ds_migration(DS_PAGE_1);
                    ds_migration_erase(DS_PAGE_1);
                    ds_page_vaild_address(ds_page1_addr, dspack1);
                }
            }
            else if ((page1_satatus == STATUS_INVALID_PARAM) && (page2_satatus == STATUS_SUCCESS))
            {
                ds_migration_erase(DS_PAGE_0);
                dsinfo.current_page = DS_PAGE_1;
            }
            else if ((page1_satatus == STATUS_SUCCESS) && (page2_satatus == STATUS_INVALID_PARAM))
            {
                ds_migration_erase(DS_PAGE_1);
                dsinfo.current_page = DS_PAGE_0;
            }
            else if ((page1_satatus == STATUS_INVALID_PARAM) && (page2_satatus == STATUS_INVALID_PARAM))
            {
                ds_migration_erase(DS_PAGE_0);
                ds_migration_erase(DS_PAGE_1);
                dsinfo.current_page    = DS_PAGE_0;
                dsinfo.current_offset  = DS_START_OFFSET;
                dsinfo.current_address = ds_page1_addr;
                dsinfo.current_sn      = DS_SERAIL_NUMBER_START;
            }
        }
    }
    else
    {
        return STATUS_INVALID_REQUEST;
    }

    return STATUS_SUCCESS;
}

uint32_t ds_erase_check(ds_search_t * ds_search)
{
    uint8_t dsbufP1[DS_DATA_CHECK_SIZE];
    uint32_t i, j;
    uint32_t end_address, start_address;

    end_address   = ds_search->end_address;
    start_address = ds_search->address;

    for (i = start_address; i < end_address; i += DS_DATA_CHECK_SIZE)
    {
        flash_read_page((uint32_t) dsbufP1, i);

        while (flash_check_busy())
            ;

        for (j = 0; j < DS_DATA_CHECK_SIZE; j++)
        {
            if (dsbufP1[j] != DS_VAILD_TYPE)
            {
                ds_search->offset  = j;
                ds_search->address = i;

                return STATUS_ERROR;
            }
        }
    }

    return STATUS_SUCCESS;
}

uint32_t ds_page_vaild_address(uint32_t ds_start_address, ds_t * dspacktmp)
{
    uint32_t ds_status;
    ds_search_t ds_vaild_search;

    ds_vaild_search.address = ds_start_address;
    ds_vaild_search.offset  = DS_START_OFFSET;
    ds_vaild_search.type    = DS_VAILD_TYPE;
    ds_vaild_search.flag    = DS_VAILD_TYPE_SEARCH;

    if (ds_start_address == dsinfo.start_address)
    {
        ds_vaild_search.end_address = dsinfo.middle_address;
    }
    else
    {
        ds_vaild_search.end_address = dsinfo.end_address;
    }

    ds_status = ds_vaild_address_search(&ds_vaild_search, dspacktmp);

    if (ds_status == STATUS_SUCCESS)
    {
        if (dsinfo.current_address < dsinfo.middle_address)
        {
            dsinfo.current_page = DS_PAGE_0;
        }
        else
        {
            dsinfo.current_page = DS_PAGE_1;
        }
    }

    return ds_status;
}

uint32_t ds_vaild_address_search(ds_search_t * dssearch, ds_t * dspacktmp)
{

    uint32_t ds_status, read_type_search;
    uint32_t address;
    ds_t dspack;

    dspack = (*(ds_t *) (dssearch->address + dssearch->offset));

    if ((dspack.type == DS_VAILD_TYPE) && (dspack.sn == DS_SERAIL_NUMBER_MAX))
    {
        return STATUS_INVALID_REQUEST;
    }

    read_type_search = 0;

    do
    {
        dspack = (*(ds_t *) (dssearch->address + dssearch->offset));

        ds_status = ds_vaild(dssearch, &dspack);

        if (ds_status == STATUS_SUCCESS && dssearch->type == DS_VAILD_TYPE)
        {
            break;
        }

        if (ds_status == DS_READ_FINSIH_SUCCESS && dssearch->type != DS_VAILD_TYPE)
        {
            if ((dspack.type == DS_VAILD_TYPE) && (read_type_search != DS_READ_TYPE_SEARCH) &&
                (read_type_search != DS_READ_TYPE_DELETE))
            {
                ds_status = STATUS_INVALID_REQUEST;
            }
            else
            {
                ds_status = STATUS_SUCCESS;
            }

            break;
        }
        else if (ds_status == DS_READ_STATUS_SUCCESS && dssearch->type != DS_VAILD_TYPE)
        {
            if (dspack.type == DS_VAILD_TYPE)
            {
                ds_status = STATUS_INVALID_REQUEST;
            }
            else
            {
                read_type_search = DS_READ_TYPE_SEARCH;
                memcpy(dspacktmp, (ds_t *) &dspack, sizeof(ds_t));
            }
        }

        dssearch->offset += DS_HEADER_OFFSET + dspack.len + DS_TAIL_OFFSET;

        address = (dssearch->address + dssearch->offset);

        if (address > dssearch->end_address)
        {
            ds_status = STATUS_INVALID_REQUEST;
            break;
        }

    } while (ds_status != STATUS_SUCCESS);

    return ds_status;
}

uint32_t ds_vaild(ds_search_t * dssearch, ds_t * ds_get_vaild)
{
    static uint8_t crc_result, crc;
    uint16_t magic;
    uint32_t address;
    ds_t dspack;
    ds_rw_t dscrc;
    address = (dssearch->address + dssearch->offset);

    dspack = (*(ds_t *) (dssearch->address + dssearch->offset));

    if (dspack.type == DS_VAILD_TYPE)
    {
        if (dssearch->flag == DS_VAILD_TYPE_SEARCH)
        {
            dsinfo.current_address = dssearch->address + dssearch->offset;
            dsinfo.current_offset  = dssearch->offset;

            return STATUS_SUCCESS;
        }
        else
        {
            return DS_READ_FINSIH_SUCCESS;
        }
    }
    else if ((dssearch->type == dspack.type) || (dssearch->type == DS_VAILD_TYPE) || (dssearch->type == DS_INVAILD_TYPE_00))
    {
        ds_get_vaild->type = dspack.type;
        ds_get_vaild->len  = dspack.len;
        ds_get_vaild->sn   = dspack.sn;
        address += DS_HEADER_OFFSET;

        ds_get_vaild->buf_addr = address;

        address += dspack.len;
        crc = flash_read_byte(address);

        address += sizeof(uint8_t);
        magic = flash_read_byte(address);

        address += sizeof(uint8_t);
        magic |= flash_read_byte(address) << 8;

        dscrc.type    = ds_get_vaild->type;
        dscrc.address = ds_get_vaild->buf_addr;
        dscrc.len     = ds_get_vaild->len;
        crc_result    = ds_cal_crc(&dscrc, dspack.sn);

        if (dssearch->type != DS_INVAILD_TYPE_00)
        {
            if (crc_result != crc)
            {
                return STATUS_INVALID_PARAM;
            }
        }

        if (dssearch->flag == DS_VAILD_TYPE_SEARCH)
        {

            ds_update_type(dspack.type);

            if (dspack.sn >= dsinfo.current_sn)
            {
                dsinfo.current_sn = dspack.sn;
            }
        }

        ds_get_vaild->crc   = crc;
        ds_get_vaild->magic = magic;

        if (dssearch->type == dspack.type)
        {
            if (dssearch->flag == DS_READ_TYPE_FLAG)
            {
                return DS_READ_STATUS_SUCCESS;
            }
            else if (dssearch->flag == DS_READ_TYPE_DELETE)
            {
                address = (ds_get_vaild->buf_addr - DS_HEADER_OFFSET);

                flash_write_byte(address, DS_INVAILD_TYPE_00);
                while (flash_check_busy())
                {
                    ;
                } // clear type

                return DS_READ_DELETE_SUCCESS;
            }
        }
    }

    return STATUS_INVALID_REQUEST;
}

uint32_t ds_get_current_page()
{
    return dsinfo.current_page;
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
    uint32_t dsaddress, endaddress;

    if (ds_get_current_page() == DS_PAGE_0)
    {
        dsaddress  = dsinfo.current_address + set_page_offset;
        endaddress = dsinfo.middle_address;
    }
    else
    {
        dsaddress  = dsinfo.current_address + set_page_offset;
        endaddress = dsinfo.end_address;
    }

    if ((dsaddress > endaddress))
    {
        return STATUS_INVALID_PARAM;
    }

    dsinfo.current_address += set_page_offset;

    return STATUS_SUCCESS;
}

uint32_t ds_read(ds_rw_t * ds_read)
{
    uint32_t ds_status;
    uint32_t start_address, end_address;
    ds_search_t dssearch;
    ds_t dspacktmp;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if (ds_get_current_page() == DS_PAGE_0)
    {
        start_address = dsinfo.start_address;
        end_address   = dsinfo.middle_address;
    }
    else
    {
        start_address = dsinfo.middle_address;
        end_address   = dsinfo.end_address;
    }

    dssearch.flag        = DS_READ_TYPE_SEARCH;
    dssearch.type        = ds_read->type;
    dssearch.address     = start_address;
    dssearch.end_address = end_address;
    dssearch.offset      = DS_START_OFFSET;

    ds_status = ds_vaild_address_search(&dssearch, &dspacktmp);

    if (ds_status == STATUS_SUCCESS)
    {
        ds_read->address = dspacktmp.buf_addr;
        ds_read->len     = dspacktmp.len;
    }
    else
    {
        ds_read->address = 0;
        ds_read->len     = 0;
    }

    return ds_status;
}

uint32_t ds_write(ds_rw_t * ds_write)
{

    uint8_t temp, crc_result;
    uint32_t i;
    uint32_t dswraddress, dsmaxaddress;
    uint16_t magic_number;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if ((ds_write->type == DS_INVAILD_TYPE_00) || (ds_write->type == DS_INVAILD_TYPE_FF))
    {
        return STATUS_INVALID_REQUEST;
    }

    if (ds_get_current_page() == DS_PAGE_0)
    {
        dsmaxaddress = dsinfo.middle_address;
    }
    else
    {
        dsmaxaddress = dsinfo.end_address;
    }

    dswraddress = dsinfo.current_address + DS_HEADER_OFFSET + ds_write->len + DS_TAIL_OFFSET;

    if (dswraddress >= dsinfo.end_address)
    {
        return STATUS_INVALID_REQUEST;
    }

    if ((dswraddress > dsmaxaddress))
    {

        if (ds_get_current_page() == DS_PAGE_1)
        {
            ds_migration_erase(DS_PAGE_0);
            ds_migration(DS_PAGE_1);
            dsinfo.current_page = DS_PAGE_0;

            dswraddress = dsinfo.migration_address + DS_HEADER_OFFSET + ds_write->len + DS_TAIL_OFFSET;
        }
        else
        {
            ds_migration_erase(DS_PAGE_1);
            ds_migration(DS_PAGE_0);
            dsinfo.current_page = DS_PAGE_1;

            dswraddress = dsinfo.migration_address + DS_HEADER_OFFSET + ds_write->len + DS_TAIL_OFFSET;
        }

        if ((dswraddress > dsmaxaddress))
        {
            dsinfo.current_address = dsinfo.migration_address;
            return STATUS_INVALID_REQUEST;
        }
    }

    dswraddress = dsinfo.current_address;

    ds_update_type(ds_write->type);

    flash_write_byte(dswraddress, ds_write->type);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, ds_write->len);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, ds_write->len >> 8);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    if (dsinfo.current_sn == DS_SERAIL_NUMBER_MAX)
    {
        dsinfo.current_sn = DS_SERAIL_NUMBER_START;
    }

    flash_write_byte(dswraddress, dsinfo.current_sn);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, dsinfo.current_sn >> 8);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, dsinfo.current_sn >> 16);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, dsinfo.current_sn >> 24);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    for (i = 0; i < ds_write->len; i++)
    {
        temp = (*(uint32_t *) (ds_write->address + i));
        flash_write_byte(dswraddress, temp);
        while (flash_check_busy())
        {
            ;
        }
        dswraddress += 1;
    }

    crc_result = ds_cal_crc(ds_write, dsinfo.current_sn);
    flash_write_byte(dswraddress, crc_result);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    magic_number = DS_MAGIC_NUMBER;
    flash_write_byte(dswraddress, magic_number);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, magic_number >> 8);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    dsinfo.current_address = dswraddress;
    dsinfo.current_offset  = dswraddress;
    dsinfo.current_sn += 1;

    return STATUS_SUCCESS;
}

uint32_t ds_migration(ds_page_t page)
{

    uint32_t ds_status;
    uint32_t address, end_address, i;
    uint8_t update_flag;

    ds_search_t dssearch;
    ds_rw_t dswr;
    ds_t dspack;
    ds_t ds_migration_after;
    ds_t ds_migration_befor;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if (page == DS_PAGE_0)
    {
        address                  = dsinfo.start_address;
        end_address              = dsinfo.middle_address;
        dsinfo.migration_address = dsinfo.middle_address;
    }
    else
    {
        address                  = dsinfo.middle_address;
        end_address              = dsinfo.end_address;
        dsinfo.migration_address = dsinfo.start_address;
    }

    update_flag = DS_UPDATE_DISABLE;

    for (i = 1; i <= dsinfo.type_max; i++)
    {
        dssearch.address = address;
        dssearch.offset  = DS_START_OFFSET;
        dssearch.type    = (uint8_t) i;
        dssearch.flag    = DS_READ_TYPE_SEARCH;

        do
        {

            dspack = (*(ds_t *) (dssearch.address + dssearch.offset));

            ds_status = ds_vaild(&dssearch, &dspack);

            if (ds_status == DS_READ_STATUS_SUCCESS && dssearch.type != DS_VAILD_TYPE)
            {
                if (update_flag == DS_UPDATE_DISABLE)
                {
                    memcpy((ds_t *) &ds_migration_befor, (ds_t *) &dspack, sizeof(ds_t));
                    update_flag = DS_UPDATE_ENABLE;
                }
                else
                {
                    memcpy((ds_t *) &ds_migration_after, (ds_t *) &dspack, sizeof(ds_t));

                    if ((ds_migration_after.sn) > (ds_migration_befor.sn))
                    {
                        memcpy((ds_t *) &ds_migration_befor, (ds_t *) &ds_migration_after, sizeof(ds_t));
                        update_flag = DS_UPDATE_ENABLE;
                    }
                }
            }

            dssearch.offset += DS_HEADER_OFFSET + dspack.len + DS_TAIL_OFFSET;

            if ((dssearch.address + dssearch.offset) > end_address)
            {
                break;
            }

        } while (ds_status != STATUS_SUCCESS);

        if (update_flag == DS_UPDATE_ENABLE)
        {
            dswr.type    = ds_migration_befor.type;
            dswr.len     = ds_migration_befor.len;
            dswr.address = ds_migration_befor.buf_addr;
            ds_migration_write(page, &dswr);
            update_flag = DS_UPDATE_DISABLE;
        }
    }

    return STATUS_SUCCESS;
}
uint32_t ds_migration_write(ds_page_t page, ds_rw_t * ds_write)
{

    uint32_t i;
    uint32_t dswraddress;
    uint8_t bufTemp, crc_result;
    uint16_t magic_number;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if ((ds_write->type == DS_INVAILD_TYPE_00) || (ds_write->type == DS_INVAILD_TYPE_FF))
    {
        return STATUS_INVALID_REQUEST;
    }

    dswraddress = dsinfo.migration_address;

    flash_write_byte(dswraddress, ds_write->type);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, ds_write->len);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, ds_write->len >> 8);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    if (dsinfo.current_sn == DS_SERAIL_NUMBER_MAX)
    {
        dsinfo.current_sn = DS_SERAIL_NUMBER_START;
    }

    flash_write_byte(dswraddress, dsinfo.current_sn);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, dsinfo.current_sn >> 8);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, dsinfo.current_sn >> 16);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, dsinfo.current_sn >> 24);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    for (i = 0; i < ds_write->len; i++)
    {
        bufTemp = (*(uint32_t *) (ds_write->address + i));
        flash_write_byte(dswraddress, bufTemp);
        while (flash_check_busy())
        {
            ;
        }
        dswraddress += 1;
    }

    crc_result = ds_cal_crc(ds_write, dsinfo.current_sn);
    flash_write_byte(dswraddress, crc_result);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    magic_number = DS_MAGIC_NUMBER;
    flash_write_byte(dswraddress, magic_number);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;
    flash_write_byte(dswraddress, magic_number >> 8);
    while (flash_check_busy())
    {
        ;
    }
    dswraddress += 1;

    dsinfo.migration_address = dswraddress;
    dsinfo.current_sn += 1;

    return STATUS_SUCCESS;
}

uint32_t ds_migration_erase(ds_page_t page)
{
    uint32_t i, end_addr;
    uint32_t start_addr;

    if ((dsinfo.invaild == DS_INVAILD) || (page > DS_PAGE_1))
    {
        return STATUS_INVALID_PARAM;
    }

    if (page == DS_PAGE_0)
    {
        start_addr = dsinfo.start_address;
        end_addr   = dsinfo.middle_address;
    }
    else
    {
        start_addr = dsinfo.middle_address;
        end_addr   = dsinfo.end_address;
    }

    for (i = start_addr; i < end_addr; i += DATA_SET_ERASE_SIZE)
    {
        flash_erase(FLASH_ERASE_SECTOR, i);
    }

    return STATUS_SUCCESS;
}

uint32_t ds_reset_to_default()
{
    uint32_t i, end_addr;
    uint32_t start_addr;

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    start_addr = dsinfo.start_address;
    end_addr   = dsinfo.end_address;

    for (i = start_addr; i < end_addr; i += DATA_SET_ERASE_SIZE)
    {
        flash_erase(FLASH_ERASE_SECTOR, i);
    }

    dsinfo.current_page    = DS_PAGE_1;
    dsinfo.current_offset  = DS_START_OFFSET;
    dsinfo.current_address = start_addr;
    dsinfo.current_sn      = DS_SERAIL_NUMBER_START;

    return STATUS_SUCCESS;
}

uint32_t ds_delete_type(uint8_t type)
{

    uint32_t ds_status;
    uint32_t start_address, end_address;
    ds_search_t dssearch;
    ds_t dspacktmp;

    if ((type > dsinfo.type_max) || (type == DS_INVAILD_TYPE_00) || (type == DS_INVAILD_TYPE_FF) || (dsinfo.invaild == DS_INVAILD))
    {
        return STATUS_INVALID_PARAM;
    }

    if (dsinfo.invaild == DS_INVAILD)
    {
        return STATUS_INVALID_PARAM;
    }

    if (ds_get_current_page() == DS_PAGE_0)
    {
        start_address = dsinfo.start_address;
        end_address   = dsinfo.middle_address;
    }
    else
    {
        start_address = dsinfo.middle_address;
        end_address   = dsinfo.end_address;
    }

    dssearch.flag        = DS_READ_TYPE_DELETE;
    dssearch.type        = type;
    dssearch.address     = start_address;
    dssearch.end_address = end_address;
    dssearch.offset      = DS_START_OFFSET;

    ds_status = ds_vaild_address_search(&dssearch, &dspacktmp);

    return ds_status;
}

uint8_t ds_cal_crc(ds_rw_t * ds_crc, uint32_t sn)
{
    uint8_t value = 0, crc_result = 0;
    uint32_t i = 0;

    crc_result ^= (ds_crc->type);
    crc_result ^= (ds_crc->len & 0xFF);
    crc_result ^= (ds_crc->len >> 8);

    crc_result ^= (sn & 0xFF);
    crc_result ^= (sn >> 8);
    crc_result ^= (sn >> 16);
    crc_result ^= (sn >> 24);

    for (i = 0; i < ds_crc->len; i++)
    {
        value = (*(uint32_t *) (ds_crc->address + i));
        crc_result ^= value;
    }

    return crc_result;
}

uint32_t ds_update_type(uint8_t type)
{
    if (type >= dsinfo.type_max)
    {
        dsinfo.type_max = type;
    }

    return STATUS_SUCCESS;
}

uint32_t ds_update_crrent_sn(uint32_t serial_number)
{
    if (serial_number >= dsinfo.current_sn)
    {
        dsinfo.current_sn = serial_number;
    }

    return STATUS_SUCCESS;
}
