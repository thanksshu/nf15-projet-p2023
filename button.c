#include "includes.h"


bool is_button_left_pressed_before = false;
bool is_button_left_pressed_now;


bool is_button_right_pressed_before = false;
bool is_button_right_pressed_now;


bool is_button_val_pressed_before = false;
bool is_button_val_pressed_now;

void init_button(void)
{
    // choix de la fonction GPIO pour le bouton P2.7(le bouton left)
    P2->SEL0 &= ~BIT7; // on met P2SEL0 � 0 pour P2.7
    P2->SEL1 &= ~BIT7; // on met P2SEL1 � 0 pour P2.7

    // choix de la fonction GPIO pour le bouton P10.4(le bouton down) et P10.5(le bouton right)
    P10->SEL0 &= ~BIT4 & ~BIT5; // on met P10SEL0 � 0 pour P10.4 et P10.5
    P10->SEL1 &= ~BIT4 & ~BIT5; // on met P10SEL1 � 0 pour P10.4 et P10.5

    // choix de la fonction GPIO pour le bouton P2.6(le bouton up)
    P2->SEL0 &= ~BIT6; // on met P2SEL0 � 0 pour P2.6
    P2->SEL1 &= ~BIT6; // on met P2SEL1 � 0 pour P2.6

    // choix de la fonction GPIO pour le bouton P7.4(le bouton valid)
    P7->SEL0 &= ~BIT4; // on met P7SEL0 � 0 pour P7.4
    P7->SEL1 &= ~BIT4; // on met P7SEL1 � 0 pour P7.4

    // reglage du bouton P2.7(le bouton left)
    P2->DIR &= ~BIT7;     //P2.7 entr�e
    P2->REN |= BIT7;      //R�sistance de pull autoris�e sur P2.7
    P2->OUT |= BIT7;      //Pull-up sur P2.7

    // reglage du bouton bouton P10.4(le bouton down) et P10.5(le bouton right)
    P10->DIR &= ~BIT4;     //P10.4 entr�e
    P10->REN |= BIT4;      //R�sistance de pull autoris�e sur P10.4
    P10->OUT |= BIT4;      //Pull-up sur P10.4

    P10->DIR &= ~BIT5;     //P10.5 entr�e
    P10->REN |= BIT5;      //R�sistance de pull autoris�e sur P10.5
    P10->OUT |= BIT5;      //Pull-up sur P10.5

    // reglage du bouton P2.6(le bouton up)
    P2->DIR &= ~BIT6;     //P2.6 entr�e
    P2->REN |= BIT6;      //R�sistance de pull autoris�e sur P2.6
    P2->OUT |= BIT6;      //Pull-up sur P2.6

    // reglage du bouton P7.4(le bouton valid)
    P7->DIR &= ~BIT4;     //P7.4 entr�e
    P7->REN |= BIT4;      //R�sistance de pull autoris�e sur P7.4
    P7->OUT |= BIT4;      //Pull-up sur P7.4

    // relage de l'interruption pour le bouton P2.6(le bouton up) et bouton P2.7(le bouton left)
    P2->IES &= ~BIT6;      //l'interruption se produit sur un front montant
    P2->IE = BIT6;        //on autorise des interruptions uniquement sur P2.6
    P2->IFG = 0;          //on efface les drapeaux d'interruption sur P2

    P2->IES &= ~BIT7;      //l'interruption se produit sur un front montant
    P2->IE = BIT7;        //on autorise des interruptions uniquement sur P2.7
    P2->IFG = 0;          //on efface les drapeaux d'interruption sur P2

    NVIC_EnableIRQ(PORT2_IRQn); //autorisation NVIC pour le Port 2

}

// fonction d'interruption (ISR) pour le port 2
void PORT2_IRQHandler()
{
    if (P2->IFG & BIT6)
    {

        P2->IFG &= ~BIT6; // on efface le drapeau d'interruption pour P2.6

    }
    if (P2->IFG & BIT7)
    {

        P2->IFG &= ~BIT7; // on efface le drapeau d'interruption pour P2.7(bouton left)

    }
}
//when user pressed the button, we have to know the user had down the opration
//now we shoulde know the button left(P2.7) right(P10.5) and valide(P7.4) are pressed (active at niveau low)

bool is_left_pressed()
{
    bool result = false;
    bool is_button_left_pressed = !((bool) (P2->IN & BIT7));
    if (is_button_left_pressed_before == false && is_button_left_pressed_now == true)
    {
        result = true;
    }
    else
    {
        result = false;
    }

    is_button_left_pressed_before = is_button_left_pressed_now;

    return result;
}

