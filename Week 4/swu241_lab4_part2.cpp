/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #4  Exercise #2
 * Exercise Description: Motor rotates (counter)clockwise, speed up/down
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/RUA52GFeeeI
 */
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
int RECV_PIN = 9; //define input pin for the reciever on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results;

int i = 0; // cnt
int phase = 0; 
int j = 0; // display value
int counter = 0; 
int t; //ticks
int speedVal = 1; // changing value changes the period

int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int leds[7] = {a,b,c,d,e,f,g};
long disp[15] = {
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
const unsigned short tasksNum = 4;
task tasks[tasksNum];

enum rec_States {rec_INIT, rec_S0};
int rec_Tick(int state){
  // --------------------transitions---------------------------
    switch(state){ 
      case rec_INIT:
         state = rec_S0;
        break;
      case rec_S0:
        state = rec_S0;
        break;
    }
  // -------------------state actions--------------------------
    switch(state){ 
      case rec_INIT:
        break;
      case rec_S0:
        if (irrecv.decode(&results)) {
          Serial.println(results.value, HEX);
          irrecv.resume(); // Receive the next value
        }
        break;
    }

    return state;
}

enum dis_States {dis_INIT, dis_S0}; 
int dis_Tick(int state2){
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
    case dis_INIT:
      state2 = dis_S0; 
      break; 
    case dis_S0:
      state2 = dis_S0; 
      break; 
    
  }

  // -------------------state actions--------------------------
  switch(state2){
    case dis_INIT:
      break; 
    case dis_S0:
      for (j = 0; j < 15; j++){
        if (results.value == disp[j]){
        for (i = 0; i < 7; i++){
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

enum speed_States {speed_INIT, speed_wait, speed_u, speed_d};
int speed_Tick(int state3){
  switch (state3){
    case speed_INIT:
      state3 = speed_wait; 
      //speedVal = 5; 
      break; 
    case speed_wait:
      if ((results.value != disp[12]) && (results.value != disp[13])){ // btns up & down
        state3 = speed_wait; 
      }
      else if (results.value == disp[12]){
        if (speedVal > 1)
          speedVal--;
        state3 = speed_u; 
      }
      else if (results.value == disp[13]){
        speedVal++; 
        state3 = speed_d; 
      }
      break; 
    case speed_u:
      if ((results.value != disp[12]) && (results.value != disp[13]))
        state3 = speed_wait; 
      break; 
    case speed_d:
      if ((results.value != disp[12]) && (results.value != disp[13]))
        state3 = speed_wait; 
      break; 
  }
  switch (state3){
    case speed_INIT:
      break; 
    case speed_wait: 
      break; 
    case speed_u:
      Serial.println(speedVal);
      break; 
    case speed_d:
      Serial.println(speedVal);
      break; 
  }

  return state3; 
  
}

enum rotate_States {rotate_INIT, rotate_L, rotate_r};
int rotate_Tick(int state4){
  switch (state4){
    case rotate_INIT:
      state4 = rotate_L; 
      counter = 0; 
      break; 
    case rotate_L:
      counter++; 
        if (counter >= speedVal){
          counter = 0; 
          phase--;
        }
        if (phase < 0)
          phase = 7; 
        state4 = rotate_L;
      if (results.value == disp[10]){ // right btn is pressed
        //k = 0; //step counter
        state4 = rotate_r;
        phase = 0; 
        counter = 0; 
      }
      break; 
    case rotate_r:
      counter++; 
      if (counter >= speedVal){
        counter = 0; 
        phase++;
      } 
      if (phase > 7)
        phase = 0; 
      state4 = rotate_r;
      if (results.value == disp[11]){ // left btn
        //k = 0; 
        state4 = rotate_L;
        phase = 7; 
        counter = 0; 
      }
      
      break; 
  }
  switch (state4){
    case rotate_INIT:
      break; 
    case rotate_L:
      for (i = 0; i < 4; i++){
        if (steps[phase][i] == 1)
          digitalWrite(sig[i], HIGH); 
        else
          digitalWrite(sig[i], LOW); 
      }
      break; 
    case rotate_r:
      for (i = 0; i < 4; i++){
        if (steps[phase][i] == 1)
          digitalWrite(sig[i], HIGH); 
        else 
          digitalWrite(sig[i], LOW); 
      }
      break; 
  }

  return state4; 
  
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
  tasks[i].state = rec_INIT;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &rec_Tick;
  i++;
  tasks[i].state = dis_INIT;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &dis_Tick;
  i++;
  tasks[i].state = speed_INIT;
  tasks[i].period = 1;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &speed_Tick;
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
