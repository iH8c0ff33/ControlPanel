//Libraries
#include <Wire.h>
//Pin numbers
const int PSUpin 	= 2;
const int CHG1pin 	= 3;
const int CHG2pin 	= 4;
const int LEDpin 	= 13;
//States
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
//Variables
boolean read0 = 0;
boolean read1 = 0;
int Voltage = 0;
char voltage[3] = 0;
double Vcc;
const double R1 = 1005000;
const double R2 = 9950;

void setup() {
	pinMode(LEDpin, OUTPUT);
	pinMode(PSUpin, OUTPUT);
	pinMode(CHG1pin, OUTPUT);
	pinMode(CHG2pin, OUTPUT);
	Wire.begin(0);
	Wire.onReceive(processRequest);
	delay(200);
	Vcc = readVcc()/1000.0;
}

long readVcc() {
	long result;
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	delay(2);
	ADCSRA |= _BV(ADSC);
	while (bit_is_set(ADCSRA,ADSC));
	result = ADCL;
	result |= ADCH<<8;
	result =11253L / result;
	return result;
}

void loop() {
	if (read0) {
		Wire.beginTransmission(1);
		Voltage = ((analogRead(A0) / 1023.0) * Vcc) * (R2 / (R1 + R2));
		voltage[] = {Voltage / 100 % 10, Voltage / 10 % 10, Voltage % 10};
		Wire.write(voltage[0]);
		Wire.write(voltage[1]);
		Wire.write(voltage[2]);
		Wire.write(";");
		Wire.endTransmission();
	} else if (read1) {
		Wire.beginTransmission(1);
		Voltage = ((analogRead(A1) / 1023.0) * Vcc) * (R2 / (R1 + R2));
		voltage[] = {Voltage / 100 % 10, Voltage / 10 % 10, Voltage % 10};
		Wire.write(voltage[0]);
		Wire.write(voltage[1]);
		Wire.write(voltage[2]);
		Wire.write(";");
		Wire.endTransmission();
	}
	delay(500);
}

void processRequest(int howMany) {
	char request = Wire.read();
	switch(request) {
		case LEDon:
			digitalWrite(LEDpin, 1);
			break;
		case LEDoff:
			digitalWrite(LEDpin, 0);
			break;
		case PSUon:
			digitalWrite(PSUpin, 1);
			break;
		case PSUoff:
			digitalWrite(PSUpin, 0);
			break;
		case CHG1on:
			digitalWrite(CHG1pin, 1);
			break;
		case CHG1off:
			digitalWrite(CHG1pin, 0);
			break;
		case CHG2on:
			digitalWrite(CHG2pin, 1);
			break;
		case CHG2off:
			digitalWrite(CHG2pin, 0);
			break;
		case VOL0on:
			read0 = 1;
			break;
		case VOL0off:
			read0 = 0;
			break;
		case VOL1on:
			read1 = 1;
			break;
		case VOL1off:
			read1 = 0;
			break;
	}

}