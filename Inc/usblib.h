/* 
 * This file is part of the SaeWave RemoteSwitch (USB-CDC-CMSIS) 
 * distribution (https://github.com/saewave/STM32F103-USB-CDC-CMSIS).
 * Copyright (c) 2017 Samoilov Alexey.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef USBLIB_H
#define USBLIB_H

#include "stdint.h"

#define EPCOUNT 4
#define USB_BASE ((uint32_t)0x40005C00)
#define USB_PBUFFER ((uint32_t)0x40006000)
#define STRX 12
#define STTX 4
#define CTR_RX 0x8000
#define CTR_TX 0x80
#define CDC_CMD_PACKET_SIZE 8          /* Control Endpoint Packet size */
#define CDC_DATA_FS_CMD_PACKET_SIZE 16 /* Endpoint IN & OUT Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE 64 /* Endpoint IN & OUT Packet size */
#define LANG_US (uint16_t)0x0409

#define DEVICE_VENDOR_ID 0x25AE
#define DEVICE_PRODUCT_ID 0x24AB
#define STRING_DT 3

/* EPxREG: EndPoint Registers Bit Definitions */
#define EP_CTR_RX 0x8000  /* Correct RX Transfer */
#define EP_DTOG_RX 0x4000 /* RX Data Toggle */
#define EP_STAT_RX 0x3000 /* RX Status */
#define EP_SETUP 0x0800   /* EndPoint Setup */
#define EP_TYPE 0x0600    /* EndPoint Type */
#define EP_KIND 0x0100    /* EndPoint Kind */
#define EP_CTR_TX 0x0080  /* Correct TX Transfer */
#define EP_DTOG_TX 0x0040 /* TX Data Toggle */
#define EP_STAT_TX 0x0030 /* TX Status */
#define EP_EA 0x000F      /* EndPoint Address */
#define EP_MASK (EP_CTR_RX | EP_SETUP | EP_TYPE | EP_KIND | EP_CTR_TX | EP_EA)

/* EP_STAT_TX: TX Status */
#define TX_DISABLE 0x0000 /* Disabled */
#define TX_STALL 0x0010   /* Stalled */
#define TX_NAK 0x0020     /* NAKed */
#define TX_VALID 0x0030   /* Valid */

/* EP_STAT_RX: RX Status */
#define RX_DISABLE 0x0000 /* Disabled */
#define RX_STALL 0x1000   /* Stalled */
#define RX_NAK 0x2000     /* NAKed */
#define RX_VALID 0x3000   /* Valid */

/* EP_TYPE: EndPoint Types */
#define EP_BULK 0x0000        /* BULK EndPoint */
#define EP_CONTROL 0x0200     /* CONTROL EndPoint */
#define EP_ISOCHRONOUS 0x0400 /* ISOCHRONOUS EndPoint */
#define EP_INTERRUPT 0x0600   /* INTERRUPT EndPoint */

/* bmRequestType.Type */
#define REQUEST_STANDARD 0
#define REQUEST_CLASS 1
#define REQUEST_VENDOR 2
#define REQUEST_RESERVED 3

/* USB Standard Request Codes */
#define USB_REQUEST_GET_STATUS 0
#define USB_REQUEST_CLEAR_FEATURE 1
#define USB_REQUEST_SET_FEATURE 3
#define USB_REQUEST_SET_ADDRESS 5
#define USB_REQUEST_GET_DESCRIPTOR 6
#define USB_REQUEST_SET_DESCRIPTOR 7
#define USB_REQUEST_GET_CONFIGURATION 8
#define USB_REQUEST_SET_CONFIGURATION 9
#define USB_REQUEST_GET_INTERFACE 10
#define USB_REQUEST_SET_INTERFACE 11
#define USB_REQUEST_SYNC_FRAME 12

/* USB Descriptor Types */
#define USB_DEVICE_DESC_TYPE 1
#define USB_CFG_DESC_TYPE 2
#define USB_STR_DESC_TYPE 3
#define USB_IFACE_DESC_TYPE 4
#define USB_EP_DESC_TYPE 5
#define USB_DEVICE_QR_DESC_TYPE 6
#define USB_OSPEED_CFG_DESC_TYPE 7
#define USB_IFACE_PWR_DESC_TYPE 8
/* USB Device Classes */
#define USB_RESERVED 0x00
#define USB_AUDIO 0x01
#define USB_COMM 0x02
#define USB_HID 0x03
#define USB_MONITOR 0x04
#define USB_PHYSIC 0x05
#define USB_POWER 0x06
#define USB_PRINTER 0x07
#define USB_STORAGE 0x08
#define USB_HUB 0x09
#define USB_VENDOR_SPEC 0xFF
/* Interface Class SubClass Codes */
#define USB_ACM_COMM 0x02

#define CDC_DATA_IFACE 0x0A
#define CS_INTERFACE 0x24
#define CS_ENDPOINT 0x25

/* CDC */
#define USB_CDC_CONFIG_DESC_SIZ 67
#define CDC_CMD_EP 0x81 /* EP2 for CDC commands */
#define CDC_IN_EP 0x82  /* EP1 for data IN */
#define CDC_OUT_EP 0x02 /* EP1 for data OUT */

#define USB_DEVICE_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND \
    (0x00) /*!< The CDC class request code for SEND_ENCAPSULATED_COMMAND. */
#define USB_DEVICE_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE \
    (0x01)                                               /*!< The CDC class request code for GET_ENCAPSULATED_RESPONSE. */
#define USB_DEVICE_CDC_REQUEST_SET_COMM_FEATURE (0x02)   /*!< The CDC class request code for SET_COMM_FEATURE. */
#define USB_DEVICE_CDC_REQUEST_GET_COMM_FEATURE (0x03)   /*!< The CDC class request code for GET_COMM_FEATURE. */
#define USB_DEVICE_CDC_REQUEST_CLEAR_COMM_FEATURE (0x04) /*!< The CDC class request code for CLEAR_COMM_FEATURE. */
#define USB_DEVICE_CDC_REQUEST_SET_AUX_LINE_STATE (0x10) /*!< The CDC class request code for SET_AUX_LINE_STATE. */
#define USB_DEVICE_CDC_REQUEST_SET_HOOK_STATE (0x11)     /*!< The CDC class request code for SET_HOOK_STATE. */
#define USB_DEVICE_CDC_REQUEST_PULSE_SETUP (0x12)        /*!< The CDC class request code for PULSE_SETUP. */
#define USB_DEVICE_CDC_REQUEST_SEND_PULSE (0x13)         /*!< The CDC class request code for SEND_PULSE. */
#define USB_DEVICE_CDC_REQUEST_SET_PULSE_TIME (0x14)     /*!< The CDC class request code for SET_PULSE_TIME. */
#define USB_DEVICE_CDC_REQUEST_RING_AUX_JACK (0x15)      /*!< The CDC class request code for RING_AUX_JACK. */
#define USB_DEVICE_CDC_REQUEST_SET_LINE_CODING (0x20)    /*!< The CDC class request code for SET_LINE_CODING. */
#define USB_DEVICE_CDC_REQUEST_GET_LINE_CODING (0x21)    /*!< The CDC class request code for GET_LINE_CODING. */
#define USB_DEVICE_CDC_REQUEST_SET_CONTROL_LINE_STATE \
    (0x22)                                                /*!< The CDC class request code for SET_CONTROL_LINE_STATE. */
#define USB_DEVICE_CDC_REQUEST_SEND_BREAK (0x23)          /*!< The CDC class request code for SEND_BREAK. */
#define USB_DEVICE_CDC_REQUEST_SET_RINGER_PARAMS (0x30)   /*!< The CDC class request code for SET_RINGER_PARAMS. */
#define USB_DEVICE_CDC_REQUEST_GET_RINGER_PARAMS (0x31)   /*!< The CDC class request code for GET_RINGER_PARAMS. */
#define USB_DEVICE_CDC_REQUEST_SET_OPERATION_PARAM (0x32) /*!< The CDC class request code for SET_OPERATION_PARAM. */
#define USB_DEVICE_CDC_REQUEST_GET_OPERATION_PARAM (0x33) /*!< The CDC class request code for GET_OPERATION_PARAM. */
#define USB_DEVICE_CDC_REQUEST_SET_LINE_PARAMS (0x34)     /*!< The CDC class request code for SET_LINE_PARAMS. */
#define USB_DEVICE_CDC_REQUEST_GET_LINE_PARAMS (0x35)     /*!< The CDC class request code for GET_LINE_PARAMS. */
#define USB_DEVICE_CDC_REQUEST_DIAL_DIGITS (0x36)         /*!< The CDC class request code for DIAL_DIGITS. */
#define USB_DEVICE_CDC_REQUEST_SET_UNIT_PARAMETER (0x37)  /*!< The CDC class request code for SET_UNIT_PARAMETER. */
#define USB_DEVICE_CDC_REQUEST_GET_UNIT_PARAMETER (0x38)  /*!< The CDC class request code for GET_UNIT_PARAMETER. */
#define USB_DEVICE_CDC_REQUEST_CLEAR_UNIT_PARAMETER \
    (0x39) /*!< The CDC class request code for CLEAR_UNIT_PARAMETER. */
#define USB_DEVICE_CDC_REQUEST_SET_ETHERNET_MULTICAST_FILTERS \
    (0x40) /*!< The CDC class request code for SET_ETHERNET_MULTICAST_FILTERS. */
#define USB_DEVICE_CDC_REQUEST_SET_ETHERNET_POW_PATTER_FILTER \
    (0x41) /*!< The CDC class request code for SET_ETHERNET_POW_PATTER_FILTER. */
#define USB_DEVICE_CDC_REQUEST_GET_ETHERNET_POW_PATTER_FILTER \
    (0x42) /*!< The CDC class request code for GET_ETHERNET_POW_PATTER_FILTER. */
#define USB_DEVICE_CDC_REQUEST_SET_ETHERNET_PACKET_FILTER \
    (0x43) /*!< The CDC class request code for SET_ETHERNET_PACKET_FILTER. */
#define USB_DEVICE_CDC_REQUEST_GET_ETHERNET_STATISTIC \
    (0x44)                                                /*!< The CDC class request code for GET_ETHERNET_STATISTIC. */
#define USB_DEVICE_CDC_REQUEST_SET_ATM_DATA_FORMAT (0x50) /*!< The CDC class request code for SET_ATM_DATA_FORMAT. */
#define USB_DEVICE_CDC_REQUEST_GET_ATM_DEVICE_STATISTICS \
    (0x51)                                               /*!< The CDC class request code for GET_ATM_DEVICE_STATISTICS. */
#define USB_DEVICE_CDC_REQUEST_SET_ATM_DEFAULT_VC (0x52) /*!< The CDC class request code for SET_ATM_DEFAULT_VC. */
#define USB_DEVICE_CDC_REQUEST_GET_ATM_VC_STATISTICS \
    (0x53) /*!< The CDC class request code for GET_ATM_VC_STATISTICS. */
#define USB_DEVICE_CDC_REQUEST_MDLM_SPECIFIC_REQUESTS_MASK \
    (0x7F) /*!< The CDC class request code for MDLM_SPECIFIC_REQUESTS_MASK. */

#define RXCNT(bsize, nblock) (uint16_t)(((bsize & 1) << 15) | ((nblock / 2 & 0x1F) << 10))
#define LOBYTE(x) ((uint8_t)(x & 0x00FF))
#define HIBYTE(x) ((uint8_t)((x & 0xFF00) >> 8))

#define LOG_LENGTH 50
#define LOG_DATA_LENGTH 10
#define LOG_OP_RESET 1
#define LOG_OP_GET_DESC_RX 2
#define LOG_OP_GET_DESC_TX 3
#define LOG_OP_GET_STATUS_TX 4
#define LOG_OP_SET_ADDRESS_RX 5
#define LOG_OP_GET_CLASS_DATA 6

#define _USB_STRING_(name, ws)                  \
    \
const struct name \
{                          \
        uint8_t  bLength;                       \
        uint8_t  bDescriptorType;               \
        uint16_t bString[(sizeof(ws) - 2) / 2]; \
    \
} \
name __attribute__((used, section("usb_string"))) = {sizeof(name), 0x03, ws};

#define _USB_LANG_ID_(lng_id)     \
    \
const struct wLANGID \
{         \
        uint8_t  bLength;         \
        uint8_t  bDescriptorType; \
        uint16_t bString[1];      \
    \
} \
wLANGID __attribute__((used, section("usb_string"))) = {0x04, 0x03, lng_id};

#pragma pack(push, 1)

typedef struct _USB_STRING_DESCRIPTOR_ {
    uint8_t bLength;
    uint8_t bDescriptorType;
} USB_STR_DESCRIPTOR;

typedef struct _USB_DEVICE_DESCRIPTOR_ {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
} USB_DEVICE_DESCRIPTOR;

typedef struct {
    uint32_t EPR[8];
    uint32_t RESERVED[8];
    uint32_t CNTR;
    uint32_t ISTR;
    uint32_t FNR;
    uint32_t DADDR;
    uint32_t BTABLE;
} USB_TypeDef;

typedef struct {
    uint16_t Value;
    uint16_t _res;
} USBLIB_PBElement;

typedef struct {
    USBLIB_PBElement TX_Address;
    USBLIB_PBElement TX_Count;
    USBLIB_PBElement RX_Address;
    USBLIB_PBElement RX_Count;
} USBLIB_EPBuf;

typedef struct {
    uint16_t  Number;       // EP number
    uint16_t  Type;         // EP Type
    uint8_t   TX_Max;       // Max TX EP Buffer
    uint8_t   RX_Max;       // Max RT EP Buffer
    uint16_t *pTX_BUFF;     // TX Buffer pointer
    uint32_t  lTX;          // TX Data length
    uint16_t *pRX_BUFF;     // RX Buffer pointer
    uint32_t  lRX;          // RX Data length
    uint8_t   TX_BUFF_SIZE; // Max length of pTX_BUFF buf, NOT a EP Buf!!!
    uint32_t  lRX_PMA;      // ?

} USBLIB_EPData;

typedef struct
{
    uint8_t Recipient : 5;
    uint8_t Type : 2;
    uint8_t Dir : 1;
} USBLIB_RequestType;

typedef struct
{
    uint8_t L : 8;
    uint8_t H : 8;
} USBLIB_WByte;

typedef struct
{
    USBLIB_RequestType bmRequestType;
    uint8_t            bRequest;
    USBLIB_WByte       wValue;
    USBLIB_WByte       wIndex;
    uint8_t            wLength;
} USBLIB_SetupPacket;

typedef struct
{
    uint8_t EPn;
    uint8_t Operation;
    uint8_t Data[LOG_DATA_LENGTH];
    uint8_t Length;
} USBLIB_Log;

typedef struct {
    uint8_t   Size;
    uint8_t   DescriptorType;
    uint16_t *String;
} USBLIB_StringDesc;

typedef struct {
    uint32_t baudRate;
    uint8_t  charFormat;
    uint8_t  parityType;
    uint8_t  dataBits;
} USBLIB_LineCoding;

#pragma pack(pop)

void USBLIB_Init(void);
void USBLIB_EP_Init(void);
//void USBLIB_HandleStatus(void);
void USBLIB_ResetUSB(void);
void USBLIB_Reset(void);
void USBLIB_SetEPTable(uint8_t EP, uint32_t TXAddress, uint32_t TXCount, uint32_t RXAddress, uint32_t RXCount);
void USBLIB_LogCheckForTransmit(void);
void USBLIB_Transmit(uint16_t *Data, uint16_t Length, uint16_t Timeout);

__weak void uUSBLIB_DataReceivedHandler(uint16_t *Data, uint16_t Length);
void uUSBLIB_LineStateHandler(USBLIB_WByte LineState);

#endif
