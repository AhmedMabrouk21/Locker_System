/*
 * Locker.h
 *
 * Created: 11/24/2022 9:06:29 PM
 *  Author: Mahmoud
 */ 


#ifndef LOCKER_H_
#define LOCKER_H_


#define FLAG_ADDRESS 1
#define DEFAULT_PASSWORD_ADDRESS_1 10
#define DEFAULT_PASSWORD_ADDRESS_2 11
#define USER_PASSWORD_ADDRESS_1 20
#define USER_PASSWORD_ADDRESS_2 21
#define PASSWORD_COUNTER 30

// Read Password from 2 EEPROM Registers
#define READ_PASSWORD(x,y) (eeprom_read(x) | (eeprom_read(y)<< 8))

// Initialize Buzzer Pin
#define BUZZER_EN(x) if(x==1) SETBIT(PORTA,0); else CLRBIT(PORTA,0);

// Lock door sensor
#define LOCK_DOOR_SENSOR() SETBIT(PORTA,1);

// Functions decleration
uint8_t get_data(void);
uint8_t long_press_get_data(void);
uint16_t STRING_TO_INT(uint8_t *str);
uint16_t get_password(void);

#endif /* LOCKER_H_ */