#include "includes.h"

uint8_t i2c_tx_data[1] = { 0 };
uint8_t i2c_rx_data[3] = { 0 };
int i2c_tx_data_index;
int i2c_rx_data_index;
int i2c_tx_data_length;
int i2c_rx_data_length;
bool i2c_failed;
bool i2c_stopped;
bool i2c_need_restart;

void init_ir_temperature_sensor_communication()
{
    P6->SEL0 |= BIT4 | BIT5; // Set P6.4 and P6.5 as UCB1

    // Configuration of I2C
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Enter reset mode
    EUSCI_B1->CTLW0 |= (EUSCI_B_CTLW0_SWRST | // Remain reset mode
            EUSCI_B_CTLW0_MODE_3 | //I2C mode
            EUSCI_B_CTLW0_SYNC | // Sync mode
            EUSCI_B_CTLW0_MST | // Master
            EUSCI_B_CTLW0_UCSSEL_2); // Use SMCLK as clock
    EUSCI_B1->CTLW1 = EUSCI_B_CTLW1_ASTP_0; // No automatic stop generated
    EUSCI_B1->BRW = 120; // Set clock speed (12MHz / 120 = 100kHz)
    EUSCI_B1->I2CSA = 0x5A; // Slave at 0x5A
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST; // Quit reset mode

    // Enable interruptions
    EUSCI_B1->IFG &= ~(EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_TXIFG0
            | EUSCI_B_IFG_NACKIFG | EUSCI_B_IFG_STPIFG);
    EUSCI_B1->IE |= (EUSCI_B_IE_RXIE | EUSCI_B_IE_TXIE0 | EUSCI_B_IE_NACKIE
            | EUSCI_B_IE_STPIE);
    NVIC_EnableIRQ(EUSCIB1_IRQn);
}

double get_ir_sensor_temperature()
{
    // Start transmission
    while (EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY)
    {
        // Wait until bus not busy
    }
    i2c_tx_data_index = 0; // Reset index
    i2c_rx_data_index = 0;

    i2c_tx_data[0] = 0b00000111; // Read T0 at 0x07
    i2c_tx_data_length = 1;
    i2c_rx_data_length = 3; // Lower byte, higher byte, PEC

    i2c_stopped = false;
    i2c_failed = false;
    i2c_need_restart = true;

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR; // Enter TX mode at first
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // Send START

    while (!i2c_stopped)
    {
        // Then every thing will be handled by the interruption,
        // we wait until the communication finished here
    }
    if (i2c_failed)
    {
        return -1;
    }

    uint16_t result = ((i2c_rx_data[1] << 8) | i2c_rx_data[0]); // Concatener bytes, PEC is not needed
    if (result & 0x8000)    // The 16th bit is a error flag
    {
        return -2;
    }

    return result * 0.02 - 273.15;
}

void EUSCIB1_IRQHandler(void)
{
    // Check for NACK, which means communication failed
    if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
    {
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_NACKIFG;
        EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP; // Stop the communication
        i2c_failed = true;
    }

    // Check for STOP
    if (EUSCI_B1->IFG & EUSCI_B_IFG_STPIFG)
    {
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_STPIFG;
        i2c_stopped = true;
    }

    // Check for TX ready
    if (EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0)
    {
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_TXIFG0; // Clear the TX ready flag

        if (i2c_tx_data_index < i2c_tx_data_length) // Send not yet finished
        {
            EUSCI_B1->TXBUF = i2c_tx_data[i2c_tx_data_index]; // Send data
            i2c_tx_data_index += 1; // Prepare for sending next data
        }
        else if (i2c_need_restart)
        {
            // RESTART needed
            i2c_need_restart = false;
            EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_TR; // Change to RX mode
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // Send a RESTART
        }
    }

    // Check for RX ready
    if (EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0)
    {
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_RXIFG0; // Clear the RX ready flag

        if (i2c_rx_data_index < i2c_rx_data_length)
        {
            i2c_rx_data[i2c_rx_data_index] = EUSCI_B1->RXBUF; // Save received data
            i2c_rx_data_index += 1; // Prepare for reading next data
        }
        else
        {
            EUSCI_B1->RXBUF; // Consume values in the RXBUF to allow STOP
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
        }
    }
}
