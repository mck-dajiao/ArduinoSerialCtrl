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
*/

void setup(){
	// init the Serial port 
	Serial.begin(9600);
	Serial1.begin(9600);
}

void loop(){
	if(Serial1.available()>0){
				
	}
}


