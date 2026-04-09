/*****************************************************************************
* | File      	:   EPD_2in13g_test.c
* | Author      :   Waveshare team
* | Function    :   2inch13 e-paper (G) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-05-29
* | Info        :
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
#include "EPD_Test.h"
#include "EPD_2in13g_V2.h"
#include "time.h"

int EPD_2in13g_V2_test(void)
{
    printf("EPD_2IN13G_V2_test Demo\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init...\n");
    EPD_2IN13G_V2_Init();

    printf("e-Paper Clear white...\n");
    EPD_2IN13G_V2_Clear(EPD_2IN13G_V2_WHITE); // White

    DEV_Delay_ms(2000);

    printf("e-Paper Clear black...\n");
    EPD_2IN13G_V2_Clear(EPD_2IN13G_V2_BLACK); // White

    DEV_Delay_ms(2000);

    printf("Clear...\n");
    EPD_2IN13G_V2_Clear(EPD_2IN13G_V2_WHITE);

    DEV_Delay_ms(2000);//important, at least 2s
    printf("close 5V, Module enters 0 power consumption ...\r\n");

    // DEV_Module_Exit();
    
    return 0;
}

