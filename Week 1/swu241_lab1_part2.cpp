/* Sophia Wu swu241@ucr.edu
 * Discussion Section: B21
 * Assignment: Lab #1  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: <URL>
 */

// C++ code
// 
const int b_size = 4; 
int leds[b_size] = {2,3,4,5}; 

// output buffer
int b_buff = 0x00; 

// Serial monitor buffer
int s_buff = 0x00; 

const int s_size = 8; 
int s[s_size]; 

// helper functions

void readData(){
  if (Serial.available()){
    s_buff = Serial.parseInt();  
    Serial.print(s_buff); 
  }  
  resetBuffer(); 
}

void resetBuffer(){
  for (int i = 0; i < b_size; i++){
   digitalWrite(leds[i], LOW);  
  }
}

void writeBuffer(unsigned char b_temp, int size = b_size){ // size = b_size will only check the right 4 bits
  for (int i = (size - 1); i >= 0; i--){
    if ((b_temp >> i) & 0x01)
    	digitalWrite(leds[i], HIGH);
  }
}


void setup()
{
  for (int i = 0; i < 4; i++){
  	pinMode(leds[i], OUTPUT);
  }
  
  Serial.begin(9600); 
}

void loop()
{
  readData(); 
  
  //resetBuffer(); 
  
  b_buff = s_buff; 

  if (b_buff == 0)
    resetBuffer();
  else
	writeBuffer(b_buff);
  
}