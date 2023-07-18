/*
 * eeprom.c
 *
 * Created: 11/22/2022 8:04:47 PM
 *  Author: AhmedMabrouk
 */ 

#include "eeprom.h"


/*****************************************************************************
* Function Name: eeprom_write
* Purpose      : Write data on specific address on EEPROM
* Parameters   : Address and data
* Return value : void
*****************************************************************************/
void eeprom_write(uint8_t add, uint16_t data) {
	while(READBIT(EECR,EEWE) != 0);
	EEAR = add;
	EEDR = data;
	SETBIT(EECR,EEMWE);
	SETBIT(EECR,EEWE);
}


/*****************************************************************************
* Function Name: eeprom_read
* Purpose      : Read data from specific address on EEPROM
* Parameters   : Address
* Return value : Short data
*****************************************************************************/
uint16_t eeprom_read(uint8_t add) {
	while(READBIT(EECR,EEWE) != 0);
	EEAR = add;
	SETBIT(EECR,EERE);
	return EEDR;
}