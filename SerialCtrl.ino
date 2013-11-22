/*
Date	: 13/11/18
Author	: dajiao

now for mega only

protocol:
	^[1][2][3][]...[]$
[1] :	0 for operate a digital pin 
		1 for operate a analog pin
[2] : 	0 for read
	  	1 for write(only support for a digital pin)
[3] :	depend on [1][2]	
		case 00:	$ return format (^00[1]$) which [1] equals the value
		case 01:	0 for normal write(0/1) [4] equals the value to write end with $
					1 for PWM write [4][5] equals the value to write end with $
		case 10:	$ return format (^10[1][2]$) which [1][2] equals the value	
		case 11:	not support write to a analog pin	

return format:
		error: ^error$
*/

char input;
char valueInput[2];
int  pwmValue;

// the lightCtrl System state
boolean state = true;
char pinC;						// current operate pin
int pinI;						// pin in int 
char retString[15];				// string to return

// args for light
boolean DState[54];				// the state of Digital pin true for online
boolean AState[16];				// the state of Analog pin true for online
int 	DValue[54];				// the value to write to a Digital pin
	
void error(){
	Serial.print("^error$");
}

void susseed(){
	Serial.print("^succeed$");
}

// 
void nWrite(){
	Serial.readBytes(&valueInput[0], 1);
	pinMode(pinI, OUTPUT);
	if(valueInput[0] == 1) {
		digitalWrite(pinI, HIGH);
	}else{
		digitalWrite(pinI, LOW);
	}
	DValue[pinI] = 256;
	susseed();
}

void pwmWrite(){
	Serial.readBytes(valueInput, 2);
	pwmValue = valueInput[0]*128 + valueInput[1];	

	// only for pwm pin
	if(pinI>2 && pinI<13){
		analogWrite(pinI, pwmValue/4);
	}else{
		error();
		return;
	}

	DValue[pinI] = pwmValue/4;
	susseed();
}

void writeDpin(){	
	
	Serial.readBytes(&input, 1);
	if(input == '0'){
		nWrite();
	}else if(input == '1'){
		pwmWrite();
	}else {
		error();
	}
}

void pwmRead(){
	if(pinI>2 && pinI<13){
		pwmValue = analogRead(pinI);		
		valueInput[0] = pwmValue/128;
		valueInput[1] = pwmValue%128;
		retString[3] = '1';
		retString[4] = valueInput[0];
		retString[5] = valueInput[1];
		retString[6] = '$';
		retString[7] = '\0';
		Serial.print(retString);
	}else{
		error();
		return;
	}
}

void nRead(){
	if(pinI>2 && pinI<54){
		pinMode(pinI, INPUT);
		valueInput[0] = digitalRead(pinI);
		retString[3] = '0';
		retString[4] = valueInput[0];
		retString[5] = '$';
		retString[6] = '\0';
	}else{
		error();
	}
}

void readDPin(){
	Serial.readBytes(&input, 1);
	retString[1] = pinC;	
	retString[2] = '0';
	if(input == '0'){
		nRead();
	}else if(input == '1'){
		pwmRead();	
	}else{
		error();
	}

}

void DPin(){

	if(!DState){
		error();
		return;
	}

	Serial.readBytes(&input, 1);
	if(input == '0'){
		readDPin();
	}else if(input == '1'){
		writeDPin();
	}else {
		error();
	}
}

void aRead(){
	if(pinI>0 && pinI<16){
		valueInput[0] = analogRead(pinI);
		retString[3] = valueInput[0]/128;
		retString[4] = valueInput[0]%128;
		retString[5] = '$';
		retString[6] = '\0';
		Serial.print(retString);
	}else{
		error();
	}
}

void APin(){
	if(!AState){
		error();
		return;
	}
	
	retString[1] = pinC;
	retString[2] = '1';
	aRead();
}


void getlist(){
	int i;
	int num = 0;
	retString[1] = pinC;
	retString[2] = '2';
	retString[3] = '0';
	for(i = 0; i < 54; i++){
		if(DState[i]){
			if(num < 8){
				num ++;
				retString[3+num] = i;
			}else{
				retString[4+num] = '$';
				retString[5+num] = '\0';
				Serial.print(retString);
				num = 1;
				retString[3+num] = i;
			}	
		}	
	}

	if(num != 0){
		retString[4+num] = '$';
		retString[5+num] = '\0';
		Serial.print(retString);
	}
}

void newPin(){
	DState[pinI] = true;	
	retString[1] = pinC;
	retString[2] = '2';
	retString[3] = '1';
	retString[4] = '$';
	retString[5] = '\0';
	Serial.print(retString);
}

void deletePin(){
	DState[pinI] = false;	
	retString[1] = pinC;
	retString[2] = '2';
	retString[3] = '2';
	retString[4] = '$';
	retString[5] = '\0';
}

void other(){
	Serial.readBytes(&input, 1);	
	switch(input){
		case '0':		getlist();
			break;
		case '1':		newPin();
			break;
		case '0':		deletePin();
			break;
		default:	
			error();
	}
}

void setStateT(){
	state = true;
}

void setStateF(){
	state = false;
}

void setup(){
	// init the Serial port 
	Serial.begin(9600);
	
	// init the return string
	retString[0] = '^';

	// attach interrupt to a state function
	attachInterrupt(0, setStateT, RISING);
	attachInterrupt(0, setStateF, FAllING);
}


void loop(){
	if(Serial.available()>0 && state){
		Serial.readBytes(&input, 1);				
		if(input == '^'){
			Serial.readBytes(&input, 1);
			pinC = input;
			pinI = (int)pinC;
			Serial.readBytes(&input, 1);
			switch(input){
				case '0' :	DPin();
					break;
				case '1' :  APin();
					break;
				case '2' :  other();
					break;
				default:    error();
					break;
			}	
		}
	}
}


