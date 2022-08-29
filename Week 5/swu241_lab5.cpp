/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #5
 * Exercise Description: Demonstrates the use of a 16x2 LCD display, the DHT11 sensor, and the DC motor
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/pg8FOlkQsTY
*/
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <LiquidCrystal.h>
#include <DHT.h>

// -------shared variables-------------
int on = 0; // low if system is off
int up = 0; 
int down = 0; 
int sensed, setTemp; // user set temperature in Fahrenheit

// ---------LED/DC motor output pin setup---------
int led = 13; 
int m1 = 8; 
int m2 = 9; 

// -----IR remote/receiver setup------
int RECV_PIN = 6; //define input pin for the reciever on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results;
long disp[3] = { //IR remote input
    0xFF906F, //u (0)
    0xFFE01F, //d (1)
    0xFFA25D  //off (2)
};

// --------LCD setup----------
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// --------DHT11 setup--------
#define DHTPIN A0     
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 3;
task tasks[tasksNum];

// ----------------------SM1: IR remote/receiver---------------------
enum SM_States {SM_INIT, SM_S0};
int SM_Tick(int state){
  // transitions
    switch(state){ 
      case SM_INIT:
         state = SM_S0;
        break;
      case SM_S0:
        state = SM_S0;
        break;
    }
  // state actions
    switch(state){ 
      case SM_INIT:
        break;
      case SM_S0:
        if (irrecv.decode(&results)) {
          if (results.value == disp[2]){ // power button pressed when system is off
            // if on is true, return 0; if on is false, return 1
            if (on){
              on = 0; 
              // turning system off clears the lcd screen
              lcd.clear(); 
              digitalWrite(led, LOW); 
              digitalWrite(m2, LOW); 
            }
            else if (!on){
              on = 1; 
            }
          } 
          // only push values into q if up or down is pressed
          else if (results.value == disp[0]){ // up/increase pressed
            up = 1; 
          }
          else if (results.value == disp[1]){ // down/decrease pressed
            down = 1; 
          }
          irrecv.resume(); // Receive the next value
        }
        break;
    }

    return state;
}

// ----------------------SM2: Change set temperature---------------------
enum SM2_States {SM2_INIT, waitChange, start, inc, dec};
int SM2_Tick(int state2){
  // transitions
    switch(state2){ 
      case SM2_INIT:
         state2 = waitChange;
        break;
      case waitChange:
        if (!on)
          state2 = waitChange; 
        else if (on){
          state2 = start; 
          setTemp = 75; 
        }
          
        break; 
      case start:
        if (!on)
          state2 = waitChange; 
        if (!up && !down)
          state2 = start; 
        else if (up && !down)
          state2 = inc; 
        else if (!up && down)
          state2 = dec; 
        break; 
      case inc:
        if(up && !down)
          state2 = inc; 
        else if (!up && !down){
          state2 = start;
          setTemp++; 
        }
        break; 
      case dec:
        if(!up && down)
          state2 = dec; 
        else if (!up && !down){
          state2 = start;
          setTemp--; 
        }
        break; 
    }
  // state actions
    switch(state2){ 
      case SM2_INIT:
        break;
      case waitChange:
        break; 
      case start:
        break; 
      case inc:
        up = 0; 
        break; 
      case dec:
        down = 0; 
        break; 
    }

    return state2;
}

// ----------------------SM3: Display & motor control---------------------
enum SM3_States {SM3_INIT, waitOut, AC};
int SM3_Tick(int state3){
  // transitions
  switch(state3){
    case SM3_INIT:
      state3 = waitOut; 
      break; 
    case waitOut:
      if (!on)
        state3 = waitOut; 
      else if (on)
        state3 = AC; 
      break; 
    case AC:
      if (on)
        state3 = AC; 
      else if (!on)
        state3 = waitOut; 
      break; 
  }

  // state actions
  switch(state3){
    case SM3_INIT:
      break; 
    case waitOut:
      break; 
    case AC:
      // print sensed temp and set temp
      lcd.setCursor(0, 0); 
      lcd.print("Temperature: ");
      lcd.print(sensed);

      lcd.setCursor(1,1);
      lcd.print("Set Temp: ");
      lcd.print(setTemp);
      
      if (setTemp == sensed){ 
        // Both LED and DC motor are off
        digitalWrite(led, LOW); 
        //digitalWrite(m1, LOW); 
        digitalWrite(m2, LOW); 
      }
      else if (setTemp > sensed){
        // Heater on, fan off
        digitalWrite(led, HIGH); 
        //digitalWrite(m1, LOW); 
        digitalWrite(m2, LOW); 
      }
      else if (setTemp < sensed){
        // Heater off, fan on
        digitalWrite(led, LOW); 
        //digitalWrite(m1, LOW); 
        digitalWrite(m2, HIGH); 
      }
      break; 
  }
  return state3; 
}


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    
  // Set 8 high, 9 low or 8 low, 9 high turns the motor on
  // Set both to LOW turns the motor off
  pinMode(m1, OUTPUT); // motor 1, pin 8
  pinMode(m2, OUTPUT); // motor 2, pin 9
  pinMode(led, OUTPUT); //LED

  // AC unit (heater/LED & fac/DC motor) off initially
  digitalWrite(led, LOW); 
  digitalWrite(m1, LOW); 
  digitalWrite(m2, LOW); 
  
  Serial.begin(9600);
  
  dht.begin();
  
  irrecv.enableIRIn();

  unsigned char i = 0;
  // IR receive, system on/off
  tasks[i].state = SM_INIT;
  tasks[i].period = 1; // TBD
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM_Tick;
  i++;
  // change set temperature
  tasks[i].state = SM2_INIT;
  tasks[i].period = 5; // TBD
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
  i++;
  // control LCD, DC motor, and led
  tasks[i].state = SM3_INIT;
  tasks[i].period = 10;
  0; // TBD
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM3_Tick;


  delay_gcd = 1; // GCD
}

void loop() {
  float f = dht.readTemperature(true); //Returns temperature in Fahrenheit as a float
  // round float data type: https://www.arduino.cc/reference/en/language/variables/data-types/float/
  sensed = round(f); 
  
  if (isnan(sensed)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
     }
   }
}
