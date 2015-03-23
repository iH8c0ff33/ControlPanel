//Libraries
#include <Wire.h>
//Pin Numbers
//States
//Variables
int i = 0;
int n = 11;
boolean loading = 0;
boolean rising 	= 1;

void setup() {
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	Wire.begin(2);
	Wire.onReceive(showCharge);
}

void loop() {
	delay(100);
}

void showCharge(int howMany) {
	char charge = Wire.read();
	switch(charge) {
		case '0':
			digitalWrite(11, 0);
			digitalWrite(10, 0);
			digitalWrite(9, 0);
			digitalWrite(8, 0);
			digitalWrite(7, 0);
			digitalWrite(6, 0);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '1':
			digitalWrite(11, 1);
			digitalWrite(10, 0);
			digitalWrite(9, 0);
			digitalWrite(8, 0);
			digitalWrite(7, 0);
			digitalWrite(6, 0);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '2':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 0);
			digitalWrite(8, 0);
			digitalWrite(7, 0);
			digitalWrite(6, 0);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '3':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 0);
			digitalWrite(7, 0);
			digitalWrite(6, 0);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '4':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 0);
			digitalWrite(6, 0);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '5':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 0);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '6':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 0);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '7':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 1);
			digitalWrite(4, 0);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '8':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 1);
			digitalWrite(4, 1);
			digitalWrite(3, 0);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case '9':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 1);
			digitalWrite(4, 1);
			digitalWrite(3, 1);
			digitalWrite(2, 0);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case 'a':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 1);
			digitalWrite(4, 1);
			digitalWrite(3, 1);
			digitalWrite(2, 1);
			digitalWrite(1, 0);
			digitalWrite(0, 0);
			break;
		case 'b':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 1);
			digitalWrite(4, 1);
			digitalWrite(3, 1);
			digitalWrite(2, 1);
			digitalWrite(1, 1);
			digitalWrite(0, 0);
			break;
		case 'c':
			digitalWrite(11, 1);
			digitalWrite(10, 1);
			digitalWrite(9, 1);
			digitalWrite(8, 1);
			digitalWrite(7, 1);
			digitalWrite(6, 1);
			digitalWrite(5, 1);
			digitalWrite(4, 1);
			digitalWrite(3, 1);
			digitalWrite(2, 1);
			digitalWrite(1, 1);
			digitalWrite(0, 1);
			break;
	}
}