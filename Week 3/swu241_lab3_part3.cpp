/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #3  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/-6Hio3q0xmg
 */
signed short value = 0; 
unsigned short sensorVal, xVal, yVal; 
unsigned long lastRan;
const unsigned long period = 100;
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int dp = 9;
int sw = 10;
int y_pin = 0;
int x_pin = 1;

int leds[8] = {a,b,c,d,e,f,g,dp};

unsigned char x, m, cnt;
unsigned char rate = 10;  

enum States {
  Start, 
  OFF, 
  M1,
  Count,
  UP,
  DOWN, 
  First,
  Delay
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
  //Serial.println(yVal); 
}

int TickFct(int state){
  sensorVal = analogRead(A3); 
  xVal = analogRead(x_pin); 
  yVal = analogRead(y_pin); 
  
  sensorVal = map(sensorVal, 0, 678, 0, 9);
  
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
  // -------------transitions-----------------
  switch(state){
    case Start:
      state = OFF; 
      m = 1;
      break; 

    case OFF:
      if (digitalRead(sw))
        state = OFF; 
      else if (!digitalRead(sw) && (m == 1))
        state = M1; 
      else if (!digitalRead(sw) && (m == 2)){
        state = UP; 
        value = 0; // Count up from 0
      }
      break; 
      
    case M1:
      if (!digitalRead(sw))
        state = Count; 
      else if (digitalRead(sw) && (xVal > 1000)){
        state = UP;        
        value = 0; 
      }
      else if (digitalRead(sw) || xVal < 1000)
        state = M1; 
      

      break; 

    case Count:
      if (!digitalRead(sw))
        state = Count; 
      else if (digitalRead(sw) && (cnt < 10))
        state = M1; 
      else if (digitalRead(sw) && (cnt >= 10)){
        state = OFF; 
        m = 1; 
      }
      break; 
      
    case UP:
      if (!digitalRead(sw) && (m != 2)){
        state = First; 
        x = 1;
      }
      else if (!digitalRead(sw) && (m == 2))
        state = UP; 
      else if (digitalRead(sw) && (xVal < 20))
        state = M1; 
      else if (digitalRead(sw) && (xVal > 20)){
        state = Delay; 
        x = 0; 
        m = 0; 
        // set value
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
      }
      
      break; 
        
    case DOWN:
      if (!digitalRead(sw)){
        state = First; 
        x = 0;
      }
      else if (digitalRead(sw) && (xVal < 20))
        state = M1; 
      else if (digitalRead(sw) && (xVal > 20)){
        state = Delay; 
        x = 1; 
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
      }
      
      break; 

    case First:
      if (!digitalRead(sw))
        state = First; 
      else if (digitalRead(sw) && (cnt < 10) && (x == 0))
        state = UP; 
      else if (digitalRead(sw) && (cnt < 10) && (x == 1))
        state = DOWN; 
      else if (digitalRead(sw) && (cnt >= 10)){
        state = OFF; 
        m = 2; 
      }
      break; 

    case Delay:
      if (digitalRead(sw) && (yVal < 20)){
        rate--;
        if (rate <= 10)
          rate = 10; 
      }else if (digitalRead(sw) && (yVal > 1000)){
        rate++; 
        if (rate >= 50)
          rate = 50; 
      }
      
      if (!digitalRead(sw)){
        if (x == 0)
          x = 1; 
        else if (x == 1)
          x = 0; 
        state = First; 
        cnt = 0; 
      }
      else if (digitalRead(sw) && (xVal < 20))
        state = M1; 
      
      else if (digitalRead(sw) && (x == 1) && (cnt > rate))
        state = DOWN; 
      else if (digitalRead(sw) && (x == 0) && (cnt > rate))
        state = UP; 
      else if (digitalRead(sw) && (cnt <= rate))
        state = Delay;
      
      break; 
  }
  
  // ----------------state actions-------------------
  switch(state){
    case Start:
      break; 

    case OFF:
      for (int i = 0; i < 8; i++){
          digitalWrite(leds[i], LOW); 
      }
      break; 

    case M1:
      for (int i = 0; i < 7; i++){
        if (nums[sensorVal][i] == 1)
          digitalWrite(leds[i], HIGH); 
        else 
          digitalWrite(leds[i], LOW);
      }
      digitalWrite(leds[7], HIGH);
      cnt = 0; 
      break; 

    case Count:
      cnt++; 
      break; 
    
    case UP: 
      cnt = 0; 
      break; 
    
    case DOWN:
      cnt = 0; 
      break; 

    case First:
      cnt++; 
      break; 

    case Delay:
      cnt++; 
      break; 
  }
  return state; 
}
