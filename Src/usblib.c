#include "usblib.h"
#include "stm32f10x.h"
#include <stdlib.h>
#include <string.h>

volatile USB_TypeDef *USB = (USB_TypeDef *)USB_BASE;
volatile USBLIB_EPBuf EPBufTable[EPCOUNT] __attribute__((at(USB_PBUFFER)));
volatile uint32_t     USBEP[EPCOUNT] __attribute__((at(USB_BASE)));
USBLIB_SetupPacket *  SetupPacket;
volatile USBLIB_Log   Log[LOG_LENGTH];
volatile uint8_t      LogIdx = 0, LogPassIdx = 1;
volatile uint8_t      DeviceAddress = 0;

USBLIB_EPData EpData[EPCOUNT] =
    {
        {0, EP_CONTROL, 8, 8, 0, 0, 0, 0, 64, 0},
        {1, EP_INTERRUPT, 16, 16, 0, 0, 0, 0, 64, 0},
        {2, EP_BULK, 64, 64, 0, 0, 0, 0, 64, 0},
        {3, EP_BULK, 64, 64, 0, 0, 0, 0, 64, 0}};

void USBLIB_Init(void)
{
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    RCC->APB1ENR = RCC_APB1ENR_USBEN;

    USB->CNTR   = USB_CNTR_FRES; /* Force USB Reset */
    USB->BTABLE = 0;
    USB->DADDR  = 0;
    USB->ISTR   = 0;
    USB->CNTR   = USB_CNTR_RESETM;
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

USBLIB_LineCoding lineCoding = {0,0,0,0};

const uint8_t USB_DEVICE_DESC[] =
    {
        (uint8_t)18,                        //    bLength
        (uint8_t)USB_DEVICE_DESC_TYPE,      //    bDescriptorType
        (uint8_t)0x00,                      //    bcdUSB
        (uint8_t)0x02,                      //    bcdUSB
        (uint8_t)USB_COMM,                  //    bDeviceClass
        (uint8_t)0,                         //    bDeviceSubClass
        (uint8_t)0,                         //    bDeviceProtocol
        (uint8_t)8,                         //    bMaxPacketSize0
        (uint8_t)LOBYTE(DEVICE_VENDOR_ID),  //    idVendor
        (uint8_t)HIBYTE(DEVICE_VENDOR_ID),  //    idVendor
        (uint8_t)LOBYTE(DEVICE_PRODUCT_ID), //    idProduct
        (uint8_t)HIBYTE(DEVICE_PRODUCT_ID), //    idProduct
        (uint8_t)0x00,                      //    bcdDevice
        (uint8_t)0x01,                      //    bcdDevice
        (uint8_t)1,                         //    iManufacturer
        (uint8_t)2,                         //    iProduct
        (uint8_t)3,                         //    iSerialNumbert
        (uint8_t)1                          //    bNumConfigurations
};
const uint8_t USBD_CDC_CFG_DESCRIPTOR[] =
    {
        /*Configuration Descriptor*/
        0x09,                    /* bLength: Configuration Descriptor size */
        USB_CFG_DESC_TYPE,       /* bDescriptorType: Configuration */
        USB_CDC_CONFIG_DESC_SIZ, /* wTotalLength:no of returned bytes */
        0x00,
        0x02, /* bNumInterfaces: 2 interface */
        0x01, /* bConfigurationValue: Configuration value */
        0x00, /* iConfiguration: Index of string descriptor describing the configuration */
        0x80, /* bmAttributes - Bus powered */
        0x32, /* MaxPower 100 mA */

        /*---------------------------------------------------------------------------*/

        /*Interface Descriptor */
        0x09,                /* bLength: Interface Descriptor size */
        USB_IFACE_DESC_TYPE, /* bDescriptorType: Interface */
        0x00, /* bInterfaceNumber: Number of Interface */
        0x00, /* bAlternateSetting: Alternate setting */
        0x01, /* bNumEndpoints: One endpoints used */
        0x02, /* bInterfaceClass: Communication Interface Class */
        0x02, /* bInterfaceSubClass: Abstract Control Model */
        0x01, /* bInterfaceProtocol: Common AT commands */
        0x00, /* iInterface: */

        /*Header Functional Descriptor*/
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,

        /*Call Management Functional Descriptor*/
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x01, /* bDescriptorSubtype: Call Management Func Desc */
        0x00, /* bmCapabilities: D0+D1 */
        0x01, /* bDataInterface: 1 */

        /*ACM Functional Descriptor*/
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x00, /* bmCapabilities */

        /*Union Functional Descriptor*/
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x06, /* bDescriptorSubtype: Union func desc */
        0x00, /* bMasterInterface: Communication class interface */
        0x01, /* bSlaveInterface0: Data Class Interface */


        /*Endpoint 2 Descriptor*/
        0x07,                        /* bLength: Endpoint Descriptor size */
        USB_EP_DESC_TYPE,            /* bDescriptorType: Endpoint */
        CDC_CMD_EP,                  /* bEndpointAddress */
        0x03,                        /* bmAttributes: Interrupt */
        LOBYTE(CDC_DATA_FS_CMD_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_CMD_PACKET_SIZE),
        0x10, /* bInterval: */
        /*---------------------------------------------------------------------------*/

        /*Data class interface descriptor*/
        0x09,                /* bLength: Endpoint Descriptor size */
        USB_IFACE_DESC_TYPE, /* bDescriptorType: */
        0x01,                /* bInterfaceNumber: Number of Interface */
        0x00,                /* bAlternateSetting: Alternate setting */
        0x02,                /* bNumEndpoints: Two endpoints used */
        0x0A,                /* bInterfaceClass: CDC */
        0x00,                /* bInterfaceSubClass: */
        0x00,                /* bInterfaceProtocol: */
        0x00,                /* iInterface: */

        /*Endpoint OUT Descriptor*/
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_EP_DESC_TYPE,                    /* bDescriptorType: Endpoint */
        CDC_OUT_EP,                          /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval: ignore for Bulk transfer */

        /*Endpoint IN Descriptor*/
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_EP_DESC_TYPE,                    /* bDescriptorType: Endpoint */
        CDC_IN_EP,                           /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00 /* bInterval: ignore for Bulk transfer */
};

/* USB String Descriptors */
_USB_LANG_ID_(LANG_US);
_USB_STRING_(wsVendor, L"SaeWave.com")
_USB_STRING_(wsProd, L"RemoteSwitch HUB")
_USB_STRING_(wsSN, L"0123-4567-89")
_USB_STRING_(wsCDC, L"CDC Device")
_USB_STRING_(wsCDCData, L"CDC Data")

void USBLIB_AddToLogArr(uint8_t Operation, uint8_t EPn, uint8_t *Data, uint8_t Length)
{
    if (LogPassIdx > 0) {
        LogPassIdx--;
        return;
    }
    Log[LogIdx].EPn       = EPn;
    Log[LogIdx].Operation = Operation;
    Log[LogIdx].Length    = Length;
    memcpy((uint8_t *)Log[LogIdx].Data, Data, Length <= LOG_DATA_LENGTH ? Length : LOG_DATA_LENGTH);
    if (LogIdx < LOG_LENGTH)
        LogIdx++;
}

void USBLIB_Reset(void)
{
    /* *********** WARNING ********** */
    /* We DO NOT CHANGE BTABLE!! So we asume that buffer table start from address 0!!! */
    USB->ISTR   = 0x00;
    USB->CNTR   = USB_CNTR_CTRM | USB_CNTR_RESETM;
    USB->BTABLE = 0x00;

    uint16_t Addr = sizeof(EPBufTable);
    for (uint8_t i = 0; i < EPCOUNT; i++) {
        EPBufTable[i].TX_Address.Value = Addr;
        EPBufTable[i].TX_Count.Value   = 0;
        Addr += EpData[i].TX_Max;
        EPBufTable[i].RX_Address.Value = Addr;
        if (EpData[i].RX_Max > 62)
            EPBufTable[i].RX_Count.Value = 0x8000 | ((EpData[i].RX_Max / 2) << 10);
        else
            EPBufTable[i].RX_Count.Value = ((EpData[i].RX_Max / 2) << 10);

        Addr += EpData[i].RX_Max;

        //        if (!EpData[i].pTX_BUFF)
        //            EpData[i].pTX_BUFF =(uint16_t*)malloc(EpData[i].TX_BUFF_SIZE);
        if (!EpData[i].pRX_BUFF)
            EpData[i].pRX_BUFF = (uint16_t *)malloc(EpData[i].RX_Max);

        USB->EPR[i] = (EpData[i].Number | EpData[i].Type | RX_VALID | TX_NAK);
    }

    for (uint8_t i = EPCOUNT; i < 8; i++) {
        USB->EPR[i] = i | RX_NAK | TX_NAK;
    }
    USB->ISTR   = 0x00;
    USB->CNTR   = USB_CNTR_CTRM | USB_CNTR_RESETM;
    USB->BTABLE = 0x00;
    USB->DADDR  = USB_DADDR_EF;
}

void USBLIB_setStatTx(uint8_t EPn, uint16_t Stat)
{
    register uint16_t val = USB->EPR[EPn];
    USB->EPR[EPn]         = (val ^ (Stat & EP_STAT_TX)) & (EP_MASK | EP_STAT_TX);
}

void USBLIB_setStatRx(uint8_t EPn, uint16_t Stat)
{
    register uint16_t val = USB->EPR[EPn];
    USB->EPR[EPn]         = (val ^ (Stat & EP_STAT_RX)) & (EP_MASK | EP_STAT_RX);
}

void USBLIB_Pma2EPBuf2(uint8_t EPn)
{
    uint8_t   Count = EpData[EPn].lRX = (EPBufTable[EPn].RX_Count.Value & 0x3FF) >> 1;
    uint32_t *Address                 = (uint32_t *)(USB_PBUFFER + EPBufTable[EPn].RX_Address.Value * 2);
    uint16_t *Distination             = (uint16_t *)EpData[EPn].pRX_BUFF;
    for (uint8_t i = 0; i < Count; i++) {
        *(uint16_t *)Distination = *(uint16_t *)Address;
        Distination++;
        Address++;
    }
}

void USBLIB_EPBuf2Pma(uint8_t EPn)
{
    uint32_t *Distination;
    uint8_t Count;

    Count                          = EpData[EPn].lTX <= EpData[EPn].TX_Max ? EpData[EPn].lTX : EpData[EPn].TX_Max;
    EPBufTable[EPn].TX_Count.Value = Count;

    Distination = (uint32_t *)(USB_PBUFFER + EPBufTable[EPn].TX_Address.Value * 2);
//    USBLIB_AddToLogArr(LOG_OP_GET_DESC_TX, EPn, (uint8_t *)EpData[EPn].pTX_BUFF, Count);
    for (uint8_t i = 0; i < (Count + 1) / 2; i++) {
        *(uint32_t *)Distination = *(uint16_t *)EpData[EPn].pTX_BUFF;
        Distination++;
        EpData[EPn].pTX_BUFF++;
    }
    EpData[EPn].lTX -= Count;
}

void USBLIB_SendData(uint8_t EPn, uint16_t *Data, uint8_t Length)
{

    EpData[EPn].lTX      = Length;
    EpData[EPn].pTX_BUFF = Data;
    if (Length > 0) {
        USBLIB_EPBuf2Pma(EPn);
    } else {
        EPBufTable[EPn].TX_Count.Value = 0;
//        USBLIB_AddToLogArr(LOG_OP_GET_DESC_TX, EPn, 0, 0);
    }
    USBLIB_setStatTx(EPn, TX_VALID);
}

void USBLIB_GetDescriptor(USBLIB_SetupPacket *SPacket)
{
    uint8_t             c;
    USB_STR_DESCRIPTOR *pSTR;
    switch (SPacket->wValue.H) {
    case USB_DEVICE_DESC_TYPE:
        USBLIB_SendData(0, (uint16_t *)&USB_DEVICE_DESC, sizeof(USB_DEVICE_DESC));
        break;

    case USB_CFG_DESC_TYPE:
        USBLIB_SendData(0, (uint16_t *)&USBD_CDC_CFG_DESCRIPTOR, sizeof(USBD_CDC_CFG_DESCRIPTOR));
        break;
    case USB_DEVICE_QR_DESC_TYPE:
        //Some strange descriptor type...
        break;

    case USB_STR_DESC_TYPE:
        pSTR = (USB_STR_DESCRIPTOR *)&wLANGID;

        for (c = 0; c < SetupPacket->wValue.L; c++) {
            pSTR = (USB_STR_DESCRIPTOR *)((uint8_t *)pSTR + pSTR->bLength);
        }
        USBLIB_SendData(0, (uint16_t *)pSTR, pSTR->bLength);
        break;
    }
}

void USBLIB_EPHandler(uint16_t Status)
{
/*    static struct lineState {
        uint8_t dtr;
        uint8_t rts;
    } lineState;
*/
    uint16_t DeviceConfigured = 0;
    uint8_t  EPn = Status & USB_ISTR_EP_ID;
    uint32_t EP  = USBEP[EPn];
    if (EP & EP_CTR_RX) { //something received
        USBEP[EPn] &= (~EP_CTR_RX & EP_MASK);
        USBLIB_Pma2EPBuf2(EPn);
//        USBLIB_AddToLogArr(LOG_OP_GET_DESC_RX, EPn, (uint8_t *)EpData[EPn].pRX_BUFF, EpData[EPn].lRX);
        if (EP && EPn == 0) { //If control endpoint
            if (EP & USB_EP0R_SETUP) {
                SetupPacket = (USBLIB_SetupPacket *)EpData[EPn].pRX_BUFF;
                if (SetupPacket->bmRequestType.Type == REQUEST_STANDARD) {
                    switch (SetupPacket->bRequest) {
                        case USB_REQUEST_SET_ADDRESS:
                            USBLIB_SendData(0, 0, 0);
                            DeviceAddress = SetupPacket->wValue.L;
                            break;
                        case USB_REQUEST_GET_DESCRIPTOR:
                            USBLIB_GetDescriptor(SetupPacket);
                            break;
                        case USB_REQUEST_GET_STATUS:
                            USBLIB_SendData(0, (uint16_t *)1, 2);
                            break;
                        case USB_REQUEST_GET_CONFIGURATION:
                            USBLIB_SendData(0, &DeviceConfigured, 1);
                            break;
                        case USB_REQUEST_SET_CONFIGURATION:
                            DeviceConfigured = 1;
                            USBLIB_SendData(0, 0, 0);
                            break;
                    }
                }
                if (SetupPacket->bmRequestType.Type == REQUEST_CLASS) {
                    USBLIB_AddToLogArr(LOG_OP_GET_DESC_RX, EPn, (uint8_t *)EpData[EPn].pRX_BUFF, EpData[EPn].lRX);
                    switch (SetupPacket->bRequest) {
                        case USB_DEVICE_CDC_REQUEST_SET_COMM_FEATURE :
                            //TODO
                            break;
                        case USB_DEVICE_CDC_REQUEST_SET_LINE_CODING:
//                            lineCoding.baudRate = (uint16_t)EpData[EPn].pRX_BUFF[0];
//                            lineCoding.charFormat = (uint8_t)EpData[EPn].pRX_BUFF[1];
//                            lineCoding.parityType = (uint8_t)EpData[EPn].pRX_BUFF[2];
//                            lineCoding.dataBits = (uint8_t)EpData[EPn].pRX_BUFF[3];
//                            USBLIB_SendData(0, 0, 0);
                            break;
                        case USB_DEVICE_CDC_REQUEST_GET_LINE_CODING:
                            USBLIB_SendData(EPn, (uint16_t*)&lineCoding, sizeof(lineCoding));
                            break;
                        case USB_DEVICE_CDC_REQUEST_SET_CONTROL_LINE_STATE:
//                            lineState.dtr = SetupPacket->wValue.H & 0x01;
//                            lineState.rts = SetupPacket->wValue.L & 0x01;
                            USBLIB_SendData(0, 0, 0);
                            break;
                    }
                }
            }
        } else {
            USBLIB_AddToLogArr(LOG_OP_GET_CLASS_DATA, EPn, (uint8_t *)EpData[EPn].pRX_BUFF, EpData[EPn].lRX);
        }
        USBLIB_setStatRx(EPn, RX_VALID);
    }
    if (EP & EP_CTR_TX) { //something transmitted
        USBEP[EPn] &= (~EP_CTR_TX & EP_MASK);

        if (DeviceAddress) {
            USB->DADDR    = DeviceAddress | 0x80;
            DeviceAddress = 0;
        }

        if (EpData[EPn].lTX) { //Have to transmit something?
            USBLIB_EPBuf2Pma(EPn);
            USBLIB_setStatTx(EPn, TX_VALID);
        }
    }
}

void USB_LP_CAN1_RX0_IRQHandler()
{
    if (USB->ISTR & USB_ISTR_SOF) {
        USB->ISTR &= ~USB_ISTR_SOF;
        // Handle SOF
    }
    if (USB->ISTR & USB_ISTR_RESET) { // Reset
        USB->ISTR &= ~USB_ISTR_RESET;
//        USBLIB_AddToLogArr(LOG_OP_RESET, 0xFF, (uint8_t *){0}, 1);
        USBLIB_Reset();
        return;
    }
    if (USB->ISTR & USB_ISTR_CTR) { //Handle data on EP
        USBLIB_EPHandler((uint16_t)USB->ISTR);
        return;
    }
    if (USB->ISTR & USB_ISTR_PMAOVR) {
        USB->ISTR &= ~USB_ISTR_PMAOVR;
        // Handle PMAOVR status
        return;
    }

    if (USB->ISTR & USB_ISTR_ERR) {
        USB->ISTR &= ~USB_ISTR_ERR;
        // Handle Error
        return;
    }
    if (USB->ISTR & USB_ISTR_WKUP) {
        USB->ISTR &= ~USB_ISTR_WKUP;
        // Handle Wakeup
        return;
    }
    if (USB->ISTR & USB_ISTR_ESOF) {
        USB->ISTR &= ~USB_ISTR_ESOF;
        // Handle ESOF
        return;
    }
    USB->ISTR = 0;
}
