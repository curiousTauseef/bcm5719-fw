////////////////////////////////////////////////////////////////////////////////
///
/// @file       main.c
///
/// @project
///
/// @brief      Main APE code
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2018-2020, Evan Lojewski
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

#include <APE.h>
#include <APE_APE.h>
#include <APE_APE_PERI.h>
#include <APE_DEVICE1.h>
#include <APE_DEVICE2.h>
#include <APE_DEVICE3.h>
#include <APE_NVIC.h>
#include <APE_SHM.h>
#include <APE_SHM1.h>
#include <APE_SHM2.h>
#include <APE_SHM3.h>
#include <Ethernet.h>
#include <NCSI.h>
#include <NVRam.h>
#include <Timer.h>
#include <ape_main.h>

#ifndef CXX_SIMULATOR
#include <ape_console.h>
#include <printf.h>
#endif

#define RMU_WATCHDOG_TIMEOUT_MS (10)
#define RX_CPU_RESET_TIMEOUT_MS (1000) /* Wait up to 1 second for each RX CPU to start */
#define GRC_RESET_TIMEOUT_MS (150)     /* Wait 150ms for the GRC reset to settle */

static NetworkPort_t *gPort;
static uint32_t gResetTime;

void handleCommand(volatile SHM_t *shm)
{
    uint32_t command = shm->LoaderCommand.bits.Command;
    if (!command)
    {
        return;
    }

    uint32_t arg0 = shm->LoaderArg0.r32;
    uint32_t arg1 = shm->LoaderArg1.r32;

    switch (command)
    {
        default:
            break;

        case SHM_LOADER_COMMAND_COMMAND_READ_MEM:
        {
            // Read word address specified in arg0
            uint32_t *addr = ((void *)arg0);
            shm->LoaderArg0.r32 = *addr;
            break;
        }
        case SHM_LOADER_COMMAND_COMMAND_WRITE_MEM:
        {
            // Write word address specified in arg0 with arg1
            uint32_t *addr = ((void *)arg0);
            *addr = arg1;
            break;
        }
        case SHM_LOADER_COMMAND_COMMAND_CALL:
        {
            // call address specified in arg0.
            void (*function)(uint32_t) = ((void *)arg0);
            function(arg1);
            break;
        }
    }

    // Mark command as handled.
    shm->LoaderCommand.bits.Command = 0;
}

void wait_for_rx(const volatile DEVICE_t *device, const volatile SHM_t *shm)
{
    uint32_t startTime = Timer_getCurrentTime1KHz();

    bool waiting = true;
    do
    {
        if (device->RxRiscStatus.bits.Halted)
        {
            // If the RX CPU has halted, exit out.
            waiting = false;
        }

        if (SHM_RCPU_SEG_SIG_SIG_RCPU_MAGIC == shm->RcpuSegSig.bits.Sig)
        {
            // Firmware has finished initialization.
            waiting = false;
        }

        if (Timer_didTimeElapsed1KHz(startTime, RX_CPU_RESET_TIMEOUT_MS))
        {
            printf("RX CPU reset timeout.\n");
            waiting = false;
        }
    } while (waiting);
}

void wait_for_all_rx()
{
    wait_for_rx(&DEVICE, &SHM);
    wait_for_rx(&DEVICE1, &SHM1);
    wait_for_rx(&DEVICE2, &SHM2);
    wait_for_rx(&DEVICE3, &SHM3);
}

void handleBMCPacket(bool passthrough)
{
    static bool packetInProgress = false;
    static uint32_t inProgressStartTime = 0;
    uint32_t buffer[1024];

    RegAPE_PERIBmcToNcRxStatus_t stat;
    stat.r32 = APE_PERI.BmcToNcRxStatus.r32;

    if (stat.bits.New)
    {
        packetInProgress = false;
        if (stat.bits.Bad)
        {
            // ACK bad packet.
            RMU_resetBadPacket();
        }
        else
        {
            uint32_t bytes = stat.bits.PacketLength;
            if (!stat.bits.Passthru)
            {
                uint32_t words = DIVIDE_RND_UP(bytes, sizeof(uint32_t));
                if (words > ARRAY_ELEMENTS(buffer))
                {
                    // This should never happen...
                    printf("Dropping NCSI packet\n");
                    while (words--)
                    {
                        // Read out the packet, but drop it.
                        uint32_t word = APE_PERI.BmcToNcReadBuffer.r32;
                        (void)word;
                    }
                }
                else
                {
                    int i = 0;
                    while (words--)
                    {
                        uint32_t word = (APE_PERI.BmcToNcReadBuffer.r32);
                        buffer[i] = word;
                        i++;
                    }

                    NetworkFrame_t *frame = ((NetworkFrame_t *)buffer);

                    handleNCSIFrame(frame);
                }
            }
            else
            {
                // Pass through to network
                NetworkPort_t *port = gPort;
                ++port->shm_channel->NcsiChannelNcsiRx.r32;
                if (port->shm_channel->NcsiChannelInfo.bits.Enabled && passthrough)
                {
                    if (!Network_TX_transmitPassthroughPacket(bytes, port))
                    {
                        printf("Resetting TX...\n");
                        // Reset, as it's likely locked up now.
                        wait_for_all_rx();
                        NCSI_reload(AS_NEEDED);
                    }
                }
                else
                {
                    printf("Dropping PT\n");
                    uint32_t words = DIVIDE_RND_UP(bytes, sizeof(uint32_t));
                    while (words--)
                    {
                        // Read out the packet, but drop it.
                        uint32_t word = APE_PERI.BmcToNcReadBuffer.r32;
                        (void)word;
                    }
                }
            }
        }
    }
    else if (stat.bits.InProgress)
    {
        if (packetInProgress)
        {
            // In some cases (RMU reset during startup w/ active communication)
            // the RMU state machine can enter a stuck state.
            // This can be seen as an InProgress for an unreasonable amount of time.
            // In such a case, reset the RMU to recover.
            if (Timer_didTimeElapsed1KHz(inProgressStartTime, RMU_WATCHDOG_TIMEOUT_MS))
            {
                printf("RMU Hang detected, resetting.\n");
                RMU_init();
                packetInProgress = false;
            }
        }
        else
        {
            packetInProgress = true;
            inProgressStartTime = Timer_getCurrentTime1KHz();
        }
    }
}

void __attribute__((interrupt)) IRQ_VoltageSource()
{
    NVIC.InterruptClearPending.r32 = NVIC_INTERRUPT_CLEAR_PENDING_CLRPEND_VMAIN;

    NetworkPort_t *port = gPort;

    if (port->device->Status.bits.VMAINPowerStatus)
    {
        printf("Vsrc: Main\n");
    }
    else
    {
        printf("Vsrc: Aux\n");
    }

    // Ensure we reinitialize hardware as needed.
    gResetTime = Timer_getCurrentTime1KHz();
    if (!gResetTime)
    {
        // We use 0 to mean that no reset has happend. Make sure this value is never 0.
        gResetTime--;
    }
}

bool resetInProgress(RegAPEStatus_t status, RegAPEStatus2_t status2)
{
    if (status.bits.Port0GRCReset || status.bits.Port1GRCReset || status2.bits.Port2GRCReset || status2.bits.Port3GRCReset)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void __attribute__((interrupt)) IRQ_PowerStatusChanged(void)
{
    RegAPEStatus_t status = APE.Status;
    RegAPEStatus2_t status2 = APE.Status2;

    // Clear Interrupts
    APE.Status.r32 = status.r32;
    APE.Status2.r32 = status2.r32;

    NVIC.InterruptClearPending.r32 = NVIC_INTERRUPT_CLEAR_PENDING_CLRPEND_GENERAL_RESET;

    printf("PowerStateChanged.\n");

    if (!gResetTime)
    {

        if (resetInProgress(status, status2))
        {
            printf("GRC Reset.\n");
            gResetTime = Timer_getCurrentTime1KHz();
            if (!gResetTime)
            {
                // We use 0 to mean that no reset has happend. Make sure this value is never 0.
                gResetTime--;
            }

            // Disable the interrupt so that we can exit the interrupt handler
            NVIC.InterruptClearEnable.r32 = NVIC_INTERRUPT_SET_ENABLE_SETENA_GENERAL_RESET;
        }
    }
}

void initSHM(volatile SHM_t *shm)
{
    RegSHMFwStatus_t status;
    status.r32 = 0;
    status.bits.Ready = 1;

    RegSHMFwFeatures_t features;
    features.r32 = 0;
    features.bits.NCSI = 1;

    shm->FwVersion.r32 = (VERSION_MAJOR << 24) | (VERSION_MINOR << 16) | VERSION_PATCH; //lint !e835
    shm->FwFeatures.r32 = features.r32;
    shm->FwStatus.r32 = status.r32;

    shm->SegSig.r32 = 'APE!'; //lint !e742
}

void __attribute__((noreturn)) loaderLoop(void)
{
    uint32_t host_state = SHM.HostDriverState.bits.State;
    bool reset_allowed = host_state == SHM_HOST_DRIVER_STATE_STATE_START;

    // Update SHM.Sig to signal ready.
    SHM.SegSig.bits.Sig = SHM_SEG_SIG_SIG_LOADER;
    initSHM(&SHM);
    initSHM(&SHM1);
    initSHM(&SHM2);
    initSHM(&SHM3);

    // Enable GRC Reset / Power Status Changed and Vsrc interrupts
    NVIC.InterruptSetEnable.r32 = NVIC_INTERRUPT_SET_ENABLE_SETENA_VMAIN | NVIC_INTERRUPT_SET_ENABLE_SETENA_GENERAL_RESET;

    for (;;)
    {
        if (gResetTime)
        {
            RegAPEStatus_t status = APE.Status;
            RegAPEStatus2_t status2 = APE.Status2;

            // Wait for reset to complete.
            if (resetInProgress(status, status2))
            {
                // We are waiting for the reset signals to clear before continuing.
                APE.Status.r32 = status.r32;
                APE.Status2.r32 = status2.r32;

                // Initialize timer for reset.
                gResetTime = Timer_getCurrentTime1KHz();
                if (!gResetTime)
                {
                    // We use 0 to mean that no reset has happend. Make sure this value is never 0.
                    gResetTime--;
                }
            }
            else if (Timer_didTimeElapsed1KHz(gResetTime, GRC_RESET_TIMEOUT_MS))
            {
                // We may still have an interrupt pending since we disabled the interrupt. Clear it so we don't get an extra trigger.
                NVIC.InterruptClearPending.r32 = NVIC_INTERRUPT_CLEAR_PENDING_CLRPEND_GENERAL_RESET;
                gResetTime = 0;

                printf("Handling reset...\n");

                // Perform TX reinit as the PHY / MII was also probably reset.
                wait_for_all_rx();
                NCSI_reload(AS_NEEDED);

                // Reset complete, re-enable interrupt handler.
                NVIC.InterruptSetEnable.r32 = NVIC_INTERRUPT_SET_ENABLE_SETENA_GENERAL_RESET;
            }

            handleBMCPacket(false);
        }
        else
        {
            Network_checkPortState(gPort);

            handleBMCPacket(true);
            NCSI_handlePassthrough();

            if (host_state != SHM.HostDriverState.bits.State)
            {
                host_state = SHM.HostDriverState.bits.State;

                if (SHM_HOST_DRIVER_STATE_STATE_START == host_state)
                {
                    printf("host started\n");

                    reset_allowed = true;
                }
                else
                {
                    if (SHM_HOST_DRIVER_STATE_STATE_UNLOAD == host_state)
                    {
                        printf("host unloaded.\n");
                    }
                    else
                    {
                        printf("wol?\n");
                    }

                    reset_allowed = false;
                }
            }
            else if (reset_allowed && !Network_checkEnableState(gPort) && !gResetTime)
            {
                printf("APE mode change, resetting.\n");
                wait_for_all_rx();
                NCSI_reload(AS_NEEDED);

                // Update host state to make sure we don't reset twice if it's changed.
                host_state = SHM.HostDriverState.bits.State;

                reset_allowed = false;
            }
        }

        handleCommand(&SHM);
        handleCommand(&SHM1);
        handleCommand(&SHM2);
        handleCommand(&SHM3);
    }
}

bool handle_reset(void)
{
    uint32_t chip_id = DEVICE.ChipId.r32;
    if (!chip_id)
    {
        printf("Resetting...\n");
        RegAPEGpio_t apegpio;
        apegpio.r32 = APE.Gpio.r32;

        // Release all locks to ensure that the RX CPU will be able to boot up
        NVRam_releaseAllLocks();
        APE_releaseAllLocks();

        // Reset needed.
        // Configure the clock mux to be driving by the APE.
        apegpio.bits.PIN0ModeOutput = 1;
        apegpio.bits.PIN1ModeOutput = 1;
        apegpio.bits.PIN2ModeOutput = 1;
        apegpio.bits.PIN0UnknownOut = 1; // Clock_P
        apegpio.bits.PIN1UnknownOut = 0; // Clock_N
        apegpio.bits.PIN2UnknownOut = 1; // CLock Mux = APE GPIO
        APE.Gpio.r32 = apegpio.r32;

        for (int i = 0; i < 38; i++)
        {
            // Clock it...
            apegpio.bits.PIN0UnknownOut = ~apegpio.bits.PIN0UnknownOut; // Clock_P
            apegpio.bits.PIN1UnknownOut = ~apegpio.bits.PIN1UnknownOut; // Clock_N
            APE.Gpio.r32 = apegpio.r32;
        }

        do
        {
            // Wait for the ChipID register to be readable
            chip_id = DEVICE.ChipId.r32;
        } while (!chip_id);

        // Restore the mux settings.
        apegpio.bits.PIN2UnknownOut = 0; // CLock Mux = PCIe clock
        APE.Gpio.r32 = apegpio.r32;

        // Wait for the RX CPU to finish executing before continuing.
        wait_for_all_rx();

        return true;
    }
    else
    {
        // No reset
        return false;
    }
}

//lint -esym(714, __start) // Referenced by build tools.
void __attribute__((noreturn)) __start()
{
    // Ensure all pending interrupts are cleared.
    NVIC.InterruptClearPending.r32 = 0xFFFFFFFF;
    gResetTime = false;

    // Switch to APE interrupt handlers
    union
    {
        uint32_t u32;
        vector_table_t *vectors;
    } caster;
    caster.vectors = &gVectors;
    NVIC.VectorTableOffset.r32 = caster.u32;

    // Handle Initialization
    bool full_init = handle_reset();
    if (reset_ape_console())
    {
        full_init = true;
    }

    printf("APE v" STRINGIFY(VERSION_MAJOR) "." STRINGIFY(VERSION_MINOR) "." STRINGIFY(VERSION_PATCH) " NCSI Port " STRINGIFY(NETWORK_PORT) "\n");
    gPort = Network_getPort(NETWORK_PORT);

    NCSI_usePort(gPort);

    RMU_init();

    if (full_init)
    {
        printf("Chip Reset.\n");
        NCSI_init();
    }
    else
    {
        printf("APE Reload.\n");
        NCSI_reload(SHM_HOST_DRIVER_STATE_STATE_START != SHM.HostDriverState.bits.State ? AS_NEEDED : NEVER_RESET);
    }

    loaderLoop();
}
