/* Sophia Wu (swu241@ucr.edu)
 * Discussion Section: B21
 * Assignment: Lab #2  Exercise #4
 * Exercise Description: [optional - include for your own benefit]
 *        
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 * Demo Link: https://youtu.be/O4GL-cYv6Nw
 */
const int b_size = 4; 
int leds[b_size] = {2,3,4,5}; 
int btns[b_size] = {8,9,10,11}; 

unsigned char btn1, btn2, btn3, btn4;
unsigned char led1, led2, led3, led4;

unsigned char x = 0; 
unsigned short value = 0; 
unsigned short press = 0; 
unsigned short release = 0; 
unsigned short holdtime = 0; 


 enum States {
  Start,
  M1, 
  Blink_L, 
  Blink_H,
  M2, 
  Release,
  M2_R, 
  M2_L, 
  check, 
  calc,
  OFF
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


void resetBuffer(){
  for (int i = 0; i < b_size; i++){
   digitalWrite(leds[i], LOW);  
  }
}

void writeBuffer(unsigned char b_temp, int size = b_size){ // size = b_size will only check the right 4 bits
  for (int i = (size - 1); i >= 0; i--){
    if ((b_temp >> i) & 0x01)
      digitalWrite(leds[i], HIGH);
  }
}


 int TickFct(int state){
   
   btn1 = digitalRead(btns[0]); 
   btn2 = digitalRead(btns[1]); 
   btn3 = digitalRead(btns[2]); 
   btn4 = digitalRead(btns[3]);
   
   led1 = digitalRead(leds[0]); 
   led2 = digitalRead(leds[1]); 
   led3 = digitalRead(leds[2]); 
   led4 = digitalRead(leds[3]);
   
   value = analogRead(A0);
      
   // Transitions
   switch(state){
     case Start:
       state = M1;  
       break; 
     
     case M1:
       if (!btn1 && !btn2 && !btn3 && !btn4)
         state = Blink_L; 
       else if (btn1)
         state = M1; 
       break; 
     
     case Blink_L: 
       if (btn1){
         state = check; 
         press = millis(); 
         x = 1; 
       }
       if (btn2){
         state = M2; 
         resetBuffer(); 
         writeBuffer(1);
       }
       else if (!btn1 && !btn2 && !btn3 && !btn4){
         delay(value); 
         state = Blink_H;
       }
       break; 
     
     case Blink_H:
       if (btn1){
         state = check; 
         press = millis(); 
         x = 1; 
       }
       if (btn2){
         state = M2; 
         resetBuffer(); 
         writeBuffer(1);
       }
       else if (!btn1 && !btn2 && !btn3 && !btn4){
         delay(value); 
         state = Blink_L;
       }
       break; 
     
     case M2:
       if (btn2){
         resetBuffer(); 
         writeBuffer(1);
         state = M2; 
       }
       else if (!btn1 && !btn2 && !btn3 && !btn4)
         state = Release;
       break; 
     
     case Release: 
       if (btn1){
         state = check; 
         press = millis(); 
         x = 2; 
       }
       else if (btn2)
         state = M2; 
       else if (btn3){
         state = M2_R; 
         if (led1 && !led2 && !led3 && !led4){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[3], HIGH); 
         }
         else if (!led1 && led2 && !led3 && !led4){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[0], HIGH);   
         }
         else if (!led1 && !led2 && led3 && !led4){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[1], HIGH); 
         }
         else if (!led1 && !led2 && !led3 && led4){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[2], HIGH); 
         }

       }
       else if (btn4){
         state = M2_L; 
         if (led1){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[1], HIGH); 
         }
         else if (led2){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[2], HIGH);     
         }
         else if (led3){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[3], HIGH); 
         }
     
         else if (led4){
           for (int i = 0; i < 4; i++)
             digitalWrite(leds[i], LOW); 
           digitalWrite(leds[0], HIGH); 
         }
       }
       else if (!btn1 && !btn2 && !btn3 && !btn4)
         state = Release;
       break; 
         
     case M2_R: 
       if (btn3)
         state = M2_R; 
       else if (!btn1 && !btn2 && !btn3 && !btn4)
         state = Release; 
       break; 
     
     case M2_L:
       if (btn4)
         state = M2_L; 
       else if (!btn1 && !btn2 && !btn3 && !btn4)
         state = Release; 
       break; 
     
     case check:
       if (btn1)
         state = check; 
       else if (!btn1 && !btn2 && !btn3 && !btn4){
         state = calc;
         release = millis(); 
         holdtime = press - release; 
       }        
       break; 
     
     case calc:
       if ((!btn1 && !btn2 && !btn3 && !btn4) && (holdtime < 1000))
         state = M1; 
       else if ((!btn1 && !btn2 && !btn3 && !btn4) && (holdtime >= 1000))
         state = OFF; 
       break; 
     
     case OFF:
       if (!btn1 && !btn2 && !btn3 && !btn4)
         state = OFF; 
       else if ((btn1) && (x == 1))
         state = M1; 
       else if ((btn1) && (x == 2)){
         state = M2;
         resetBuffer(); 
         writeBuffer(1);
       }
       break; 

   }
   
   // State actions
   switch(state){
     case Start:
       break; 
     
     case M1:
       break; 
     
     case Blink_L: 
       resetBuffer(); 
       break; 
     
     case Blink_H:
       writeBuffer(15);
       break; 
     
     case M2:
       break; 
       
     case Release:
       break; 
     
     case M2_R:
       break; 
     
     case M2_L:
       break; 
     
     case check:
       break; 
     
     case calc:
       //holdtime = press - release; 
       break; 
     
     case OFF:
       resetBuffer(); 
       break; 

   }
   
   return state; 
 }
