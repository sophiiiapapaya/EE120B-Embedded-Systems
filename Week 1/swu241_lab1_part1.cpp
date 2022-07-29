/* Sophia Wu swu241@ucr.edu
 * Discussion Section: B21
 * Assignment: Lab #1  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/kBSYPJpr_10
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
    Serial.println(s_buff);
  } 
}

void resetBuffer(){
  for (int i = 0; i < b_size; i++){
   digitalWrite(leds[i], LOW);  
  }
}

void writeBuffer(unsigned char b_temp, int size = b_size){
  digitalWrite(leds[b_temp - 1], HIGH);
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
  
  b_buff = s_buff; 
  
  if ((b_buff == 1) || (b_buff == 2) || (b_buff == 3) || (b_buff == 4)){
    writeBuffer(b_buff);
  }
  if (b_buff == 0)
    resetBuffer();
}

       
       