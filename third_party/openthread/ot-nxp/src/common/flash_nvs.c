/*
 *  Copyright (c) 2024, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <openthread/error.h>
#include <openthread/instance.h>
#include <openthread/logging.h>

#include <nvs_port.h>
#include <settings.h>

/* The ASCII code of the character '0' */
#define ASCII_0 0x30

#define KEY_DATA_OK 0x0000
#define KEY_NOT_FOUND 0x0001
#define KEY_BUFFER_OVERFLOW 0x0002
#define KEY_READ_FAILED 0x0004

/* NVS settings prefix for OT keys */
#define OT_KEY_PREFIX "ot"

/*
 * The format of the OpenThread key names is "ot/xxxx/y", where <x> are hex
 * digits which form the key id, and <y> is a decimal digit which designates
 * the key index. This is to say that, at this time, we can store a maximum
 * of 10 values for the same key.
 */
#define KEY_NAME_SIZE 16 /* characters */

#define KEY_NAME_TO_UINT(key) (key[0] - ASCII_0)

struct key_data
{
    char     name[KEY_NAME_SIZE];
    int      index;
    uint16_t len;
    uint16_t flags;
    uint8_t *data;
};

/*
 * Zephyr settings subtree callback function used to search and read a certain
 * key
 */
int subtree_cb_read_value(const char *key, size_t len, settings_read_cb read_cb, void *cb_arg, void *param)
{
    ssize_t          cnt;
    struct key_data *k = (struct key_data *)param;

    /* Here we assume the key index number has only one digit */
    if (k->index != KEY_NAME_TO_UINT(key))
        /*
         * This is not value we're looking for. Continue searching the settings
         * subtree for other keys.
         */
        return 0;

    /* Found something. Reset key flags. */
    k->flags = KEY_DATA_OK;

    if ((k->data == NULL) || (k->len == 0))
    {
        /*
         * The user is just testing the presence or size of this key. No
         * further data reading is required.
         */
        k->len = len;
        if (len == 0)
            k->flags |= KEY_NOT_FOUND;

        /* Return 1 (one) to stop processing further keys in this subtree */
        return 1;
    }

    /*
     * If the real size of the key exceeds the size of the key value storage
     * buffer signal a data buffer overflow.
     */
    if (len > k->len)
        k->flags |= KEY_BUFFER_OVERFLOW;

    /* Read the key value as much as it fits in the provided buffer */
    cnt = read_cb(cb_arg, k->data, k->len);
    if (cnt > 0)
    {
        k->len = (uint16_t)cnt;
    }
    else
    {
        /*
         * We have either read an empty key or there was an error reading the
         * data.
         */
        k->len = 0;
        if (cnt == 0)
        {
            k->flags |= KEY_NOT_FOUND;
        }
        else
        {
            k->flags |= KEY_READ_FAILED;
        }
    }

    /* Return 1 (one) to stop processing further keys in this subtree */
    return 1;
}

/*
 * Zephyr settings subtree callback function used to count the keys available
 * in the subtree
 */
int subtree_cb_count_values(const char *key, size_t len, settings_read_cb read_cb, void *cb_arg, void *param)
{
    unsigned *cnt = (unsigned *)param;

    (*cnt)++;

    /* Continue searching the subtree for other keys */
    return 0;
}

/*
 * Zephyr settings subtree callback function used to wipe out all the keys
 * available in the subtree
 */
int subtree_cb_wipe_all(const char *key, size_t len, settings_read_cb read_cb, void *cb_arg, void *param)
{
    int  err;
    char key_name[KEY_NAME_SIZE];

    /* Generate the full name of the key */
    sprintf(key_name, OT_KEY_PREFIX "/%s", key);
    err = settings_delete(key_name);
    if (err != 0)
    {
        /* An error has occurred hence abort this operation */
        otLogWarnPlat("WARNING: Failed wipe out settings subtree! (err=%d)", err);
        return 1;
    }

    /* Continue browsing and removing the rest of the keys in this subtree */
    return 0;
}

void otPlatSettingsInit(otInstance *aInstance, const uint16_t *aSensitiveKeys, uint16_t aSensitiveKeysLength)
{
    const struct flash_area *fa;
    int                      err;

    /* Get flash memory driver params */
    err = flash_area_open(SETTINGS_PARTITION, &fa);
    if (err != 0)
    {
        otLogCritPlat("ERROR: Failed acquire flash memory properties! (err=%d)", err);
        return;
    }

    /* Flash memory init */
    err = flash_init(fa->fa_dev);
    if (err != 0)
    {
        otLogCritPlat("ERROR: Failed initialize flash memory! (err=%d)", err);
        return;
    }

    /* Zephyr Settings module init */
    err = settings_subsys_init();
    if (err != 0)
    {
        otLogCritPlat("ERROR: Failed initialize settings management subsystem! (err=%d)", err);
    }
}

void otPlatSettingsDeinit(otInstance *aInstance)
{
}

otError otPlatSettingsGet(otInstance *aInstance, uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength)
{
    int             err;
    otError         ret = OT_ERROR_NONE;
    struct key_data k   = {.index = aIndex, .flags = KEY_NOT_FOUND, .len = *aValueLength, .data = aValue};

    /* Generate the name of the key, as known by the NVS/Settings module */
    sprintf(k.name, OT_KEY_PREFIX "/%x", aKey);
    err = settings_load_subtree_direct(k.name, subtree_cb_read_value, &k);
    if ((err != 0) || ((k.flags & KEY_NOT_FOUND) != 0) || ((k.flags & KEY_READ_FAILED) != 0))
    {
        *aValueLength = 0;
        ret           = OT_ERROR_NOT_FOUND;
        otLogWarnPlat("ERROR: Failed to get settings key 0x%04x[%d]!", aKey, aIndex);
    }
    else
    {
        *aValueLength = k.len;
    }

    return ret;
}

otError otPlatSettingsAdd(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    int      err;
    char     key_name[KEY_NAME_SIZE];
    unsigned cnt = 0;

    /* Count the number of values already available for this key */
    sprintf(key_name, OT_KEY_PREFIX "/%x", aKey);
    err = settings_load_subtree_direct(key_name, subtree_cb_count_values, &cnt);
    if (err != 0)
    {
        return OT_ERROR_NO_BUFS;
    }

    /* Generate the name of the key, as known by the NVS/Settings module */
    sprintf(key_name, OT_KEY_PREFIX "/%x/%u", aKey, cnt);
    err = settings_save_one(key_name, aValue, aValueLength);
    if (err != 0)
    {
        return OT_ERROR_NO_BUFS;
    }

    return OT_ERROR_NONE;
}

otError otPlatSettingsSet(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    int     err;
    otError ret = OT_ERROR_NONE;
    char    key_name[KEY_NAME_SIZE];

    /* Generate the name of the key, as known by the NVS/Settings module */
    sprintf(key_name, OT_KEY_PREFIX "/%x/0", aKey);
    err = settings_save_one(key_name, aValue, aValueLength);
    if (err != 0)
    {
        otLogWarnPlat("ERROR: Failed to set settings key 0x%04x!", aKey);
        ret = OT_ERROR_NO_BUFS;
    }

    return ret;
}

otError otPlatSettingsDelete(otInstance *aInstance, uint16_t aKey, int aIndex)
{
    int     err;
    otError ret = OT_ERROR_NONE;
    char    key_name[KEY_NAME_SIZE];

    /* Generate the name of the key, as known by the NVS/Settings module */
    sprintf(key_name, OT_KEY_PREFIX "/%x/%d", aKey, aIndex);
    err = settings_delete(key_name);
    if (err != 0)
    {
        otLogWarnPlat("ERROR: Failed to remove settings key 0x%04x[%d]!", aKey, aIndex);
        ret = OT_ERROR_NOT_FOUND;
    }

    return ret;
}

void otPlatSettingsWipe(otInstance *aInstance)
{
    int err;

    err = settings_load_subtree_direct(OT_KEY_PREFIX, subtree_cb_wipe_all, NULL);
    if (err != 0)
    {
        otLogWarnPlat("WARNING: Failed to wipe out OT settings keys (err=%d)!", err);
    }
}

void otPlatSaveSettingsIdle(void)
{
}

void otPlatDumpOtSettings(void)
{
}
