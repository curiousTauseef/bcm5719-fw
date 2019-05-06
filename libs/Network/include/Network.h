////////////////////////////////////////////////////////////////////////////////
///
/// @file       Network.h
///
/// @project
///
/// @brief      Network TX/RX Support Routines
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

#ifndef NETWORK_H
#define NETWORK_H

#include <APE_APE_PERI.h>
#include <stdbool.h>
#include <stdint.h>

void Network_InitTxRx(void);

uint32_t Network_TX_numBlocksNeeded(uint32_t frame_size);
int32_t Network_TX_allocateBlock(void);

void Network_TX_transmitBePacket(uint8_t *packet, uint32_t length);
void Network_TX_transmitLePacket(uint8_t *packet, uint32_t length);

void Network_TX_transmitPassthroughPacket(uint32_t length);

// void Network_TX_transmitPassthroughPacket(RegAPE_PERIBmcToNcRxStatus_t
// rx_status);

bool Network_RxLePatcket(uint32_t *buffer, uint32_t *length);
bool Network_PassthroughRxPatcket(void);

void Network_SetMACAddr(uint16_t high, uint32_t low, uint32_t index,
                        bool enabled);

#endif /* NETWORK_H */