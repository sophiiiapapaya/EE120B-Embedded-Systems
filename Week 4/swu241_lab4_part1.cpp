/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #4  Exercise #1
 * Exercise Description: IR remote/receiver, display r,L,u,d,0-9
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/WfBSw-2NyrU
 */
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
int RECV_PIN = 9; //define input pin for the reciever on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results;

//int i = 0;
int j = 0; // display value

int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int leds[7] = {a,b,c,d,e,f,g};


  long disp[15] = { // put in global scope and test if it can be read
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
    0xFFC23D, //r
    0xFF22DD, //L
    0xFF906F, //u
    0xFFE01F, //d
    0xFFA25D  //off
  };
  
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum SM_States {SM_INIT, SM_S0};
int SM_Tick(int state){
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
          Serial.println(results.value, HEX);
          irrecv.resume(); // Receive the next value
        }
        break;
    }

    return state;
}

enum SM2_States {SM2_INIT, SM2_S0}; 
int SM2_Tick(int state2){
  int nums[15][7] = {
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
  switch(state2){
    case SM2_INIT:
      state2 = SM2_S0; 
      break; 
    case SM2_S0:
      state2 = SM2_S0; 
      break; 
    
  }

  // -------------------state actions--------------------------
  switch(state2){
    case SM2_INIT:
      break; 
    case SM2_S0:
      for (j = 0; j < 15; j++){
        if (results.value == disp[j]){
        for (int i = 0; i < 7; i++){
            if (nums[j][i] == 1)
              digitalWrite(leds[i], HIGH); 
            else 
              digitalWrite(leds[i], LOW);
        }
       } 
      }
      
      break; 
  }

  return state2; 
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 7; i++){
    pinMode(leds[i], OUTPUT); 
    digitalWrite(leds[i], LOW); 
  }
  Serial.begin(9600);
  
  irrecv.enableIRIn();
  
  unsigned char i = 0;
  tasks[i].state = SM_INIT;
  tasks[i].period = 1;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

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
