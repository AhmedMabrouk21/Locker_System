/*
 * Locker.c
 *
 * Created: 11/22/2022 8:01:37 PM
 * Author : AhmedMabrouk
 */ 

#include "std_macros.h"
#include "LCD.h"
#include "LCD_cfg.h"
#include "KEYPAD.h"
#include "eeprom.h"
#include "Locker.h"

int main(void)
{
	// Initializing Drivers
	
	START:
	LCD_init();
	KEYPAD_init();
	LOCK_DOOR_SENSOR()

	uint8_t KEY_DATA, timer = 0;
	
	// Initialize default password
	
	uint16_t default_password = 0; //0000
	uint16_t user_password;
	
	eeprom_write(DEFAULT_PASSWORD_ADDRESS_1, default_password);
	eeprom_write(DEFAULT_PASSWORD_ADDRESS_2, (default_password>>8));
	
	// Initialize Texts
	
	uint8_t welcome_text[] = "WELCOME :)";
	uint8_t create_password_text[] = "New Password";
	uint8_t old_password_text[] = "Old Password";
	uint8_t enter_password_text[] = "Enter Password";
	uint8_t hint_password_text[] = "----";
	uint8_t correct_password_text[] = "Correct Password";
	uint8_t wrong_password_text[] = "Wrong Password";
	uint8_t much_wrong_password_text[] = "Too Much Trials";
	uint8_t try_again_text[] = "Try Again";
	uint8_t locker_oppened_text[] = "Locker Is Opened";
	
	// End Initialization
	
    while (1) 
    {
		// First Time Program Run
		
		if (eeprom_read(FLAG_ADDRESS) == 255) {
			eeprom_write(PASSWORD_COUNTER, 0);
			LCD_write_command(0x82);
			LCD_write_string(welcome_text);
			_delay_ms(1000);
			LCD_write_command(1);
			
			LCD_write_command(0x82);
			LCD_write_string(create_password_text);
			LCD_write_command(0xc6);
			LCD_write_string(hint_password_text);

			LCD_write_command(0xc6);
			user_password = get_password();
			LCD_write_command(1);
			
			eeprom_write(USER_PASSWORD_ADDRESS_1, user_password);
			eeprom_write(USER_PASSWORD_ADDRESS_2, (user_password>>8));
			
			eeprom_write(FLAG_ADDRESS, 1);
		}
		
		// Enter Password (3 times trials only)
		
		if ( eeprom_read(PASSWORD_COUNTER) < 3) {
			
			LCD_write_command(0x81);
			LCD_write_string(enter_password_text);
			LCD_write_command(0xc6);
			LCD_write_string(hint_password_text);
			
			LCD_write_command(0xc6);
			user_password = get_password();
			LCD_write_command(1);
		
			if ((user_password == READ_PASSWORD(USER_PASSWORD_ADDRESS_1,USER_PASSWORD_ADDRESS_2)) || (user_password == READ_PASSWORD(DEFAULT_PASSWORD_ADDRESS_1,DEFAULT_PASSWORD_ADDRESS_2))) {
				
				LCD_write_string(correct_password_text);
				eeprom_write(PASSWORD_COUNTER, 0);
				_delay_ms(1000);
				break;
				
			} else {
				
				LCD_write_command(0x81);
				LCD_write_string(wrong_password_text);
				LCD_write_command(0xc4);
				LCD_write_string(try_again_text);
				
				BUZZER_EN(1)
				_delay_ms(1000);
				BUZZER_EN(0)
				LCD_write_command(1);
				eeprom_write(PASSWORD_COUNTER, (eeprom_read(PASSWORD_COUNTER) + 1));
				
			}
			
		} else {
			
			// If user entered password 3 times wrong ( Must enter default password )
			
			LCD_write_command(0x80);
			LCD_write_string(much_wrong_password_text);
			
			BUZZER_EN(1)
			_delay_ms(1000);
			BUZZER_EN(0)
			LCD_write_command(1);
			
			while (1) {
				LCD_write_command(0x81);
				LCD_write_string(enter_password_text);
				LCD_write_command(0xc6);
				LCD_write_string(hint_password_text);
				
				LCD_write_command(0xc6);
				user_password = get_password();
				LCD_write_command(1);
			
				if (user_password == READ_PASSWORD(DEFAULT_PASSWORD_ADDRESS_1,DEFAULT_PASSWORD_ADDRESS_2)) {
					eeprom_write(PASSWORD_COUNTER, 0);
					eeprom_write(FLAG_ADDRESS, 255);
					break;
				} else {
					LCD_write_string(much_wrong_password_text);
					BUZZER_EN(1)
					_delay_ms(1000);
					BUZZER_EN(0)
					LCD_write_command(1);
				}		
			}
		}		
    }
	
	// If password is correct and locker's door is opened 
	
	while (1) {
		
		LCD_write_command(1);
		LCD_write_string(locker_oppened_text);
		
		timer = 0;
		
		while(KEYPAD_read() == -1){
			if (READBIT(PINA,1) == 0) { // If locker's door is closed
				goto START;
			}
		}
		KEY_DATA = KEYPAD_read();
		while(KEYPAD_read() != -1){	// long press for 3 second
			timer ++;
			_delay_ms(1000);
			if (timer >= 2) {
				break;
			}
		}

		if (KEY_DATA == '0') { // Allow user to change password
			CHANGE_PASSWORD:
			LCD_write_command(1);
			LCD_write_command(0x82);
			LCD_write_string(old_password_text);
			LCD_write_command(0xc6);
			LCD_write_string(hint_password_text);

			while(KEYPAD_read() != -1){}
			LCD_write_command(0xc6);
			user_password = get_password();
	
			if (user_password == READ_PASSWORD(USER_PASSWORD_ADDRESS_1,USER_PASSWORD_ADDRESS_2)) {
		
				LCD_write_command(1);
				LCD_write_command(0x82);
				LCD_write_string(create_password_text);
				LCD_write_command(0xc6);
				LCD_write_string(hint_password_text);

				LCD_write_command(0xc6);
				user_password = get_password();
				LCD_write_command(1);
				eeprom_write(USER_PASSWORD_ADDRESS_1, user_password);
				eeprom_write(USER_PASSWORD_ADDRESS_2, (user_password>>8));
				
			} else {
				
				LCD_write_command(0x81);
				LCD_write_string(wrong_password_text);
				LCD_write_command(0xc4);
				LCD_write_string(try_again_text);
				
				BUZZER_EN(1)
				_delay_ms(1000);
				BUZZER_EN(0)
				LCD_write_command(1);
				
				goto CHANGE_PASSWORD;
				
			}
		}
	}
}


/*****************************************************************************
* Function Name: get_data
* Purpose      : get data from keypad
* Parameters   : void
* Return value : character
*****************************************************************************/
uint8_t get_data(void) {
	uint8_t key;
	while(KEYPAD_read() == -1){}
	key = KEYPAD_read();
	while(KEYPAD_read() != -1){}
	return key;
}


/*****************************************************************************
* Function Name: STRING_TO_INT
* Purpose      : Convert string to integer
* Parameters   : Address of first element of string
* Return value : Short
*****************************************************************************/
uint16_t STRING_TO_INT(uint8_t *str) {
	uint8_t i=0;
	uint16_t num=0;
	while (i<4) {
		if ((str[i] >= '0') && (str[i] <= '9')) {
			num = num*10 + str[i] -'0';
			i++;
		}
	}
	return num;
}


/*****************************************************************************
* Function Name: get_password
* Purpose      : Allow user to enter password
* Parameters   : void
* Return value : Short
*****************************************************************************/
uint16_t get_password(void) {
	uint8_t i = 0, KEY_DATA, number[4];
	
	while (i<4) {
		KEY_DATA = get_data();
		if ((KEY_DATA >= '0') && (KEY_DATA <= '9')) {
			LCD_write_char('*');
			number[i] = KEY_DATA;
			i++;
		}
	}
	
	return STRING_TO_INT(number);
}