#include "includes.h"

bool is_left_pressed_before = false;
bool is_right_pressed_before = false;
bool is_val_pressed_before = false;

void init_button(void)   // Set the initial state of the buttons
{
    P2->SEL0 &= ~BIT7; // Select GPIO function for left button(P2.7)
    P2->SEL1 &= ~BIT7;

    P10->SEL0 &= ~BIT4 & ~BIT5; // Select GPIO function for down button (P10.4) and right button (P10.5)
    P10->SEL1 &= ~BIT4 & ~BIT5;

    P2->SEL0 &= ~BIT6; // Select GPIO function for up button(P2.6)
    P2->SEL1 &= ~BIT6;

    P7->SEL0 &= ~BIT4; // Select GPIO function for valid button (P7.4)
    P7->SEL1 &= ~BIT4;

    // Set the left button
    P2->DIR &= ~BIT7;     // The left button is enter
    P2->REN |= BIT7;      // Enable resistor
    P2->OUT |= BIT7;      // Pull-up resistor

    // Set the down button and right button
    P10->DIR &= ~BIT4;     // The down button is enter
    P10->REN |= BIT4;      // Enable resistor
    P10->OUT |= BIT4;      // Pull-up resistor

    P10->DIR &= ~BIT5;     // The right button is enter
    P10->REN |= BIT5;      // Enable resistor
    P10->OUT |= BIT5;      // Pull-up resistor

    // Set the up button
    P2->DIR &= ~BIT6;     // The up button is enter
    P2->REN |= BIT6;      // Enable resistor
    P2->OUT |= BIT6;      // Pull-up resistor

    // Set the valid button
    P7->DIR &= ~BIT4;     // The valid button is enter
    P7->REN |= BIT4;      // Enable resistor
    P7->OUT |= BIT4;      // Pull-up resistor
}

bool is_left_pressed() // Confirm that the left button is pressed
{
    bool result = false; // Define the initial state of the button as not pressed
    bool now = !((bool) (P2->IN & BIT7 )); // Force the button state to be translated into a boolean type
    if (is_left_pressed_before == false && now == true) // If the state of button was high and now is low, the button is pressed
    {
        result = true;  // Indicates that the left button is pressed
    }
    else
    {
        result = false; // Indicates that the left button is not pressed
    }

    is_left_pressed_before = now; // Transfer the current state to the previous state for the next comparison

    return result;  // Return the state of the button
}

bool is_right_pressed()
{
    bool result = false;
    bool now = !((bool) (P10->IN & BIT5 ));
    if (is_right_pressed_before == false && now == true)
    {
        result = true;
    }
    else
    {
        result = false;
    }

    is_right_pressed_before = now;

    return result;
}

bool is_val_pressed()
{
    bool result = false;
    bool now = !((bool) (P7->IN & BIT4 ));
    if (is_val_pressed_before == false && now == true)
    {
        result = true;
    }
    else
    {
        result = false;
    }

    is_val_pressed_before = now;

    return result;
}

