////////////////////////////////////////////////////////////////////////////////
///
/// @file       bcm5719_APE_sim.cpp
///
/// @project    bcm5719
///
/// @brief      bcm5719_APE_sim
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2018, Evan Lojewski
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
/// 3. Neither the name of the <organization> nor the
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

#include <stdint.h>
#include <utility>
#include <bcm5719_APE.h>

#ifdef __ppc64__
#define BARRIER()    do { asm volatile ("sync 0\neieio\n" ::: "memory"); } while(0)
#else
#define BARRIER()    do { asm volatile ("" ::: "memory"); } while(0)
#endif

static uint32_t read_from_ram(uint32_t val, uint32_t offset, void *args)
{
    uint8_t *base = (uint8_t *)args;
    base += offset;

    BARRIER();
    return *(uint32_t *)base;
}

static uint32_t write_to_ram(uint32_t val, uint32_t offset, void *args)
{
    uint8_t *base = (uint8_t *)args;
    base += offset;

    BARRIER();
    *(uint32_t *)base = val;
    BARRIER();
    return val;
}

void init_bcm5719_APE_sim(void *base)
{
    APE.mIndexReadCallback = read_from_ram;
    APE.mIndexReadCallbackArgs = base;

    APE.mIndexWriteCallback = write_to_ram;
    APE.mIndexWriteCallbackArgs = base;

    /** @brief Component Registers for @ref APE. */
    /** @brief Bitmap for @ref APE_t.Mode. */
    APE.Mode.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Mode.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Status. */
    APE.Status.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Status.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.GpioMessage. */
    APE.GpioMessage.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.GpioMessage.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Event. */
    APE.Event.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Event.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxbufoffsetFunc0. */
    APE.RxbufoffsetFunc0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxbufoffsetFunc0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxbufoffsetFunc1. */
    APE.RxbufoffsetFunc1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxbufoffsetFunc1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetDoorbellFunc0. */
    APE.TxToNetDoorbellFunc0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetDoorbellFunc0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Mode2. */
    APE.Mode2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Mode2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Status2. */
    APE.Status2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Status2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.LockGrantObsolete. */
    APE.LockGrantObsolete.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.LockGrantObsolete.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolModeStatus0. */
    APE.RxPoolModeStatus0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolModeStatus0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolModeStatus1. */
    APE.RxPoolModeStatus1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolModeStatus1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolRetire0. */
    APE.RxPoolRetire0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolRetire0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolRetire1. */
    APE.RxPoolRetire1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolRetire1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetPoolModeStatus0. */
    APE.TxToNetPoolModeStatus0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetPoolModeStatus0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetBufferAllocator0. */
    APE.TxToNetBufferAllocator0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetBufferAllocator0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Tick1mhz. */
    APE.Tick1mhz.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Tick1mhz.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Tick1khz. */
    APE.Tick1khz.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Tick1khz.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Tick10hz. */
    APE.Tick10hz.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Tick10hz.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Gpio. */
    APE.Gpio.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Gpio.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.Gint. */
    APE.Gint.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.Gint.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.OtpControl. */
    APE.OtpControl.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.OtpControl.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.OtpStatus. */
    APE.OtpStatus.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.OtpStatus.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.OtpAddr. */
    APE.OtpAddr.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.OtpAddr.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.OtpReadData. */
    APE.OtpReadData.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.OtpReadData.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.CpuStatus. */
    APE.CpuStatus.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.CpuStatus.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetPoolModeStatus1. */
    APE.TxToNetPoolModeStatus1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetPoolModeStatus1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetBufferAllocator1. */
    APE.TxToNetBufferAllocator1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetBufferAllocator1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetDoorbellFunc1. */
    APE.TxToNetDoorbellFunc1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetDoorbellFunc1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxbufoffsetFunc2. */
    APE.RxbufoffsetFunc2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxbufoffsetFunc2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetDoorbellFunc2. */
    APE.TxToNetDoorbellFunc2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetDoorbellFunc2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolModeStatus2. */
    APE.RxPoolModeStatus2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolModeStatus2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolRetire2. */
    APE.RxPoolRetire2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolRetire2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetPoolModeStatus2. */
    APE.TxToNetPoolModeStatus2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetPoolModeStatus2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetBufferAllocator2. */
    APE.TxToNetBufferAllocator2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetBufferAllocator2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxbufoffsetFunc3. */
    APE.RxbufoffsetFunc3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxbufoffsetFunc3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetDoorbellFunc3. */
    APE.TxToNetDoorbellFunc3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetDoorbellFunc3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolModeStatus3. */
    APE.RxPoolModeStatus3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolModeStatus3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RxPoolRetire3. */
    APE.RxPoolRetire3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RxPoolRetire3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetPoolModeStatus3. */
    APE.TxToNetPoolModeStatus3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetPoolModeStatus3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.TxToNetBufferAllocator3. */
    APE.TxToNetBufferAllocator3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.TxToNetBufferAllocator3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcRxStatus. */
    APE.BmcToNcRxStatus.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcRxStatus.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacHigh. */
    APE.BmcToNcSourceMacHigh.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacHigh.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacLow. */
    APE.BmcToNcSourceMacLow.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacLow.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch0High. */
    APE.BmcToNcSourceMacMatch0High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch0High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch0Low. */
    APE.BmcToNcSourceMacMatch0Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch0Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch1High. */
    APE.BmcToNcSourceMacMatch1High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch1High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch1Low. */
    APE.BmcToNcSourceMacMatch1Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch1Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch2High. */
    APE.BmcToNcSourceMacMatch2High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch2High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch2Low. */
    APE.BmcToNcSourceMacMatch2Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch2Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch3High. */
    APE.BmcToNcSourceMacMatch3High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch3High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch3Low. */
    APE.BmcToNcSourceMacMatch3Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch3Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch4High. */
    APE.BmcToNcSourceMacMatch4High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch4High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch4Low. */
    APE.BmcToNcSourceMacMatch4Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch4Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch5High. */
    APE.BmcToNcSourceMacMatch5High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch5High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch5Low. */
    APE.BmcToNcSourceMacMatch5Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch5Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch6High. */
    APE.BmcToNcSourceMacMatch6High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch6High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch6Low. */
    APE.BmcToNcSourceMacMatch6Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch6Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch7High. */
    APE.BmcToNcSourceMacMatch7High.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch7High.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcSourceMacMatch7Low. */
    APE.BmcToNcSourceMacMatch7Low.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcSourceMacMatch7Low.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcRxVlan. */
    APE.BmcToNcRxVlan.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcRxVlan.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcReadBuffer. */
    APE.BmcToNcReadBuffer.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcReadBuffer.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcRxControl. */
    APE.BmcToNcRxControl.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcRxControl.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcTxStatus. */
    APE.BmcToNcTxStatus.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcTxStatus.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcTxControl. */
    APE.BmcToNcTxControl.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcTxControl.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcTxBuffer. */
    APE.BmcToNcTxBuffer.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcTxBuffer.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.BmcToNcTxBufferLast. */
    APE.BmcToNcTxBufferLast.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.BmcToNcTxBufferLast.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.RmuControl. */
    APE.RmuControl.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.RmuControl.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.ArbControl. */
    APE.ArbControl.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.ArbControl.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestPhy0. */
    APE.PerLockRequestPhy0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestPhy0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestGrc. */
    APE.PerLockRequestGrc.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestGrc.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestPhy1. */
    APE.PerLockRequestPhy1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestPhy1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestPhy2. */
    APE.PerLockRequestPhy2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestPhy2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestMem. */
    APE.PerLockRequestMem.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestMem.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestPhy3. */
    APE.PerLockRequestPhy3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestPhy3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestPort6. */
    APE.PerLockRequestPort6.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestPort6.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockRequestGpio. */
    APE.PerLockRequestGpio.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockRequestGpio.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantPhy0. */
    APE.PerLockGrantPhy0.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantPhy0.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantGrc. */
    APE.PerLockGrantGrc.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantGrc.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantPhy1. */
    APE.PerLockGrantPhy1.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantPhy1.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantPhy2. */
    APE.PerLockGrantPhy2.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantPhy2.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantMem. */
    APE.PerLockGrantMem.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantMem.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantPhy3. */
    APE.PerLockGrantPhy3.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantPhy3.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantPort6. */
    APE.PerLockGrantPort6.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantPort6.r32.installWriteCallback(write_to_ram, (uint8_t *)base);

    /** @brief Bitmap for @ref APE_t.PerLockGrantGpio. */
    APE.PerLockGrantGpio.r32.installReadCallback(read_from_ram, (uint8_t *)base);
    APE.PerLockGrantGpio.r32.installWriteCallback(write_to_ram, (uint8_t *)base);


}