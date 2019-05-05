////////////////////////////////////////////////////////////////////////////////
///
/// @file       rx.c
///
/// @project
///
/// @brief      Network reception routines
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2019, Evan Lojewski
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
/// 3. Neither the name of the copyright holder nor the
/// names of its contributors may be used to endorse or promote products
/// derived from this software without specific prior written permission.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////


#include <Network.h>
#include <Ethernet.h>
#include <APE_APE.h>
#include <APE_APE_PERI.h>
#include <APE_RX_PORT.h>
#include <types.h>

#include <stdbool.h>

#ifdef CXX_SIMULATOR
#include <stdio.h>
#endif

bool Network_RxLePatcket(uint32_t* buffer, uint32_t* bytes)
{
    RegAPERxbufoffsetFunc0_t rxbuf;
    rxbuf = APE.RxbufoffsetFunc0;
    if((int)rxbuf.bits.Valid)
    {
        union {
            uint32_t r32;
            struct {
                uint32_t payload_length:7;
                uint32_t next_block:23;
                uint32_t first:1;
                uint32_t not_last:1;
            } bits;
        } control;
        int count = rxbuf.bits.Count;
        // int tailid = rxbuf.bits.Tail;
        int blockid = rxbuf.bits.Head;
        // printf("Valid packet at offset %x\n", blockid);
        uint32_t buffer_pos = 0;

        do
        {
            // printf("Block at %x\n", blockid);
            RegRX_PORTIn_t* block = (RegRX_PORTIn_t*)&RX_PORT.In[RX_PORT_IN_ALL_BLOCK_WORDS * blockid];
            // printf("Control %x\n", (uint32_t)block[0].r32);
            control.r32 = block[0].r32;
            // printf(" Payload Len %d\n", control.bits.payload_length);
            // printf(" Next Block %d\n", control.bits.next_block);
            // printf(" First %d\n", control.bits.first);
            // printf(" Not Last %d\n", control.bits.not_last);
            int32_t words = (control.bits.payload_length + 3)/4;
            int32_t offset;
            if(control.bits.first)
            {
                offset = RX_PORT_IN_ALL_FIRST_PAYLOAD_WORD;
            }
            else
            {
                offset = RX_PORT_IN_ALL_ADDITIONAL_PAYLOAD_WORD;
            }
            // printf("Using offset %d\n", offset);
            for(int i = 0; i < words; i++)
            {
                uint32_t data = block[i + offset].r32;
                buffer[buffer_pos++] = data;
                // printf(" word %d: 0x%08X\n", i, data);
            }

            blockid = control.bits.next_block;
            count--;
        } while(count);

        // Transmit to NC
        // disableNCSIHandling();
        // enableNCSIHandling();

        RegAPERxPoolRetire0_t retire;
        retire.r32 = (1 << 24);
        retire.bits.Head = rxbuf.bits.Head;
        retire.bits.Tail = rxbuf.bits.Tail;
        retire.bits.Count = rxbuf.bits.Count;
        // retire.print();


        APE.RxPoolRetire0 = retire;
        rxbuf.r32 |= (1 << 31);
        // rxbuf.print();
        APE.RxbufoffsetFunc0 = rxbuf;
        *bytes = 4 * buffer_pos;

        return true;
    }
    else
    {
        return false;
    }
}

bool Network_PassthroughRxPatcket(void)
{
    RegAPERxbufoffsetFunc0_t rxbuf;
    rxbuf = APE.RxbufoffsetFunc0;
    if((int)rxbuf.bits.Valid)
    {
        union {
            uint32_t r32;
            struct {
                uint32_t payload_length:7;
                uint32_t next_block:23;
                uint32_t first:1;
                uint32_t not_last:1;
            } bits;
        } control;
        int count = rxbuf.bits.Count;
        // int tailid = rxbuf.bits.Tail;
        int blockid = rxbuf.bits.Head;

        do
        {
            count--;
            // printf("Block at %x\n", blockid);
            RegRX_PORTIn_t* block = (RegRX_PORTIn_t*)&RX_PORT.In[RX_PORT_IN_ALL_BLOCK_WORDS * blockid];
            // printf("Control %x\n", (uint32_t)block[0].r32);
            control.r32 = block[0].r32;
            // printf(" Payload Len %d\n", control.bits.payload_length);
            // printf(" Next Block %d\n", control.bits.next_block);
            // printf(" First %d\n", control.bits.first);
            // printf(" Not Last %d\n", control.bits.not_last);
            int32_t words = (control.bits.payload_length + 3)/4;
            int32_t offset;
            if(control.bits.first)
            {
                offset = RX_PORT_IN_ALL_FIRST_PAYLOAD_WORD;
            }
            else
            {
                offset = RX_PORT_IN_ALL_ADDITIONAL_PAYLOAD_WORD;
            }

            // Wait for enough free space.
            while(APE_PERI.BmcToNcTxStatus.bits.InFifo < words);


            int i;
            for(i = 0; i < words-1; i++)
            {
                uint32_t data = block[i + offset].r32;
                APE_PERI.BmcToNcTxBuffer.r32 = data;
            }

            if(count) // could use !control.bits.not_last
            {
                uint32_t data = block[i + offset].r32;
                APE_PERI.BmcToNcTxBuffer.r32 = data;
            }
            else
            {
                uint32_t data = block[i + offset].r32;
                // Last word to send.
                APE_PERI.BmcToNcTxControl.r32 = 0; // full word.
                APE_PERI.BmcToNcTxBufferLast.r32 = data;
            }

            // Retire this block.
            RegAPERxPoolRetire0_t retire;
            retire.r32 = (1 << 24);
            retire.bits.Head = blockid;
            retire.bits.Tail = blockid;
            retire.bits.Count = 1;
            APE.RxPoolRetire0 = retire;


            blockid = control.bits.next_block;
        } while(count);

        // Mark the register as read.
        rxbuf.r32 |= (1 << 31);
        APE.RxbufoffsetFunc0 = rxbuf;

        return true;
    }
    else
    {
        return false;
    }
}