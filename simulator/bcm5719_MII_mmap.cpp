////////////////////////////////////////////////////////////////////////////////
///
/// @file       bcm5719_MII_mmap.cpp
///
/// @project    bcm5719
///
/// @brief      bcm5719_MII_mmap
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
#include <bcm5719_MII.h>

#ifdef __ppc64__
#define BARRIER()    do { asm volatile ("sync 0\neieio\n" ::: "memory"); } while(0)
#else
#define BARRIER()    do { asm volatile ("" ::: "memory"); } while(0)
#endif

typedef std::pair<uint8_t *, uint32_t> ram_offset_t;

static uint32_t read_from_ram(uint32_t val, void *args)
{
    ram_offset_t *loc = (ram_offset_t *)args;

    uint8_t *base = loc->first;
    base += loc->second;

    BARRIER();
    return *(uint32_t *)base;
}

static uint32_t read_from_ram_index(uint32_t index, void *args)
{
    uint8_t *base = (uint8_t *)args;
    base += index;

    BARRIER();
    return *(uint32_t *)base;
}

static uint32_t write_to_ram(uint32_t val, void *args)
{
    ram_offset_t *loc = (ram_offset_t *)args;

    uint8_t *base = loc->first;
    base += loc->second;

    BARRIER();
    *(uint32_t *)base = val;
    BARRIER();
    return val;
}

static void write_to_ram_index(uint32_t index, uint32_t val, void *args)
{
    uint8_t *base = (uint8_t *)args;
    base += index;

    BARRIER();
    *(uint32_t *)base = val;
    BARRIER();
}

void init_bcm5719_MII_mmap(void *base)
{
    MII.mIndexReadCallback = read_from_ram_index;
    MII.mIndexReadCallbackArgs = base;

    MII.mIndexWriteCallback = write_to_ram_index;
    MII.mIndexWriteCallbackArgs = base;

    /** @brief Component Registers for @ref MII. */
    /** @brief Bitmap for @ref MII_t.Control. */
    static ram_offset_t MII_Control_r16((uint8_t *)base, (uint32_t)0);
    MII.Control.r16.installReadCallback(read_from_ram, &MII_Control_r16);
    MII.Control.r16.installWriteCallback(write_to_ram, &MII_Control_r16);

    /** @brief Bitmap for @ref MII_t.Status. */
    static ram_offset_t MII_Status_r16((uint8_t *)base, (uint32_t)1);
    MII.Status.r16.installReadCallback(read_from_ram, &MII_Status_r16);
    MII.Status.r16.installWriteCallback(write_to_ram, &MII_Status_r16);

    /** @brief Bitmap for @ref MII_t.PhyIdHigh. */
    static ram_offset_t MII_PhyIdHigh_r16((uint8_t *)base, (uint32_t)2);
    MII.PhyIdHigh.r16.installReadCallback(read_from_ram, &MII_PhyIdHigh_r16);
    MII.PhyIdHigh.r16.installWriteCallback(write_to_ram, &MII_PhyIdHigh_r16);

    /** @brief Bitmap for @ref MII_t.PhyIdLow. */
    static ram_offset_t MII_PhyIdLow_r16((uint8_t *)base, (uint32_t)3);
    MII.PhyIdLow.r16.installReadCallback(read_from_ram, &MII_PhyIdLow_r16);
    MII.PhyIdLow.r16.installWriteCallback(write_to_ram, &MII_PhyIdLow_r16);

    /** @brief Bitmap for @ref MII_t.AutonegotiationAdvertisement. */
    static ram_offset_t MII_AutonegotiationAdvertisement_r16((uint8_t *)base, (uint32_t)4);
    MII.AutonegotiationAdvertisement.r16.installReadCallback(read_from_ram, &MII_AutonegotiationAdvertisement_r16);
    MII.AutonegotiationAdvertisement.r16.installWriteCallback(write_to_ram, &MII_AutonegotiationAdvertisement_r16);

    /** @brief Bitmap for @ref MII_t.AutonegotiationLinkPartnerAbilityBasePage. */
    static ram_offset_t MII_AutonegotiationLinkPartnerAbilityBasePage_r16((uint8_t *)base, (uint32_t)5);
    MII.AutonegotiationLinkPartnerAbilityBasePage.r16.installReadCallback(read_from_ram, &MII_AutonegotiationLinkPartnerAbilityBasePage_r16);
    MII.AutonegotiationLinkPartnerAbilityBasePage.r16.installWriteCallback(write_to_ram, &MII_AutonegotiationLinkPartnerAbilityBasePage_r16);

    /** @brief Bitmap for @ref MII_t.AutonegotiationExpansion. */
    static ram_offset_t MII_AutonegotiationExpansion_r16((uint8_t *)base, (uint32_t)6);
    MII.AutonegotiationExpansion.r16.installReadCallback(read_from_ram, &MII_AutonegotiationExpansion_r16);
    MII.AutonegotiationExpansion.r16.installWriteCallback(write_to_ram, &MII_AutonegotiationExpansion_r16);

    /** @brief Bitmap for @ref MII_t.AutonegotiationNextPageTransmit. */
    static ram_offset_t MII_AutonegotiationNextPageTransmit_r16((uint8_t *)base, (uint32_t)7);
    MII.AutonegotiationNextPageTransmit.r16.installReadCallback(read_from_ram, &MII_AutonegotiationNextPageTransmit_r16);
    MII.AutonegotiationNextPageTransmit.r16.installWriteCallback(write_to_ram, &MII_AutonegotiationNextPageTransmit_r16);

    /** @brief Bitmap for @ref MII_t.AutonegotiationLinkPartnerAbilityNextPage. */
    static ram_offset_t MII_AutonegotiationLinkPartnerAbilityNextPage_r16((uint8_t *)base, (uint32_t)8);
    MII.AutonegotiationLinkPartnerAbilityNextPage.r16.installReadCallback(read_from_ram, &MII_AutonegotiationLinkPartnerAbilityNextPage_r16);
    MII.AutonegotiationLinkPartnerAbilityNextPage.r16.installWriteCallback(write_to_ram, &MII_AutonegotiationLinkPartnerAbilityNextPage_r16);

    /** @brief Bitmap for @ref MII_t.1000baseTControl. */
    static ram_offset_t MII__1000baseTControl_r16((uint8_t *)base, (uint32_t)9);
    MII._1000baseTControl.r16.installReadCallback(read_from_ram, &MII__1000baseTControl_r16);
    MII._1000baseTControl.r16.installWriteCallback(write_to_ram, &MII__1000baseTControl_r16);

    /** @brief Bitmap for @ref MII_t.1000baseTStatus. */
    static ram_offset_t MII__1000baseTStatus_r16((uint8_t *)base, (uint32_t)10);
    MII._1000baseTStatus.r16.installReadCallback(read_from_ram, &MII__1000baseTStatus_r16);
    MII._1000baseTStatus.r16.installWriteCallback(write_to_ram, &MII__1000baseTStatus_r16);

    /** @brief Bitmap for @ref MII_t.BroadreachLreAccess. */
    static ram_offset_t MII_BroadreachLreAccess_r16((uint8_t *)base, (uint32_t)14);
    MII.BroadreachLreAccess.r16.installReadCallback(read_from_ram, &MII_BroadreachLreAccess_r16);
    MII.BroadreachLreAccess.r16.installWriteCallback(write_to_ram, &MII_BroadreachLreAccess_r16);

    /** @brief Bitmap for @ref MII_t.IeeeExtendedStatus. */
    static ram_offset_t MII_IeeeExtendedStatus_r16((uint8_t *)base, (uint32_t)15);
    MII.IeeeExtendedStatus.r16.installReadCallback(read_from_ram, &MII_IeeeExtendedStatus_r16);
    MII.IeeeExtendedStatus.r16.installWriteCallback(write_to_ram, &MII_IeeeExtendedStatus_r16);

    /** @brief Bitmap for @ref MII_t.PhyExtendedControl. */
    static ram_offset_t MII_PhyExtendedControl_r16((uint8_t *)base, (uint32_t)16);
    MII.PhyExtendedControl.r16.installReadCallback(read_from_ram, &MII_PhyExtendedControl_r16);
    MII.PhyExtendedControl.r16.installWriteCallback(write_to_ram, &MII_PhyExtendedControl_r16);

    /** @brief Bitmap for @ref MII_t.PhyExtendedStatus. */
    static ram_offset_t MII_PhyExtendedStatus_r16((uint8_t *)base, (uint32_t)17);
    MII.PhyExtendedStatus.r16.installReadCallback(read_from_ram, &MII_PhyExtendedStatus_r16);
    MII.PhyExtendedStatus.r16.installWriteCallback(write_to_ram, &MII_PhyExtendedStatus_r16);

    /** @brief Bitmap for @ref MII_t.ReceiveErrorCounter. */
    static ram_offset_t MII_ReceiveErrorCounter_r16((uint8_t *)base, (uint32_t)18);
    MII.ReceiveErrorCounter.r16.installReadCallback(read_from_ram, &MII_ReceiveErrorCounter_r16);
    MII.ReceiveErrorCounter.r16.installWriteCallback(write_to_ram, &MII_ReceiveErrorCounter_r16);

    /** @brief Bitmap for @ref MII_t.FalseCarrierSenseCounter. */
    static ram_offset_t MII_FalseCarrierSenseCounter_r16((uint8_t *)base, (uint32_t)19);
    MII.FalseCarrierSenseCounter.r16.installReadCallback(read_from_ram, &MII_FalseCarrierSenseCounter_r16);
    MII.FalseCarrierSenseCounter.r16.installWriteCallback(write_to_ram, &MII_FalseCarrierSenseCounter_r16);

    /** @brief Bitmap for @ref MII_t.LocalRemoteReceiverNotOkCounter. */
    static ram_offset_t MII_LocalRemoteReceiverNotOkCounter_r16((uint8_t *)base, (uint32_t)20);
    MII.LocalRemoteReceiverNotOkCounter.r16.installReadCallback(read_from_ram, &MII_LocalRemoteReceiverNotOkCounter_r16);
    MII.LocalRemoteReceiverNotOkCounter.r16.installWriteCallback(write_to_ram, &MII_LocalRemoteReceiverNotOkCounter_r16);

    /** @brief Bitmap for @ref MII_t.AuxiliaryControl. */
    static ram_offset_t MII_AuxiliaryControl_r16((uint8_t *)base, (uint32_t)24);
    MII.AuxiliaryControl.r16.installReadCallback(read_from_ram, &MII_AuxiliaryControl_r16);
    MII.AuxiliaryControl.r16.installWriteCallback(write_to_ram, &MII_AuxiliaryControl_r16);

    /** @brief Bitmap for @ref MII_t.AuxiliaryStatusSummary. */
    static ram_offset_t MII_AuxiliaryStatusSummary_r16((uint8_t *)base, (uint32_t)25);
    MII.AuxiliaryStatusSummary.r16.installReadCallback(read_from_ram, &MII_AuxiliaryStatusSummary_r16);
    MII.AuxiliaryStatusSummary.r16.installWriteCallback(write_to_ram, &MII_AuxiliaryStatusSummary_r16);

    /** @brief Bitmap for @ref MII_t.InterruptStatus. */
    static ram_offset_t MII_InterruptStatus_r16((uint8_t *)base, (uint32_t)26);
    MII.InterruptStatus.r16.installReadCallback(read_from_ram, &MII_InterruptStatus_r16);
    MII.InterruptStatus.r16.installWriteCallback(write_to_ram, &MII_InterruptStatus_r16);

    /** @brief Bitmap for @ref MII_t.InterruptMask. */
    static ram_offset_t MII_InterruptMask_r16((uint8_t *)base, (uint32_t)27);
    MII.InterruptMask.r16.installReadCallback(read_from_ram, &MII_InterruptMask_r16);
    MII.InterruptMask.r16.installWriteCallback(write_to_ram, &MII_InterruptMask_r16);

    /** @brief Bitmap for @ref MII_t.CabletronLed. */
    static ram_offset_t MII_CabletronLed_r16((uint8_t *)base, (uint32_t)28);
    MII.CabletronLed.r16.installReadCallback(read_from_ram, &MII_CabletronLed_r16);
    MII.CabletronLed.r16.installWriteCallback(write_to_ram, &MII_CabletronLed_r16);

    /** @brief Bitmap for @ref MII_t.Test1. */
    static ram_offset_t MII_Test1_r16((uint8_t *)base, (uint32_t)30);
    MII.Test1.r16.installReadCallback(read_from_ram, &MII_Test1_r16);
    MII.Test1.r16.installWriteCallback(write_to_ram, &MII_Test1_r16);

    /** @brief Bitmap for @ref MII_t.BlockSelect. */
    static ram_offset_t MII_BlockSelect_r16((uint8_t *)base, (uint32_t)31);
    MII.BlockSelect.r16.installReadCallback(read_from_ram, &MII_BlockSelect_r16);
    MII.BlockSelect.r16.installWriteCallback(write_to_ram, &MII_BlockSelect_r16);

    /** @brief Bitmap for @ref MII_t.10baseT. */
    static ram_offset_t MII__10baseT_r16((uint8_t *)base, (uint32_t)280);
    MII._10baseT.r16.installReadCallback(read_from_ram, &MII__10baseT_r16);
    MII._10baseT.r16.installWriteCallback(write_to_ram, &MII__10baseT_r16);

    /** @brief Bitmap for @ref MII_t.DllSelection. */
    static ram_offset_t MII_DllSelection_r16((uint8_t *)base, (uint32_t)284);
    MII.DllSelection.r16.installReadCallback(read_from_ram, &MII_DllSelection_r16);
    MII.DllSelection.r16.installWriteCallback(write_to_ram, &MII_DllSelection_r16);

    /** @brief Bitmap for @ref MII_t.PowerAndMiiControl. */
    static ram_offset_t MII_PowerAndMiiControl_r16((uint8_t *)base, (uint32_t)536);
    MII.PowerAndMiiControl.r16.installReadCallback(read_from_ram, &MII_PowerAndMiiControl_r16);
    MII.PowerAndMiiControl.r16.installWriteCallback(write_to_ram, &MII_PowerAndMiiControl_r16);

    /** @brief Bitmap for @ref MII_t.SpareControl1. */
    static ram_offset_t MII_SpareControl1_r16((uint8_t *)base, (uint32_t)540);
    MII.SpareControl1.r16.installReadCallback(read_from_ram, &MII_SpareControl1_r16);
    MII.SpareControl1.r16.installWriteCallback(write_to_ram, &MII_SpareControl1_r16);

    /** @brief Bitmap for @ref MII_t.ClockAlignmentControl. */
    static ram_offset_t MII_ClockAlignmentControl_r16((uint8_t *)base, (uint32_t)796);
    MII.ClockAlignmentControl.r16.installReadCallback(read_from_ram, &MII_ClockAlignmentControl_r16);
    MII.ClockAlignmentControl.r16.installWriteCallback(write_to_ram, &MII_ClockAlignmentControl_r16);

    /** @brief Bitmap for @ref MII_t.MiscTest1. */
    static ram_offset_t MII_MiscTest1_r16((uint8_t *)base, (uint32_t)1048);
    MII.MiscTest1.r16.installReadCallback(read_from_ram, &MII_MiscTest1_r16);
    MII.MiscTest1.r16.installWriteCallback(write_to_ram, &MII_MiscTest1_r16);

    /** @brief Bitmap for @ref MII_t.SpareControl2. */
    static ram_offset_t MII_SpareControl2_r16((uint8_t *)base, (uint32_t)1052);
    MII.SpareControl2.r16.installReadCallback(read_from_ram, &MII_SpareControl2_r16);
    MII.SpareControl2.r16.installWriteCallback(write_to_ram, &MII_SpareControl2_r16);

    /** @brief Bitmap for @ref MII_t.MiscTest2. */
    static ram_offset_t MII_MiscTest2_r16((uint8_t *)base, (uint32_t)1304);
    MII.MiscTest2.r16.installReadCallback(read_from_ram, &MII_MiscTest2_r16);
    MII.MiscTest2.r16.installWriteCallback(write_to_ram, &MII_MiscTest2_r16);

    /** @brief Bitmap for @ref MII_t.SpareControl3. */
    static ram_offset_t MII_SpareControl3_r16((uint8_t *)base, (uint32_t)1308);
    MII.SpareControl3.r16.installReadCallback(read_from_ram, &MII_SpareControl3_r16);
    MII.SpareControl3.r16.installWriteCallback(write_to_ram, &MII_SpareControl3_r16);

    /** @brief Bitmap for @ref MII_t.MiscellaneousControl. */
    static ram_offset_t MII_MiscellaneousControl_r16((uint8_t *)base, (uint32_t)1816);
    MII.MiscellaneousControl.r16.installReadCallback(read_from_ram, &MII_MiscellaneousControl_r16);
    MII.MiscellaneousControl.r16.installWriteCallback(write_to_ram, &MII_MiscellaneousControl_r16);


}
