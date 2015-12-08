/* Copyright (C) 2015  Adam Green     (https://github.com/adamgreen)                                                                                                
   Copyright (C) 2015  Chang,Jia-Rung (https://github.com/JaredCJR)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include <mbed.h>
#include <CrashCatcher.h>
#include <stdint.h>
#include "MyImplementationIO/usart"

extern "C" void testInitFPURegisters();
static void enable8ByteStackAlignment();
static void crashWithFPUDisabled();
DigitalOut myled(LED1);

int main()
{
	USART3_INIT();

    enable8ByteStackAlignment();

    /*
     * Blink LED with gcc4mbed libraries for a while ,then Crash with FPU disabled but try to initialize the FPU.
     */
    while (1)
    {
		for(int i = 0;i < 5;i++)
		{
			wait(0.5);
			myled = 1;
			wait(0.5);
			myled = 0;
		}

        /*Fault with FPU disabled*/
        crashWithFPUDisabled();
    }
}

static void enable8ByteStackAlignment()
{
    SCB->CCR |= SCB_CCR_STKALIGN_Msk;
}


static void disableFPU()
{
    static const uint32_t FPCA = 1 << 2;
    SCB->CPACR &= ~(0xF << 20);
    __set_CONTROL(__get_CONTROL() & ~FPCA);
}

static void crashWithFPUDisabled()
{
    disableFPU();
    __disable_irq();
    testInitFPURegisters();
    CRASH_CATCHER_READ_FAULT();
}


/* Let CrashCatcher know what RAM contents should be part of crash dump.
 * The last "regions" must end with "{0xFFFFFFFF, 0xFFFFFFFF}"
 */
extern "C" const CrashCatcherMemoryRegion* CrashCatcher_GetMemoryRegions(void)
{
    static const CrashCatcherMemoryRegion regions[] = { 
        //STM32F429i-Discovery
                                                        {0x20000000, 0x2001C000, CRASH_CATCHER_BYTE},
                                                        {0x2001C000, 0x20020000, CRASH_CATCHER_BYTE},
                                                        {0x20020000, 0x20030000, CRASH_CATCHER_BYTE},
                                                        {0xFFFFFFFF, 0xFFFFFFFF, CRASH_CATCHER_BYTE} 
                                                      };
    return regions;
}

extern "C" int CrashCatcher_getc(void)
{
    return USART3_ReceiveChar();
}

extern "C" void CrashCatcher_putc(int c)
{
    USART3_SendChar(c);
} 
