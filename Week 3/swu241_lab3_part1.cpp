/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #3  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/0gAzY0fKHxo
 */

unsigned short sensorVal = 0; 
unsigned short value = 0; 
unsigned long lastRan;
const unsigned long period = 1000;
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int dp = 9;
int sw = 10;

int leds[8] = {a,b,c,d,e,f,g,dp};

enum States {
  Start, 
  S0, 
} state = Start; 

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++){
    pinMode(leds[i], OUTPUT); 
    digitalWrite(leds[i], LOW); 
  }
  pinMode(sw, INPUT_PULLUP);
  Serial.begin(9600);
  lastRan = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  state = (States)TickFct(state); 
  Serial.println(analogRead(A3)); 
  delay(100);
}

int TickFct(int state){
  value = analogRead(A3); 
  value = map(value, 0, 670, 0, 9);

  int nums[10][7] = {
    {1,1,1,1,1,1,0}, //0
    {0,1,1,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,1,0}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,1,0,1,1}  //9
  };
  // transitions
  switch(state){
    case Start:
      state = S0; 
      break; 
    case S0:
      state = S0; 
      break; 
  }
  
  // state actions
  switch(state){
    case Start:
      break; 
    case S0:
      for (int i = 0; i < 7; i++){
        if (nums[value][i] == 1)
          digitalWrite(leds[i], HIGH); 
        else 
          digitalWrite(leds[i], LOW);
      
      }
      digitalWrite(leds[7], HIGH); 
      break; 
  }
  return state; 
}
