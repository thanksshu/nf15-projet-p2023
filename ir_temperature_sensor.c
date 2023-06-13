#include "includes.h"

uint8_t i2c_tx_data[16] = { 0 }; // Buffer for TX data
uint8_t i2c_rx_data[16] = { 0 }; // Buffer for RX data
int i2c_tx_data_index;
int i2c_rx_data_index;
int i2c_tx_data_length;
int i2c_rx_data_length;
bool i2c_failed;
bool i2c_stopped;
bool i2c_need_restart;

uint8_t gen_crc(uint8_t *data, size_t len)
{
    uint8_t crc = 0x00; // Initial value
    uint8_t width = 8; // Width is 8
    size_t i, j;

    for (i = 0; i < len; i++)
    {
        crc ^= *(data + i);
        for (j = 0; j < width; j++)
        {
            if ((crc & 0x80) != 0) // Check MSB
                crc = (uint8_t) ((crc << 1) ^ 0x07); // Polynomial is 0b00000111
            else
                crc <<= 1;
        }
    }
    return crc;
}

void init_ir_temperature_sensor_communication()
{

    // Configuration of I2C
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Enter reset mode
    EUSCI_B1->CTLW0 |= (EUSCI_B_CTLW0_SWRST | // Remain reset mode
            EUSCI_B_CTLW0_MODE_3 | //I2C mode
            EUSCI_B_CTLW0_SYNC | // Sync mode
            EUSCI_B_CTLW0_MST | // Master
            EUSCI_B_CTLW0_UCSSEL_2); // Use SMCLK as clock
    EUSCI_B1->CTLW1 = EUSCI_B_CTLW1_ASTP_0; // No automatic stop generated
    EUSCI_B1->BRW = 120; // Set clock speed (12MHz / 120 = 100kHz)
    EUSCI_B1->I2CSA = IR_SENSOR_ADDR; // Slave at 0x5A
    P6->SEL0 |= BIT4 | BIT5; // Set P6.4 and P6.5 as UCB1
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST; // Quit reset mode

    // Enable interruptions
    EUSCI_B1->IFG &= ~(EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_TXIFG0
            | EUSCI_B_IFG_NACKIFG | EUSCI_B_IFG_STPIFG);
    EUSCI_B1->IE |= (EUSCI_B_IE_RXIE | EUSCI_B_IE_TXIE0 | EUSCI_B_IE_NACKIE
            | EUSCI_B_IE_STPIE);
    NVIC_EnableIRQ(EUSCIB1_IRQn);
}

double get_ir_sensor_emissivity()
{
    // Start transmission
    while (EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY)
    {
        // Wait until bus not busy
    }
    i2c_tx_data_index = 0; // Reset index
    i2c_rx_data_index = 0;

    i2c_tx_data[0] = 0b00100100; // Access emissivity at 0x04 in EPPROM
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

    double result = ((i2c_rx_data[1] << 8) | i2c_rx_data[0]); // Concatener bytes, PEC is not needed

    uint8_t all_data[5];
    all_data[0] = IR_SENSOR_ADDR;
    all_data[1] = i2c_tx_data[0];
    all_data[2] = IR_SENSOR_ADDR;
    all_data[3] = i2c_rx_data[0];
    all_data[4] = i2c_rx_data[1];

    if (i2c_rx_data[2] != gen_crc(all_data, 5))
    {
        return -2;
    }

    return result / 65535;
}

int set_ir_sensor_emissivity(double value)
{
    /* First set to 0 */
    while (EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY)
    {
        // Wait until bus not busy
    }
    i2c_tx_data_index = 0; // Reset index
    i2c_rx_data_index = 0;

    i2c_tx_data[0] = 0b00100000 | 0x04; // Access emissivity at 0x04 in EPPROM
    i2c_tx_data[1] = 0x00; // Lower byte
    i2c_tx_data[2] = 0x00; // Higher byte
    i2c_tx_data[3] = gen_crc(i2c_tx_data, 3); // PEC

    i2c_tx_data_length = 4;
    i2c_rx_data_length = 0;

    i2c_stopped = false;
    i2c_failed = false;
    i2c_need_restart = false;

    EUSCI_B1->IFG &= ~EUSCI_B_IFG_TXIFG0; // Clear the TX ready flag
    EUSCI_B1->IFG &= ~EUSCI_B_IFG_RXIFG0; // Clear the RX ready flag
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR; // Enter TX mode
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

//    get_ir_sensor_emissivity();
    _delay_cycles(15000); // Wait 10ms to be safe

    /* Then send needed value */
    while (EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY)
    {
        // Wait until bus not busy
    }

    i2c_tx_data_index = 0; // Reset index
    i2c_rx_data_index = 0;

    i2c_tx_data[0] = 0b00100100; // Access emissivity at 0x04 in EPPROM
    i2c_tx_data[1] = ((int) round(value * 65535)) & 0x00ff; // Lower byte
    i2c_tx_data[2] = (((int) round(value * 65535)) & 0xff00) >> 8; // Higher byte
    i2c_tx_data[3] = gen_crc(i2c_tx_data, 3); // PEC

    i2c_tx_data_length = 4;
    i2c_rx_data_length = 0;

    i2c_stopped = false;
    i2c_failed = false;
    i2c_need_restart = false;

    EUSCI_B1->IFG &= ~EUSCI_B_IFG_TXIFG0; // Clear the TX ready flag
    EUSCI_B1->IFG &= ~EUSCI_B_IFG_RXIFG0; // Clear the RX ready flag
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR; // Enter TX mode
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // Send START

    while (!i2c_stopped)
    {
        // Then every thing will be handled by the interruption,
        // we wait until the communication finished here
    }
    if (i2c_failed)
    {
        return -2;
    }

    _delay_cycles(15000); // Wait 10ms to be safe
    return 0;
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

    i2c_tx_data[0] = 0b00000111; // Access T_object at 0x07 in RAM
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
        return -3;
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

        if (i2c_tx_data_index < i2c_tx_data_length)
        {
            // Send not yet finished
            EUSCI_B1->TXBUF = i2c_tx_data[i2c_tx_data_index]; // Send data
            i2c_tx_data_index += 1; // Prepare for sending next data
        }
        else
        {
            if (i2c_need_restart)
            {
                // RESTART needed
                i2c_need_restart = false;
                EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_TR; // Change to RX mode
                EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // Send a RESTART
            }
            else
            {
                EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP; // Send STOP
            }
        }
    }

    // Check for RX ready
    if (EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0)
    {
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_RXIFG0; // Clear the RX ready flag

        if (i2c_rx_data_index < i2c_rx_data_length)
        {
            // Read not yet finished
            i2c_rx_data[i2c_rx_data_index] = EUSCI_B1->RXBUF; // Save received data
            i2c_rx_data_index += 1; // Prepare for reading next data
        }
        else
        {
            EUSCI_B1->RXBUF; // Consume values in the RXBUF to allow STOP
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP; // Send STOP
        }
    }
}
