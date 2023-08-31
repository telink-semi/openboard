/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#pragma once

#include "msc.h"

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+


#define CFG_TUD_MSC_EP_BUFSIZE 512

//--------------------------------------------------------------------+
// Application API
//--------------------------------------------------------------------+

// Set SCSI sense response
bool tud_msc_set_sense(unsigned char lun, unsigned char sense_key, unsigned char add_sense_code, unsigned char add_sense_qualifier);

//--------------------------------------------------------------------+
// Application Callbacks (WEAK is optional)
//--------------------------------------------------------------------+

// Invoked when received SCSI READ10 command
// - Address = lba * BLOCK_SIZE + offset
//   - offset is only needed if CFG_TUD_MSC_EP_BUFSIZE is smaller than BLOCK_SIZE.
//
// - Application fill the buffer (up to bufsize) with address contents and return number of read byte. If
//   - read < bufsize : These bytes are transferred first and callback invoked again for remaining data.
//
//   - read == 0      : Indicate application is not ready yet e.g disk I/O busy.
//                      Callback invoked again with the same parameters later on.
//
//   - read < 0       : Indicate application error e.g invalid address. This request will be STALLed
//                      and return failed status in command status wrapper phase.
int tud_msc_read10_cb (unsigned char lun, unsigned int lba, unsigned int offset, void* buffer, unsigned int bufsize);

// Invoked when received SCSI WRITE10 command
// - Address = lba * BLOCK_SIZE + offset
//   - offset is only needed if CFG_TUD_MSC_EP_BUFSIZE is smaller than BLOCK_SIZE.
//
// - Application write data from buffer to address contents (up to bufsize) and return number of written byte. If
//   - write < bufsize : callback invoked again with remaining data later on.
//
//   - write == 0      : Indicate application is not ready yet e.g disk I/O busy.
//                       Callback invoked again with the same parameters later on.
//
//   - write < 0       : Indicate application error e.g invalid address. This request will be STALLed
//                       and return failed status in command status wrapper phase.
//
// TODO change buffer to const unsigned char*
int tud_msc_write10_cb (unsigned char lun, unsigned int lba, unsigned int offset, unsigned char* buffer, unsigned int bufsize);

// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively
void tud_msc_inquiry_cb(unsigned char lun, unsigned char vendor_id[8], unsigned char product_id[16], unsigned char product_rev[4]);

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(unsigned char lun);

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(unsigned char lun, unsigned int* block_count, unsigned short* block_size);

/**
 * Invoked when received an SCSI command not in built-in list below.
 * - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, TEST_UNIT_READY, START_STOP_UNIT, MODE_SENSE6, REQUEST_SENSE
 * - READ10 and WRITE10 has their own callbacks
 *
 * \param[in]   lun         Logical unit number
 * \param[in]   scsi_cmd    SCSI command contents which application must examine to response accordingly
 * \param[out]  buffer      Buffer for SCSI Data Stage.
 *                            - For INPUT: application must fill this with response.
 *                            - For OUTPUT it holds the Data from host
 * \param[in]   bufsize     Buffer's length.
 *
 * \return      Actual bytes processed, can be zero for no-data command.
 * \retval      negative    Indicate error e.g unsupported command, tinyusb will \b STALL the corresponding
 *                          endpoint and return failed status in command status wrapper phase.
 */
int tud_msc_scsi_cb (unsigned char lun, unsigned char const scsi_cmd[16], void* buffer, unsigned short bufsize);

/*------------- Optional callbacks -------------*/

// Invoked when received GET_MAX_LUN request, required for multiple LUNs implementation
unsigned char tud_msc_get_maxlun_cb(void);

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage
bool tud_msc_start_stop_cb(unsigned char lun, unsigned char power_condition, bool start, bool load_eject);

// Invoked when Read10 command is complete
void tud_msc_read10_complete_cb(unsigned char lun);

// Invoke when Write10 command is complete, can be used to flush flash caching
void tud_msc_write10_complete_cb(unsigned char lun);

// Invoked when command in tud_msc_scsi_cb is complete
void tud_msc_scsi_complete_cb(unsigned char lun, unsigned char const scsi_cmd[16]);
void tud_msc_read10_start_cb(unsigned char lun, unsigned int lba, unsigned int total_bytes);
void tud_msc_write10_start_cb(unsigned char lun, unsigned int lba, unsigned int total_bytes);
// Hook to make a mass storage device read-only. TODO remove
//bool tud_msc_is_writable_cb(unsigned char lun);

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+
void     mscd_init            (void);
void     mscd_reset           (unsigned char rhport);
unsigned short mscd_open            (unsigned char rhport, tusb_desc_interface_t const * itf_desc, unsigned short max_len);
bool     mscd_xfer_cb         (unsigned char rhport, unsigned char ep_addr, xfer_result_t event, unsigned int xferred_bytes);


