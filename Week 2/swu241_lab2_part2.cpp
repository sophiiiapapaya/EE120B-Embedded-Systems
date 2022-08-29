/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #2  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/Todg_Pfi75w
 */
const int b_size = 4; 
int leds[b_size] = {2,3,4,5}; 
int btns[b_size] = {8,9,10,11}; 

unsigned char LED = 0; 
unsigned short value = 0; 
unsigned short cnt = 0; 

 enum States {
  Start,
  OFF, 
  ON, 
 } state = Start ; 

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
  //this state machine reads the photoresistor every second and turns on the leds if it is "dark"
  state = (States)TickFct(state);
 }

 int TickFct(int state){
   value = analogRead(A0);
   
   //LED = digitalRead(leds[0]) | digitalRead(leds[1]) | digitalRead(leds[2]) | digitalRead(leds[3]); 
   
   // Transitions
   switch(state){
     case Start:
       state = OFF; 
       cnt = 0; 
       break; 
     
     case OFF: 
       delay(value); 
         state = ON;
       break; 
     
     case ON:
       delay(value); 
         state = OFF;
       break;  
   }
   
   // State actions
   switch(state){
     case Start:
       break; 
     
     case OFF: 
       for (int i = 0; i < 4; i++)
         digitalWrite(leds[i], LOW); 
       break; 
     
     case ON:
       for (int i = 0; i < 4; i++)
         digitalWrite(leds[i], HIGH); 
       break; 
   }
   
   return state; 
 }
