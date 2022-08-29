/* Sophia Wu swu241@ucr.edu
 * Discussion Section: B21
 * Assignment: Lab #1  Exercise #4
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/Bn0CLMQ2A5k
 */

// C++ code
// 
const int b_size = 4; 
int leds[b_size] = {2,3,4,5}; 
int btn[b_size] = {8,9,10,11}; 
int pos; 

// output buffer
int b_buff = 0x00; 

// Serial monitor buffer
int s_buff = 0x00; 

const int s_size = 8; 
int s[s_size]; 

// helper functions

void btnState(){

  while (digitalRead(btn[0]) == HIGH){
    if (digitalRead(leds[0]) == LOW)
        s_buff = 1; 
    else
        s_buff = 0; 
    pos = 0; 
  } 
  
  while (digitalRead(btn[1]) == HIGH){
    if (digitalRead(leds[1]) == LOW)
        s_buff = 1; 
    else
        s_buff = 0; 
    pos = 1; 
  } 
  
  while (digitalRead(btn[2]) == HIGH){
    if (digitalRead(leds[2]) == LOW)
        s_buff = 1; 
    else
        s_buff = 0; 
    pos = 2; 
  } 
  
  while (digitalRead(btn[3]) == HIGH){
    if (digitalRead(leds[3]) == LOW)
        s_buff = 1; 
    else
        s_buff = 0; 
    pos = 3; 
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
  
  if (b_buff == 1)
  	digitalWrite(leds[pos], HIGH);
  else
    digitalWrite(leds[pos], LOW); 
  
  
}