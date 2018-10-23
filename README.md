# STM32F103-USB-CDC-CMSIS
STM32F103 USB CDC CMSIS provires a simple low weight firmware for STM32F1 serie to implement Virtual Com Port.
Current implementation create Virtual Com Port with next settings: 115200, 0, none, 8, but you can change these settings as you want.
## How to use
* Add `usblib.c` and `usblib.h` to your project.
* Make sure you have configured USB to 48 MHz clock and RCC to 48 MHz or more (72 MHz is recomended).
* It's strongly recomended to use 1,5K GPIO driven pull-up rezistor to have full controll of USB line initiate sequence.
* Call `USBLIB_Init();` to enable USB and make initial configuration of USB registers. Please note - `USBLIB_Init();` will trigget USB RESET sequence.
* Enable 1.5K pull-up resistor to allow host detect device connection.
  * If your device connected to Windows host use `RemoteSwitchHUB.inf` to install a default Windows driver. After installation you will see new COM port named `RemoteSwitch HUB` (If you want to change this name you need to edit  it in `usblib.c` and `inf` files).
  * If your device connected to Unix systems then you will see a new device in /dev/ dir.
* Implement ```void uUSBLIB_LineStateHandler(USBLIB_WByte LineState)``` in your code to handle line state. See `main.c` for example. 
NOTE: you can't send any data to host when port is closed! To controll when port is opened use this condition:
```c 
if (LineState.L) {      //App connected to the virtual port
    USBLIB_Transmit((uint16_t *)"Welcome to the club!\r\n", 22);
}
```
* Inplement `void uUSBLIB_DataReceivedHandler(uint16_t *Data, uint16_t Length)` in your code to accept data from host. See `main.c` f.e.

Code well tested on Windows7, MacOS 10.13 (High Sierra) and Raspbian OS.
