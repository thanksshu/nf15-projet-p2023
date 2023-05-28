#include "includes.h"

void command_physical_screen(uint8_t data, bool data_mode)
{

    P2->OUT = (P2->OUT & ~BIT5 ) | (data_mode << 5);
    _delay_cycles(SPI_WAITING_TIME); // Wait for A0 settling down

    while (!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG))
    {
        // Wait until ready to send
    }
    EUSCI_A1->TXBUF = data;  // send data and clear TXIFG
}

void init_physical_screen_communication(void)
{
    /* Initialise UCA1 as SPI for the screen */
    P2->SEL0 |= BIT0 | BIT1 | BIT2 | BIT3; // Use UCA1 with P2.0, P2.1, P2.2, P2.3

    EUSCI_A1->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Enter reset mode (start register configuration)

    EUSCI_A1->CTLW0 |= (EUSCI_A_CTLW0_UCSSEL_2 | // Use SMCLK as clock
            EUSCI_A_CTLW0_SYNC | // Synchrony mode
            EUSCI_A_CTLW0_MST | // Master Mode
            EUSCI_A_CTLW0_CKPL | // Polarity = 1
            EUSCI_A_CTLW0_CKPH | // Phase = 1
            EUSCI_A_CTLW0_MSB);  // Send MSB first

    EUSCI_A1->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Quit reset mode (end register configuration)

    // Start interruption
    NVIC_EnableIRQ(EUSCIA1_IRQn);

    /* Initialise P2.5 as A0 for the screen */
    P2->SEL0 &= ~BIT5;
    P2->SEL1 &= ~BIT5;
    P2->DIR |= BIT5;
    P2->OUT &= ~BIT5;

    /* Initialise P2.4 as RST for the screen */
    P2->SEL0 &= ~BIT4;
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;
    P2->OUT |= BIT4; // Initialise reset
    _delay_cycles(75000);
    P2->OUT &= ~BIT4; // Enter reset mode
    _delay_cycles(75000);
    P2->OUT |= BIT4; // Quit reset mode
}

void init_physical_screen_display()
{
    command_physical_screen(0x40, false);
    command_physical_screen(0xA1, false);
    command_physical_screen(0xC0, false);
    command_physical_screen(0xA6, false);
    command_physical_screen(0xA2, false);
    command_physical_screen(0x2F, false);
    command_physical_screen(0xF8, false);
    command_physical_screen(0x00, false);
    command_physical_screen(0x27, false);
    command_physical_screen(0x81, false);
    command_physical_screen(0x10, false);
    command_physical_screen(0xAC, false);
    command_physical_screen(0x00, false);
}

void turn_physical_screen_on()
{
    command_physical_screen(0xAF, false);
}

void turn_physical_screen_off()
{
    command_physical_screen(0xAE, false);

}

void set_physical_screen_page(int page)
{
    command_physical_screen((0b1011 << 4) | page, false);
}

void set_physical_screen_column(int column)
{
    command_physical_screen((0b0001 << 4) | (column >> 4), false); // Select column MSB 4bit
    command_physical_screen((0b0000 << 4) | (column & (~0b11110000)), false); // Select column LSB 4bit
}

void draw_physical_screen_page_column(uint8_t bitmap)
{
    command_physical_screen(bitmap, true);
}

