/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #3  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/oEn5EXobIXw
 */
 
signed short value = 0; 
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

unsigned char x; 

enum States {
  Start, 
  UP,
  Press,
  DOWN
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
  if ((millis() - lastRan) > period){
    state = (States)TickFct(state); 
    lastRan = millis(); 
  }
  Serial.println(value); 
}

int TickFct(int state){
  //value = analogRead(A3); 
  //value = map(value, 0, 678, 0, 9);
  
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
      state = UP; 
      x = 0;
      break; 
    
    case UP:
      if (digitalRead(sw) && (x == 0))
        state = UP; 
      else if (!digitalRead(sw)){
        state = Press; 
        x = 1;
      }
      break; 
    
    case Press:
      if (!digitalRead(sw))
        state = Press; 
      else if (digitalRead(sw) && (x == 0))
        state = UP; 
      else if (digitalRead(sw) && (x == 1))
        state = DOWN; 
      break; 
        
    case DOWN:
      if (digitalRead(sw) && (x == 1))
        state = DOWN; 
      else if (!digitalRead(sw)){
        state = Press; 
        x = 0; 
      }
      break; 
  }
  
  // state actions
  switch(state){
    case Start:
      break; 
    
    case UP: 
      if (value > 9){
        value = 0; 
        if (digitalRead(leds[7]) == HIGH)
          digitalWrite(leds[7], LOW);
        else
          digitalWrite(leds[7], HIGH); 
      }
      for (int i = 0; i < 7; i++){
        if (nums[value][i] == 1)
          digitalWrite(leds[i], HIGH); 
        else 
          digitalWrite(leds[i], LOW);
      }
      value++; 
      break; 
    
    case Press:
      break; 
    
    case DOWN:
      
      if (value < 0){
        value = 9; 
        if (digitalRead(leds[7]) == HIGH)
          digitalWrite(leds[7], LOW);
        else
          digitalWrite(leds[7], HIGH); 
      }
      for (int i = 0; i < 7; i++){
        if (nums[value][i] == 1)
          digitalWrite(leds[i], HIGH); 
        else
          digitalWrite(leds[i], LOW);
      }
      value--; 
      break; 
  }
  return state; 
}
