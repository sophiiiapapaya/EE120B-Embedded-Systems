/* Sophia Wu swu241@ucr.edu
 * Discussion Section: B21
 * Assignment: Lab #1  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/5DQGDrqIDkg
 */

// C++ code
// 
const int b_size = 4; 
int leds[b_size] = {2,3,4,5}; 
int btn[b_size] = {8,9,10,11}; 

// output buffer
int b_buff = 0x00; 

// Serial monitor buffer
int s_buff = 0x00; 

const int s_size = 8; 
int s[s_size]; 

// helper functions

void btnState(){
  while (digitalRead(btn[0]) != LOW){
    s_buff = s_buff + 1;
    if (s_buff > 15)
      s_buff = 0;  
    
    delay(1000); 
  }  
  while (digitalRead(btn[1]) != LOW){
    s_buff = s_buff - 1;
    if (s_buff < 0)
      s_buff = 15;

    delay(1000); 
  }  
  if (digitalRead(btn[2]) == HIGH){
    s_buff = 0; 
    delay(1000); 
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
    pinMode(btn[i], INPUT); 
  }
}

void loop()
{
  btnState(); 
  
  b_buff = s_buff; 

  if (b_buff == 0)
    resetBuffer();
  else
	writeBuffer(b_buff);
  
}