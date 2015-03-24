/* Libraries */
#include <Arduino.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>
/* Libraries end */

/* Vars */
// Keypad keys
char keys[4][4] = {
	{'1','2','3','a'},
	{'4','5','6','b'},
	{'7','8','9','+'},
	{'*','0','#','-'}
};
// LCD pin and settings
LiquidCrystal lcd(23, 25, 27, 29, 31, 33); // LCD pins(4, 6, 11, 12, 13, 14)
int contrast = 0;
// Pin numbers
const int lcdContrast = 8;
byte rowPins[4] = {30, 32, 34, 36};
byte colPins[4] = {22, 24, 26, 28};
const int buzzerPin = 13;
const int encoderA = 18;
const int encoderB = 19;
const int encoderAInterrupt = 5;
const int encoderBInterrupt = 4;
const int RGBRedPin = 9;
const int RGBGreenPin = 10;
const int RGBBluePin = 11;
const int button = 12;
// Rotary encoder
char state = NULL;
int encoder0PinALast = 0;
int n = LOW;
// Numbers
char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char Char = NULL;
// RGB LED color
unsigned char color[3] = {0, 0, 0};
// Temporary vars
int voltage;
char Voltage[4];
int realVoltage;
int i = 0;
bool curState = false;
bool lastState = true;
// LCD Menu vars
bool dirty = 1;
int currentMenu = 1;
int lastMenu = 1;
int currentMenuItem = 1;
int firstMenuItem = 0;
int lastMenuItem = 0;
bool PSUState = 0;
bool silentMode = 0;
bool LEDState = 0;
bool LOADState = 0;
int curCharge = 0;
// Menus
const char MAINMENU = 1;
const char PSUMENU = 2;
const char SETTINGSMENU = 3;
const char CHARGEMENU = 4;
const char CHGOVERRIDE = 5;
const char LEDOVERRIDE = 6;
const char VOLTAGEMENU = 7;
//Keypad object
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
// I2C Adresses
const char PSU = 0;
const char MEGA = 1;
const char LED = 2;
// Codes
const char LEDon 	= 'a';
const char LEDoff 	= 'b';
const char PSUon	= 'c';
const char PSUoff	= 'd';
const char CHG1on	= 'e';
const char CHG1off	= 'f';
const char CHG2on	= 'g';
const char CHG2off	= 'h';
const char VOL0on	= 'i';
const char VOL0off	= 'j';
const char VOL1on	= 'k';
const char VOL1off	= 'l';
/* Vars end */

void processVoltage(int howMany) {
	while (Wire.available() > 0) {
		voltage = Wire.read();
		int a = voltage;
		Serial.println(a);
		if (voltage != ';') {
			Voltage[i] = voltage;
			i++;
		} else if (voltage == ';') {
			realVoltage = (Voltage[0] * 100 + Voltage[1] * 10 + Voltage[2]);
			i = 0;
		}
	}
}

void charge(char bank) {
	int maxVoltage = 420;
	char turnOn, turnOff, startReading, stopReading;
	if (bank == 1) {
		maxVoltage = 400;
		turnOn = CHG1on;
		turnOff = CHG1off;
		startReading = VOL0on;
		stopReading = VOL0off;
	} else if (bank == 2) {
		maxVoltage = 200;
		turnOn = CHG2on;
		turnOff = CHG2off;
		startReading = VOL1on;
		stopReading = VOL1off;
	} else if (bank == 3) {
		startReading = VOL0on;
		stopReading = VOL0off;
	} else if (bank == 4) {
		startReading = VOL1on;
		stopReading = VOL1off;
	}
	dirty = 1;
	Wire.beginTransmission(PSU);
	Wire.write(startReading);
	Wire.endTransmission();
	Wire.beginTransmission(PSU);
	if (bank == 1 || bank == 2) {
		Wire.write(turnOn);
	}
	Wire.endTransmission();
	do {
		readKeypad();
		if (dirty) {
			lcd.clear();
			lcd.setCursor(0, 0);
			if (bank == 1) {
				lcd.print("Charging RailGun");
			} else if (bank == 2) {
				lcd.print("Charging CoilGun");
			} else if (bank == 3) {
				lcd.print("RailGun Volts");
			} else if (bank == 4) {
				lcd.print("CoilGun Volts");
			}
			lcd.setCursor(0, 1);
			lcd.print("Voltage: ");
		}
		lcd.setCursor(9, 1);
		lcd.print(realVoltage);
		delay(80);
	} while (state == NULL && realVoltage < maxVoltage + 3);
	Wire.beginTransmission(PSU);
	if (bank == 1 || bank == 2) {
		Wire.write(turnOff);
	}
	Wire.endTransmission();
	Wire.beginTransmission(PSU);
	Wire.write(stopReading);
	Wire.endTransmission();
	dirty = 1;
	delay(100);
}
/* Arduino setup */
void setup() {
	delay(500);
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
	// Pin setup
	pinMode(lcdContrast, OUTPUT);
	pinMode(buzzerPin, OUTPUT);
	pinMode(button, INPUT);
	// LCD setup
	analogWrite(lcdContrast, contrast);
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Starting");
	lcd.setCursor(0, 1);
	lcd.print("Please Wait...");
	// Rotary encoder setup
	pinMode(encoderA, INPUT);
	pinMode(encoderB, INPUT);
	digitalWrite(encoderA, 1);
	digitalWrite(encoderB, 1);
	digitalWrite(button, 1);
	// Serial setup
	Serial.begin(9600);
	// I2C setup
	Wire.begin(MEGA);
	Wire.onReceive(processVoltage);
	Wire.beginTransmission(PSU);
	Wire.write(LEDon);
	LEDState = 1;
	Wire.endTransmission();
	// LCD Menu setup
	currentMenuItem = 1;
}

void loop() {
	displayMenu(currentMenu);
}
/* Arduino setup  end */



/* Input reading */
// Keypad
void readKeypad() {
	state = kpd.getKey();
	if (state != NULL) {
		beep(80);
	}
	curState = digitalRead(button);
	if (curState == 0 && lastState != curState) {
		state = 'a';
	}
	lastState = curState;
	n = digitalRead(encoderA);
   	if ((encoder0PinALast == LOW) && (n == HIGH)) {
     	if (digitalRead(encoderB) == LOW) {
       		state = '+';
     	} else {
       		state = '-';
     	}
   } 
   encoder0PinALast = n;
}
/* Input reading end */



/* Buzzer beep */
void beep(int duration) {
	if (!silentMode) {
		digitalWrite(buzzerPin, 1);
		delay(duration);
		digitalWrite(buzzerPin, 0);
	}
}
/* Buzzer beep end */



/* LCD Menu */
void displayMenu(int menu) {
	switch(menu) {
		case MAINMENU:
			mainMenu();
			break;
		case PSUMENU:
			PSUMenu();
			break;
		case SETTINGSMENU:
			settingsMenu();
			break;
		case CHARGEMENU:
			chargeMenu();
			break;
		case CHGOVERRIDE:
			CHGOverride();
			break;
		case LEDOVERRIDE:
			LEDOverride();
			break;
		case VOLTAGEMENU:
			voltageMenu();
			break;
	}
}

void switchMenu(int menu) {
	dirty = 1;
	lastMenu = currentMenu;
	currentMenuItem = 1;
	currentMenu = menu;
}




/* Menu Items */
void displayMenuItem(int item) {
	clearPrintMenuTitle(currentMenu);
	lcd.setCursor(0, 1);
	Serial.print("displaying item: ");
	Serial.print(item);
	Serial.print("\n");
	switch(currentMenu) {
		case MAINMENU:
			switch(item) {
				case 1:
					lcd.print("1. Charge");
					break;
				case 2:
					lcd.print("2. PSU control");
					break;
				case 3:
					lcd.print("3. CHG override");
					break;
				case 4:
					lcd.print("4. LED override");
					break;
				case 5:
					lcd.print("5. Check voltage");
					break;
				case 6:
					lcd.print("6. Settings");
					break;
			}
			break;
		case PSUMENU:
			switch(item) {
				case 1:
					lcd.print("1. Switch PSU");
					break;
				case 2:
					lcd.print("2. Switch LED");
					break;
			}
			break;
		case SETTINGSMENU:
			switch(item) {
				case 1:
					lcd.print("1. Silent mode");
					break;
				case 2:
					lcd.print("2. LCD contrast");
					break;
			}
			break;
		case CHARGEMENU:
			switch(item) {
				case 1:
					lcd.print("RailGun");
					break;
				case 2:
					lcd.print("CoilGun");
					break;
			}
			break;
		case CHGOVERRIDE:
			switch(item) {
				case 1:
					lcd.print("2. RailGun 400V");
					break;
				case 2:
					lcd.print("1. CoilGun 200V");
					break;
			}
			break;
		case LEDOVERRIDE:
			switch(item) {
				case 1:
					lcd.print("1. Simulate CHG");
					break;
				case 2:
					lcd.print("2. Switch load");
					break;
			}
			break;
		case VOLTAGEMENU:
			switch(item) {
				case 1:
					lcd.print("1. RailGun");
					break;
				case 2:
					lcd.print("2. CoilGun");
					break;
			break;
		}
	}
}

void selectMenuItem(int item) {
	clearPrintMenuTitle(currentMenu);
	lcd.setCursor(0, 1);
	switch(currentMenu) {
		case MAINMENU:
			switch(item) {
				case 1:
					switchMenu(CHARGEMENU);
					break;
				case 2:
					switchMenu(PSUMENU);
					break;
				case 3:
					switchMenu(CHGOVERRIDE);
					break;
				case 4:
					switchMenu(LEDOVERRIDE);
					break;
				case 5:
					switchMenu(VOLTAGEMENU);
					break;
				case 6:
					switchMenu(SETTINGSMENU);
					break;
			}
			break;
		case PSUMENU:
			switch(item) {
				case 1:
					PSUState = selectOption("Switch PSU", "ON", "OFF", !PSUState);
					switchPSU(PSUState);
					break;
				case 2:
					LEDState = selectOption("Switch LED", "ON", "OFF", !LEDState);
					if (LEDState) {
						Wire.beginTransmission(PSU);
						Wire.write(LEDon);
						Wire.endTransmission();
					} else {
						Wire.beginTransmission(PSU);
						Wire.write(LEDoff);
						Wire.endTransmission();
					}
					break;
			}
			break;
		case SETTINGSMENU:
			switch(item) {
				case 1:
					silentMode = selectOption("Silent Mode", "ON", "OFF", !silentMode);
					break;
				case 2:
					contrast = numberSelect("Contrast", contrast, 0, 255);
					analogWrite(lcdContrast, contrast);
					break;
			}
			break;
		case CHARGEMENU:
			switch(item) {
				case 1:
					charge(item);
					break;
				case 2:
					charge(item);
					break;
			}
			break;
		case CHGOVERRIDE:
			switch(item) {
				case 1:
					if (selectOption("Charge RailGun?", "Yes", "No", false)) {
						override(item);
					}
					break;
				case 2:
					if (selectOption("Charge CoilGun?", "Yes", "No", false)) {
						override(item);
					}
					break;
			}
			break;
		case LEDOVERRIDE:
			switch(item) {
				case 1:
					overrideCharge();
					Wire.beginTransmission(LED);
					Wire.write('0');
					Wire.endTransmission();
					break;
				case 2:
					LOADState = selectOption("Turn load", "On", "Off", !LOADState);
					break;
			}
			break;
		case VOLTAGEMENU:
			switch(item) {
				case 1:
					charge(3);
					break;
				case 2:
					charge(4);
					break;
			}
			break;
	}
}
/* Menu Items end */



/* Select functions */
// On Off
bool selectOption(char name[32], char trueOpt[3], char falseOpt[3], bool currOpt) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(name);
	bool tmpOpt = currOpt;
	state = NULL;
	while(state != 'a' && state != 'b') {
		if (dirty) {
			lcd.clear();
			dirty = 0;
		}
		lcd.setCursor(0, 0);
		lcd.print(name);
		lcd.setCursor(1, 1);
		lcd.print(trueOpt);
		lcd.setCursor(5, 1);
		lcd.print(falseOpt);
		if (tmpOpt) {
			lcd.setCursor(0, 1);
		} else {
			lcd.setCursor(4, 1);
		}
		lcd.print(".");
		readKeypad();
		if (state == '+' || state == '-') {
			tmpOpt = !tmpOpt;
			dirty = 1;
			state = NULL;
		}
		delay(20);
	}
	dirty = 1;
	if (state == 'a') {
		return tmpOpt;
	} else if (state == 'b') {
		return currOpt;
	}
}
// Number
int numberSelect(char name[32], int curNumber, int minNumber, int maxNumber) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(name);
	int digits[3] = {0, 0, 0};
	char digit = 0;
	while(digit != 3 && digit != -1) {
		if (dirty) {
			lcd.clear();
			dirty = 0;
		}
		lcd.setCursor(0, 0);
		lcd.print(name);
		lcd.setCursor(0, 1);
		if (digit == 0) {
			lcd.print(".");
			lcd.print(digits[0]);
			lcd.print(digits[1]);
			lcd.print(digits[2]);
		} else if (digit == 1) {
			lcd.print(digits[0]);
			lcd.print(".");
			lcd.print(digits[1]);
			lcd.print(digits[2]);
		} else if (digit == 2) {
			lcd.print(digits[0]);
			lcd.print(digits[1]);
			lcd.print(".");
			lcd.print(digits[2]);
		}
		readKeypad();
		if (state == '+') {
			dirty = 1;
			digits[digit]++;
			state = NULL;
		} else if (state == '-') {
			dirty = 1;
			digits[digit]--;
			state = NULL;
		} else if (state == 'a') {
			dirty = 1;
			digit++;
			state = NULL;
		} else if (state == 'b') {
			dirty = 1;
			digit--;
			state = NULL;
		} else if (state != NULL) {
			dirty = 1;
			digits[digit] = state - '0';
			state = NULL;
		}
		if (digits[digit] > 9) {
			digits[digit] = 9;
		} else if (digits[digit] < 0) {
			digits[digit] = 0;
		}
		delay(20);
	}
	dirty = 1;
	int tmpNum = digits[0] * 100 + digits[1] * 10 + digits[2];
	if (digit == 3) {
		if (tmpNum > maxNumber || tmpNum < minNumber) {
			beep(1000);
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Out of Bounds!");
			lcd.setCursor(0, 1);
			lcd.print("Max:");
			lcd.print(maxNumber);
			lcd.print(" Min:");
			lcd.print(minNumber);
			state = NULL;
			while(state != 'a' && state != 'b') {
				readKeypad();
			}
			return curNumber;
		} else {
			return tmpNum;
		}
	} else {
		return curNumber;
	}
}
/* Select functions end */



void clearPrintMenuTitle(int menu) {
	lcd.clear();
	lcd.setCursor(0, 0);
	switch(menu) {
		case MAINMENU:
			lcd.print("Main Menu");
			break;
		case PSUMENU:
			lcd.print("PSU Control");
			break;
		case SETTINGSMENU:
			lcd.print("Settings");
			break;
		case CHARGEMENU:
			lcd.print("Charge Bank");
			break;
		case CHGOVERRIDE:
			lcd.print("Override Charge");
			break;
		case LEDOVERRIDE:
			lcd.print("Override LEDs");
			break;
		case VOLTAGEMENU:
			lcd.print("Read Voltage");
	}
}

void cleanUp() {
	if (dirty) {
		clearPrintMenuTitle(currentMenu);
		displayMenuItem(currentMenuItem);
		dirty = 0;
	}
}

void isOutOfBounds() {
	if (currentMenuItem < firstMenuItem) {
		currentMenuItem = lastMenuItem;
		displayMenuItem(currentMenuItem);
	}
	if (currentMenuItem > lastMenuItem) {
		currentMenuItem = firstMenuItem;
		displayMenuItem(currentMenuItem);
	}
}

void changeMenuItem() {
	if (state != NULL) {
		switch(state) {
			case '+': // UP
				currentMenuItem--;
				displayMenuItem(currentMenuItem);
				break;
			case '-': // DOWN
				currentMenuItem++;
				displayMenuItem(currentMenuItem);
				break;
			case 'a': // SELECT
				selectMenuItem(currentMenuItem);
				break;
			case 'b': // BACK
				switchMenu(lastMenu);
				break;
		}
	}
}



/* Menus */
void mainMenu() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 6;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}

void PSUMenu() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 2;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}

void settingsMenu() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 2;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}

void chargeMenu() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 2;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}

void CHGOverride() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 2;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}

void LEDOverride() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 2;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}

void voltageMenu() {
	// Set Menu First and Last Item
	firstMenuItem = 1;
	lastMenuItem = 2;
	// Clean up title if messed up (dirty)
	cleanUp();
	// Read Keypad state
	readKeypad();
	// Return to top or bottom
	isOutOfBounds();
	// Go up or down
	changeMenuItem();
}
/* Menus end */
/* LCD Menu end */

void switchPSU(bool powerOn) {
	if (powerOn) {
		Wire.beginTransmission(PSU);
		Wire.write(PSUon);
		Wire.endTransmission();
	} else {
		Wire.beginTransmission(PSU);
		Wire.write(PSUoff);
		Wire.endTransmission();
	}
}

void override(char bank) {
	// Print WARNING message
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Press any key to");
	lcd.setCursor(0, 1);
	lcd.print("stop charging!!!");
	// Start charging
	Wire.beginTransmission(PSU);
	if (bank == 1) { //RailGun
		Wire.write(CHG1on);
	} else if (bank == 2) { // CoilGun
		Wire.write(CHG2on);
	}
	Wire.endTransmission();
	// Read keypad
	do {
		readKeypad();
	} while (state == NULL);
	// Stop charging
	Wire.beginTransmission(PSU);
	if (bank == 1) { // CoilGun
		Wire.write(CHG1off);
	} else if (bank == 2) { // RailGun
		Wire.write(CHG2off);
	}
	Wire.endTransmission();
}

void overrideCharge() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Set Charge");
	lcd.setCursor(0, 1);
	do {
		readKeypad();
		if (dirty) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Set Charge");
			lcd.setCursor(0, 1);
			lcd.print(curCharge);
			dirty = 0;
		}
		if (state == '+') {
			curCharge++;
			dirty = 1;
		} else if (state == '-') {
			curCharge--;
			dirty = 1;
		}
		if (curCharge > 12) {
			curCharge = 12;
		} else if (curCharge < 0) {
			curCharge = 0;
		}
		Wire.beginTransmission(LED);
		if (curCharge == 10) {
			Wire.write('a');
		} else if (curCharge == 11) {
			Wire.write('b');
		} else if (curCharge == 12) {
			Wire.write('c');
		} else {
			Char = numbers[curCharge];
			Wire.write(Char);
		}
		Wire.endTransmission();
		delay(80);
	} while (state != 'a' && state != 'b');
	dirty = 1;
}