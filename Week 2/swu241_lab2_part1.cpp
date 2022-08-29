/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #2  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/wtFIxTY5YJ8
 */
const int b_size = 4; 
int leds[b_size] = {2,3,4,5}; 
int btns[b_size] = {8,9,10,11}; 

unsigned char btn1, btn2, btn3, btn4;
unsigned char led1, led2, led3, led4;
unsigned char btn = 0; 

 enum States {
  Start,
  Release, 
  ON_P, 
  OFF_P  
 } state = Start ; 

 int TickFct(int state){
   btn1 = digitalRead(btns[0]); 
   btn2 = digitalRead(btns[1]); 
   btn3 = digitalRead(btns[2]); 
   btn4 = digitalRead(btns[3]);
   
   led1 = digitalRead(leds[0]); 
   led2 = digitalRead(leds[1]); 
   led3 = digitalRead(leds[2]); 
   led4 = digitalRead(leds[3]);
   
  // transition
  switch(state){
    case Start:
        state = Release;  
      break; 
    
    case Release:
      if ((btn1 && !led1) || (btn2 && !led2) || (btn3 && !led3) || (btn4 && !led4))
          state = ON_P; 
      else if ((btn1 && led1) || (btn2 && led2) || (btn3 && led3) || (btn4 && led4))
          state = OFF_P; 
      else if (!btn1 && !btn2 && !btn3 && !btn4)
           state = Release; 
          
      break;  
    
    case ON_P:
      if ((btn1 && !led1) || (btn2 && !led2) || (btn3 && !led3) || (btn4 && !led4))
          state = ON_P; 
      else if (!btn1 && !btn2 && !btn3 && !btn4)
          state = Release;

      break;  
    
    case OFF_P: 
      if ((btn1 && led1) || (btn2 && led2) || (btn3 && led3) || (btn4 && led4))
          state = OFF_P; 
      else if (!btn1 && !btn2 && !btn3 && !btn4)
          state = Release;
      break;
    
  }
  // state actions
  switch(state){
    case Release:
      break; 
    
    case ON_P:
    if (btn1 && !led1)
      digitalWrite(leds[0], HIGH);
    
    if (btn2 && !led2)
      digitalWrite(leds[1], HIGH);
    
    if (btn3 && !led3)
      digitalWrite(leds[2], HIGH);
    
    if (btn4 && !led4)
      digitalWrite(leds[3], HIGH);
      break;
    
    case OFF_P:
    if (btn1 && led1)
      digitalWrite(leds[0], LOW);
    
    if (btn2 && led2)
      digitalWrite(leds[1], LOW);
    
    if (btn3 && led3)
      digitalWrite(leds[2], LOW);
    
    if (btn4 && led4)
      digitalWrite(leds[3], LOW);
      break; 
   }
   return state; 
 }
 
 void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 4; i++){
    pinMode(leds[i], OUTPUT);
    pinMode(btns[i], INPUT); 
    digitalWrite(leds[i], LOW); // set all leds low initially
  }

  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  state = (States)TickFct(state); 
}
