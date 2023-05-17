#include "includes.h"

uint8_t RX_Data[50];                                // buffer des donn閑s de r閏eption
uint8_t RX_Nb, RX_Pos;                              // Nombre d'octets et position octet � recevoir
uint8_t TX_Data[50];                               // buffer des donn閑s d'envoi
uint8_t TX_Nb, TX_Pos;                             // Nombre d'octets et position octet � envoyer

uint8_t I2C_Drapeau_STOP;  // drapeau indiquant la fin de la communication
uint8_t I2C_Drapeau_NACK;  // drapeau indiquant un probl鑝e de communication
uint8_t I2C_ReStart;       // n閏閟sit� d'effectuer un restart

void UCB0_I2CMaster_Init(uint8_t ClockSpeed, uint8_t SlaveAddress)
{
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;           // Software reset enabled
    EUSCI_B0->CTLW0 =   EUSCI_B_CTLW0_SWRST |         // Remain eUSCI in reset mode
                        EUSCI_B_CTLW0_MODE_3 |        // I2C mode
                        EUSCI_B_CTLW0_MST |           // Master mode
                        EUSCI_B_CTLW0_SYNC |          // Sync mode
                        EUSCI_B_CTLW0_SSEL__SMCLK;    // SMCLK
    EUSCI_B0->CTLW1 = EUSCI_B_CTLW1_ASTP_0;           // No automatic stop generated
    EUSCI_B0->BRW = ClockSpeed;                       // baudrate = SMCLK / 30 = 400kHz@12MHz 120 100khz
    EUSCI_B0->I2CSA = SlaveAddress;                   // 7 bits slave address
	P1->SEL0 |= BIT6 | BIT7;    					  // Configure P1 Primary Function for I2C
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;          // Release eUSCI from reset


    EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0|EUSCI_B_IFG_TXIFG0|EUSCI_B_IFG_NACKIFG|EUSCI_B_IFG_STPIFG );
    EUSCI_B0->IE |=  EUSCI_B_IE_RXIE|EUSCI_B_IE_TXIE0|EUSCI_B_IE_NACKIE|EUSCI_B_IE_STPIE;

    NVIC_EnableIRQ(EUSCIB0_IRQn);
}

// indique si un NACK a 閠� re鐄
uint8_t UCB0_I2CMaster_GotNack(void) { return I2C_Drapeau_NACK; }

// indique si la lecture est termin閑
uint8_t UCB0_I2CMaster_Previous_ReadBytes_Finished(void) { return I2C_Drapeau_STOP; }

// indique si la lecture index閑 est termin閑
uint8_t UCB0_I2CMaster_Previous_ReadBytesIndexed_Finished(void) { return (RX_Pos >= RX_Nb); }

// indique si l'閏riture est termin閑
uint8_t UCB0_I2CMaster_Previous_WriteBytes_Finished(void) { return I2C_Drapeau_STOP; }

// indique si l'閏riture index閑 est termin閑
uint8_t UCB0_I2CMaster_Previous_WriteBytesIndexed_Finished(void) { return I2C_Drapeau_STOP; }

uint8_t UCB0_I2CMaster_ReadBytes(uint8_t Count)
{
    // remise � z閞o des drapeaux
    I2C_Drapeau_STOP = I2C_Drapeau_NACK = 0;          // mise � 0 des drapeaux
    TX_Nb = 0;                                        // pas d'octets � envoyer
    RX_Nb = Count;                                    // nombre d'octets � recevoir
    RX_Pos = TX_Pos = 0;                              // remise � z閞o des compteurs d'octets
	I2C_ReStart = 0;                                  // pas de restart

    while ((EUSCI_B0->STATW&EUSCI_B_STATW_BBUSY));    // V閞ification que le bus est disponible

    EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_TXIFG0 );  
    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TR);           // Passage du maitre en r閏epteur
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_TXSTT);         // Lancement de la communication

    if(RX_Nb == 1)                                    // Si un seul bit doit 阾re lu
    {
        while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT); // scrutation de l'envoi du START
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;       // envoi du STOP pour arr阾er la lecture
    }
    return 0;
}

uint8_t UCB0_I2CMaster_ReadBytes_Indexed(uint8_t RegAddress, uint8_t Count)
{
    TX_Nb = 1;                                        // envoi d'un octet (le registre � lire)
    RX_Nb = Count;                                    // nombre d'octets � recevoir
    TX_Data[0] = RegAddress;                          // on envoie l'adresse du registre � lire
    RX_Pos = 0;                                       // remise � z閞o du compteur d'octets re鐄s
    TX_Pos = 0;                                       // remise � z閞o du compteur d'octets 閙is
    I2C_Drapeau_STOP = I2C_Drapeau_NACK = 0;          // mise � z閞o des drapeaux
	I2C_ReStart = 1;                                  // n閏閟sit� de restart

    while ((EUSCI_B0->STATW&EUSCI_B_STATW_BBUSY));    // V閞ification que le bus est disponible
    EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_TXIFG0 );
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR | EUSCI_B_CTLW0_TXSTT;  // lancement comm. en TX
    return 0;
}

uint8_t UCB0_I2CMaster_ReadBytes_GetData(uint8_t Count, uint8_t *Data)
{
    uint8_t i;
    for (i = 0; i < Count; i++)
        Data[i] = RX_Data[i];                         // r閏up閞ation des donn閑s re鐄es
    return RX_Pos;                                    // Nombre de bits effectivement lus
}

void UCB0_I2CMaster_WriteBytes_Local(uint8_t Count)
{
    I2C_Drapeau_STOP = I2C_Drapeau_NACK = 0;          // mise � z閞o des drapeaux
    TX_Nb = Count;                                    // nombre d'octets � envoyer
    RX_Nb = 0;                                        // pas d'octets � recevoir
    RX_Pos = TX_Pos = 0;                              // remise � z閞o des compteurs d'octets
	I2C_ReStart = 0;                                  // pas de restart

    while ((EUSCI_B0->STATW&EUSCI_B_STATW_BBUSY));    // V閞ification  bus disponible
    EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0 | EUSCI_B_IFG_TXIFG0);
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_TR + EUSCI_B_CTLW0_TXSTT);  // lancement comm. (TX+START)
}

uint8_t UCB0_I2CMaster_WriteBytes(uint8_t Count, uint8_t *Data)
{
    uint8_t i;
    for (i = 0; i < Count; i++)
        TX_Data[i] = Data[i];	                      // donn閑s � envoyer

	UCB0_I2CMaster_WriteBytes_Local(Count);           // fonction locale d'envoi des donn閑s
	return 0;
}

uint8_t UCB0_I2CMaster_WriteBytesIndexed(uint8_t RegAddress, uint8_t Count, uint8_t *Data)
{
    uint8_t i;
    TX_Data[0] = RegAddress;                          // adresse du registre � lire
    for (i = 0; i < Count; i++)
        TX_Data[i+1] = Data[i];                       // donn閑s � envoyer

	UCB0_I2CMaster_WriteBytes_Local(1 + Count);       // fonction locale d'envoi des donn閑s
    return 0;
}

void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG)          // drapeau NACKIFG : d閒aut de communication
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_NACKIFG;
        I2C_Drapeau_NACK = 1;                         // On indique la r閏eption d'un NACK
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;       // On stoppe la communication
    }

    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0)           // une donn閑 a 閠� re鐄e
    {
		EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG0;         // on efface le drapeau de r閏eption
        RX_Data[RX_Pos] = EUSCI_B0->RXBUF;            // lire RXBUF (met RXIFG0 � 0)
        RX_Pos += 1;

        if ((RX_Nb > 1)&&(RX_Pos == RX_Nb-1))         // reception de l'avant dernier octet
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;   // on stoppe la communication
    }

    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)           // buffer d'envoi disponible
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG0;
        if (TX_Pos < TX_Nb)                           // si des donn閑s doivent 阾re envoy閑s
        {
            EUSCI_B0->TXBUF = TX_Data[TX_Pos];        // 閏rire dans TXBUF (met TXIFG0 � 0)
            TX_Pos += 1;
        }
        else                                          // tout a 閠� envoy�
        {
            if (I2C_ReStart == 0)
            {
				EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP; // on envoie le stop
			}
			else
            {
                EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TR); // Le maitre bascule en RX
                EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // envoi du START
                if(RX_Nb == 1)                          // S'il y a un seul octet � recevoir
                {
                    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT); // on attend la fin du START
                    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;       // on envoie le STOP
                }
            }
        }
    }
    
    if (EUSCI_B0->IFG & EUSCI_B_IFG_STPIFG)			  // drapeau STOP est activ�
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_STPIFG;
        I2C_Drapeau_STOP = 1;                         // on indique la fin de communication
    }
}
