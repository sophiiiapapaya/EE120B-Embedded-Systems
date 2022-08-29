/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #4  Exercise #3
 * Exercise Description: 
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/71SjDKUc6hc
 */
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
int RECV_PIN = 9; //define input pin for the reciever on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results;

int comp = 0; // compare value
int k = 0; 
int i = 0; // cnt
int j = 0; // display value
int phase = 0; 
int counter = 0; 
int t = 0; //ticks 
int speedVal = 1; // changing value changes the period

int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int leds[7] = {a,b,c,d,e,f,g};
long disp[15] = { //IR remote
    0xFF6897, //0
    0xFF30CF, //1
    0xFF18E7, //2
    0xFF7A85, //3
    0xFF10EF, //4
    0xFF38C7, //5
    0xFF5AA5, //6
    0xFF42BD, //7
    0xFF4AB5, //8
    0xFF52AD, //9
    0xFFC23D, //r (10)
    0xFF22DD, //L (11)
    0xFF906F, //u (12)
    0xFFE01F, //d (13)
    0xFFA25D  //off (14)
};

// set code to 2477
long setCode[4] = {disp[2], disp[4], disp[7], disp[7]};
long entered[4]; 

int IN1 = 10;
int IN2 = 11;
int IN3 = 12;
int IN4 = 13;
int sig[4] = {IN1,IN2,IN3,IN4};
int steps[8][4] { //codes for the motor to turn properly
  {0,0,0,1},
  {0,0,1,1},
  {0,0,1,0},
  {0,1,1,0},
  {0,1,0,0},
  {1,1,0,0},
  {1,0,0,0},
  {1,0,0,1},
};

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 3;
task tasks[tasksNum];

enum SM_States {SM_INIT, SM_S0};
int SM_Tick(int state){
  int nums[15][7] = { // 7 segment display
    {1,1,1,1,1,1,0}, //0
    {0,1,1,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,1,0}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,1,0,1,1}, //9
    {1,0,0,0,1,1,0}, //r (10)
    {0,0,0,1,1,1,0}, //L (11)
    {0,1,1,1,1,1,0}, //u (12)
    {0,1,1,1,1,0,1}, //d (13)
    {0,0,0,0,0,0,0}  //OFF
  };
  // --------------------transitions---------------------------
    switch(state){ 
      case SM_INIT:
         state = SM_S0;
        break;
      case SM_S0:
        state = SM_S0;
        break;
    }
  // -------------------state actions--------------------------
    switch(state){ 
      case SM_INIT:
        break;
      case SM_S0:
        if (irrecv.decode(&results)) {
          //Serial.println(results.value, HEX);
          for (j = 0; j < 15; j++){
            if (results.value == disp[j]){
              // store value and increment t only if results.value == disp[]
              
              entered[t] = results.value; 
              Serial.println(entered[t], HEX);
              Serial.println(setCode[t], HEX);
              t++;
              // show number on the 7-segment display
              for (i = 0; i < 7; i++){
                if (nums[j][i] == 1)
                  digitalWrite(leds[i], HIGH); 
                else 
                 digitalWrite(leds[i], LOW);
              }
            } 
          }
          //if (t >= 4)
            //t = 0; 
          irrecv.resume(); // Receive the next value
        }
        break;
    }

    return state;
}

enum comp_States {comp_INIT, compare};
int comp_Tick(int state2){
  // transitions
  switch (state2){
    case comp_INIT:
      state2 = compare; 
      comp = 0;
      break; 
    case compare:
      state2 = compare; 
      break; 
  }
  // state actions
  switch (state2){
    case comp_INIT:
      break;  
    case compare:
      if (t == 4){
        t = 0; 
        for (i = 0; i < 4; i++){
          if (entered[i] == setCode[i])
            t++; 
        }
        if (t == 4)
          comp = 1; 
        else 
          comp = 0; 
        t = 0; 
      }
      break; 
  }

  return state2; 
  
}

enum rotate_States {rotate_INIT, lock, unlock};
int rotate_Tick(int state3){
  switch (state3){
    case rotate_INIT:
      state3 = lock; 
      break; 
    case lock:
      if ((k >= 256) && (comp == 1)){
        state3 = unlock; 
        comp = 0; 
        phase = 0; 
        k = 0; 
      }
      else
        state3 = lock;
      break; 
    case unlock:
      if ((k >= 256) && (comp == 1)){
        state3 = lock;
        comp = 0; 
        phase = 7; 
        k = 0; 
      } 
      else
        state3 = unlock; 
      break; 
  }
  switch (state3){
    case rotate_INIT:
      break; 
    case lock:
      if (comp == 1){
          phase--;
          if (phase < 0){
            phase = 7; 
            k++; 
          }
        for (i = 0; i < 4; i++){
          if (steps[phase][i] == 1)
            digitalWrite(sig[i], HIGH); 
          else
            digitalWrite(sig[i], LOW); 
        }
      }
      break; 
    case unlock:
      if (comp == 1){
           phase++;
          if (phase > 7){
            phase = 0; 
            k++; 
          }
        for (i = 0; i < 4; i++){
          if (steps[phase][i] == 1)
            digitalWrite(sig[i], HIGH); 
          else 
            digitalWrite(sig[i], LOW); 
        }
      }

      break; 
  }

  return state3; 
  
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 7; i++){
    pinMode(leds[i], OUTPUT); 
    digitalWrite(leds[i], LOW); 
  }
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  Serial.begin(9600);
  
  irrecv.enableIRIn();
  
  unsigned char i = 0;
  tasks[i].state = SM_INIT;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM_Tick;
  i++;
  tasks[i].state = comp_INIT;
  tasks[i].period = 1;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &comp_Tick;
  i++;
  tasks[i].state = rotate_INIT;
  tasks[i].period = 1;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &rotate_Tick;

  delay_gcd = 1; // GCD
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
     }
   }
}
