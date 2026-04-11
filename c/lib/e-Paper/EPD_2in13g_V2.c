/*****************************************************************************
* | File      	:   EPD_2in13g_V2.c
* | Author      :   Waveshare team
* | Function    :   2inch13 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-10-21
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_2in13g_V2.h"
#include "Debug.h" 

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN13G_V2_Reset(void)
{
    DEV_Digital_Write(EPD_PWR_PIN, 1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_PWR_PIN, 0);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);  // Short pulse - critical for HAT power-off switch circuit
    DEV_Digital_Write(EPD_PWR_PIN, 1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(10);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN13G_V2_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);   
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_2IN13G_V2_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes HIGH
parameter:
******************************************************************************/
void EPD_2IN13G_V2_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    printf("Debug: Waiting for BUSY pin (GPIO24) to go HIGH...\r\n");
    DEV_Delay_ms(100);
    
    int timeout = 0;
    const int MAX_TIMEOUT = 300;  // 30 seconds max (300 * 100ms)
    UBYTE busy_val;
    
    while(1) {
        busy_val = DEV_Digital_Read(EPD_BUSY_PIN);
        if (busy_val == 1) {
            break;  // Display is idle
        }
        
        DEV_Delay_ms(100);
        timeout++;
        
        // Print status every 10 iterations (1 second)
        if (timeout % 10 == 0) {
            printf("Debug: BUSY still LOW after %d seconds...\r\n", timeout / 10);
        }
        
        if (timeout > MAX_TIMEOUT) {
            printf("\r\nERROR: BUSY timeout! Display stuck in busy state after 30 seconds.\r\n");
            printf("BUSY pin value = %d (expected: 1 for idle)\r\n", busy_val);
            printf("Possible causes:\r\n");
            printf("  1. Display not powered on - check HAT connection\r\n");
            printf("  2. SPI communication failure - check wiring\r\n");
            printf("  3. Display hardware fault\r\n");
            break;  // Exit to prevent infinite hang
        }
    }
    
    if (timeout <= MAX_TIMEOUT) {
        printf("Debug: BUSY pin is now HIGH (idle) after %d ms\r\n", timeout * 100);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN13G_V2_TurnOnDisplay(void)
{
    EPD_2IN13G_V2_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_2IN13G_V2_SendData(0x00);
    EPD_2IN13G_V2_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN13G_V2_Init(void)
{
    EPD_2IN13G_V2_Reset();
    EPD_2IN13G_V2_ReadBusy();

    EPD_2IN13G_V2_SendCommand(0x61); //TRES
    EPD_2IN13G_V2_SendData(0x00);		// EPD_2IN13G_WIDTH_H
    EPD_2IN13G_V2_SendData(0x7C);		// EPD_2IN13G_WIDTH_L
    EPD_2IN13G_V2_SendData(0x00);		// EPD_2IN13G_HEIGHT_H
    EPD_2IN13G_V2_SendData(0xFA); 		// EPD_2IN13G_HEIGHT_L

    EPD_2IN13G_V2_SendCommand(0xE9);
    EPD_2IN13G_V2_SendData(0x01); 

    EPD_2IN13G_V2_SendCommand(0x04);
    EPD_2IN13G_V2_ReadBusy(); 
}


void EPD_2IN13G_V2_Init_Fast(void)
{
    EPD_2IN13G_V2_Reset();
    EPD_2IN13G_V2_ReadBusy();

    EPD_2IN13G_V2_SendCommand(0x61); //TRES
    EPD_2IN13G_V2_SendData(0x00);		// EPD_2IN13G_WIDTH_H
    EPD_2IN13G_V2_SendData(0x7C);		// EPD_2IN13G_WIDTH_L
    EPD_2IN13G_V2_SendData(0x00);		// EPD_2IN13G_HEIGHT_H
    EPD_2IN13G_V2_SendData(0xFA); 		// EPD_2IN13G_HEIGHT_L
    
    EPD_2IN13G_V2_SendCommand(0xe0);
    EPD_2IN13G_V2_SendData(0x02);

    EPD_2IN13G_V2_SendCommand(0xe6);	
    EPD_2IN13G_V2_SendData(90);

    EPD_2IN13G_V2_SendCommand(0xa5);
    EPD_2IN13G_V2_ReadBusy(); 

    EPD_2IN13G_V2_SendCommand(0xE9);
    EPD_2IN13G_V2_SendData(0x01); 

    EPD_2IN13G_V2_SendCommand(0x04);
    EPD_2IN13G_V2_ReadBusy(); 
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN13G_V2_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_2IN13G_V2_WIDTH % 4 == 0)? (EPD_2IN13G_V2_WIDTH / 4 ): (EPD_2IN13G_V2_WIDTH / 4 + 1);
    Height = EPD_2IN13G_V2_HEIGHT;

    EPD_2IN13G_V2_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13G_V2_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_2IN13G_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13G_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13G_V2_WIDTH % 4 == 0)? (EPD_2IN13G_V2_WIDTH / 4 ): (EPD_2IN13G_V2_WIDTH / 4 + 1);
    Height = EPD_2IN13G_V2_HEIGHT;

    EPD_2IN13G_V2_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            if(i < 31 ) EPD_2IN13G_V2_SendData(Image[i + j * Width]);
            else EPD_2IN13G_V2_SendData(0x00);
        }
    }

    EPD_2IN13G_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN13G_V2_Sleep(void)
{
    EPD_2IN13G_V2_SendCommand(0x02); //power off
    EPD_2IN13G_V2_SendData(0X00);
    EPD_2IN13G_V2_ReadBusy();       //waiting for the electronic paper IC to release the idle signal
    DEV_Delay_ms(100);           //!!!The delay here is necessary,100mS at least!!! 
    
    EPD_2IN13G_V2_SendCommand(0x07);  //deep sleep
    EPD_2IN13G_V2_SendData(0XA5);
}
