/*
 * eeprom.h
 *
 * Created: 11/22/2022 8:05:04 PM
 *  Author: AhmedMabrouk
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "std_macros.h"

void eeprom_write(uint8_t, uint16_t);
uint16_t eeprom_read(uint8_t);


#endif /* EEPROM_H_ */