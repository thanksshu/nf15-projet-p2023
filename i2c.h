#include "includes.h"
//Facteur d'horloge Si SMCLK � 12 Mhz
#define I2C_400kHz 30
#define I2C_100kHz 120

// Fonction indiquant si un NACK a �t� re�u
uint8_t UCB1_I2CMaster_GotNack(void);

// Fonction indiquant si la lecture est termin�e
uint8_t UCB1_I2CMaster_Previous_ReadBytes_Finished(void);

// Fonction indiquant si la lecture index�e est termin�e
uint8_t UCB1_I2CMaster_Previous_ReadBytesIndexed_Finished(void);

// Fonction indiquant si l'�criture est termin�e
uint8_t UCB1_I2CMaster_Previous_WriteBytes_Finished(void);

// Fonction indiquant si l'�criture index�e est termin�e
uint8_t UCB1_I2CMaster_Previous_WriteBytesIndexed_Finished(void);

// Fonction d'initialisation de la communication
// ClockFactor : Facteur de division � appliquer sur l'horloge
// SlaveAddress : addresse de l'esclave
void UCB1_I2CMaster_Init(uint8_t ClockSpeed, uint8_t SlaveAddress);

// Fonction d'envoi de donn�es
// RegAdress : adresse 8 bits du registre concern� de l'esclave
// Count : nombre d'octets � envoyer
// Data : tableaux d'octets � envoyer
uint8_t UCB1_I2CMaster_WriteBytes(uint8_t Count, uint8_t *Data);

// Fonction d'envoi de donn�es en index�
// RegAdress : adresse 8 bits du registre concern� de l'esclave
// Count : nombre d'octets � envoyer
// Data : tableaux d'octets � envoyer
uint8_t UCB1_I2CMaster_WriteBytesIndexed(uint8_t RegAddress, uint8_t Count, uint8_t *Data);

// Fonction de r�ception des donn�es
// Count : nombre d'octets � lire
uint8_t UCB1_I2CMaster_ReadBytes(uint8_t Count);

// Fonction de r�ception des donn�es en index�
// RegAdress : adresse 8 bits du registre concern� de l'esclave
// Count : nombre d'octets � lire
uint8_t UCB1_I2CMaster_ReadBytes_Indexed(uint8_t RegAddress, uint8_t Count);

// Fonction de r�cup�ration des donn�es qui ont �t� re�us
// Count : nombre d'octets � r�cup�rer
// Data : Tableau de donn�es re�ues
// valeur de retour : nombre d'octets effectivements r�cup�r�s
uint8_t UCB1_I2CMaster_ReadBytes_GetData(uint8_t Count, uint8_t *Data);

