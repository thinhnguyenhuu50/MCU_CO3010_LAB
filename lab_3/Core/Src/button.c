#include "button.h"
int KeyReg0 = NORMAL_STATE;
int KeyReg1 = NORMAL_STATE;
int KeyReg2 = NORMAL_STATE;
int KeyReg3 = NORMAL_STATE;

int TimeOutForKeyPress =  500;
int button1_pressed = 0;
int button1_long_pressed = 0;
int button1_flag = 0;

//--------------
#define NUMBER_OF_BUTTONS 1
static struct {
	uint8_t first_state;
	uint8_t second_state;
	uint8_t third_state;
	int count;
} button = {1, 1, 1, 0};

int isButton1Pressed(){
	if(button1_flag == 1){
		button1_flag = 0;
		return 1;
	}
	return 0;
}

int isButton1LongPressed(){
	if(button1_long_pressed == 1){
		button1_long_pressed = 0;
		return 1;
	}
	return 0;
}

void subKeyProcess(){
	//TODO
	//HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	button1_flag = 1;
}

void getKeyInput(){
	KeyReg2 = KeyReg1;
	KeyReg1 = KeyReg0;
	// Add your key
	KeyReg0 = HAL_GPIO_ReadPin(BUTTON_0_GPIO_Port, BUTTON_0_Pin);

	if ((KeyReg1 == KeyReg0) && (KeyReg1 == KeyReg2)){
		if (KeyReg2 != KeyReg3){
			KeyReg3 = KeyReg2;

			if (KeyReg3 == PRESSED_STATE){
				TimeOutForKeyPress = 500;
				//subKeyProcess();
				button1_flag = 1;
			}

		}else{
			TimeOutForKeyPress --;
			if (TimeOutForKeyPress == 0){
				TimeOutForKeyPress = 500;
				if (KeyReg3 == PRESSED_STATE){
					//subKeyProcess();
					button1_flag = 1;
				}
			}
		}
	}
}

//-------------------
uint8_t button_is_pressed() {
	if (button.count == 1) {
		return 1;
	}
	return 0;
}
uint8_t button_is_held(int duration);
void button_scan() {
	button.first_state = button.second_state;
	button.second_state = button.third_state;

	button.third_state = HAL_GPIO_ReadPin(BUTTON_0_GPIO_Port, BUTTON_0_Pin);
	if ((button.first_state == button.second_state) && (button.second_state == button.third_state)){
		if (button.third_state == 0) {
			button.count += 1;
			return;
		}
	}
	button.count = 0;
}

