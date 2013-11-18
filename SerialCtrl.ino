/*
Date	: 13/11/18
Author	: dajiao

now for mega only

protocol:
	^[1][2][3][]...[]$
[1] : 0 for operate a digital pin 
	  1 for operate a analog pin
	  2 for using a SPI	
	  3 
*/

void setup(){
	// init the Serial port 
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial2.begin(9600);
	Serial3.begin(9600);

}

void loop(){
	if(Serial1.available()>0){
			
	}
}


