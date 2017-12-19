#include <Servo.h>

Servo servo_R;
Servo servo_L;

const int leftSensor = 10; // Assign sensor pins for two downward sensors
const int rightSensor = 9;
int lsensorState = 0;
int rsensorState = 0;
const int front_sensor = 0;
float distance, voltage;
int p = 0, spin;


//Initialize pins and outputs of sensors and motors
void setup() { 
 servo_R.attach(12); //connect Right motor single wire to Digital Port 12
 servo_L.attach(11); //connect Left motor single wire to Digital Port 11
 pinMode(leftSensor, INPUT); // Declare assigned sensor pins as input
 pinMode(rightSensor, INPUT);
 randomSeed(analogRead(0)); //seed random number generator for spin
 Serial.begin(9600); //front sensor 
}


//Make robot stop and turn when right or left sensors reach the edge of the ring
void stops() {
  while (digitalRead(rightSensor) == HIGH || digitalRead(leftSensor) == HIGH) {
      //move forward
      servo_R.write(90);
      servo_L.write(90);
      delay(100);
    if (digitalRead(leftSensor) == HIGH && digitalRead(rightSensor) == HIGH) {
      servo_R.write(180);
      servo_L.write(0);
      delay(1000);
      servo_R.write(180);
      servo_L.write(180);
      delay(1000);
      }
    //right sensor edge
    else if(digitalRead(rightSensor) == HIGH && digitalRead(leftSensor) == LOW) {
      servo_R.write(180);
      servo_L.write(0);
      delay(700);
      servo_R.write(0);
      servo_L.write(0);
      delay(900);
    }
    //left sensor edge
    else if(digitalRead(leftSensor) == HIGH && digitalRead(rightSensor) == LOW) {
      servo_R.write(180);
      servo_L.write(0);
      delay(700);  
      servo_R.write(180);
      servo_L.write(180);
      delay(900);
    }
  }  //end while
} //end stops

//read input from front sensor (volts)
float getVoltage(int pin) {
 return (analogRead(pin) * 0.004882814);
}

//convert voltage to distance
void volt() {
 voltage = getVoltage(front_sensor); //get voltage reading from front sensor
 distance = 2.439*voltage*voltage - 14.341*voltage + 21.997;
}

//main strategy for sumo bot: continue turning and search for an opponent
//when sensed, charge forward towards opponent while staying within the ring
void loop() {
 rsensorState = digitalRead(rightSensor); //input from side sensors
 lsensorState = digitalRead(leftSensor);

 //get distance from the front sensor
 volt();
  
 while (p==0)     //initially keep turning until opponent is sensed
 {  
  //randomize which direction robot spins to search
  spin = random(1);
  if (spin == 0)
  {
  servo_R.write (180);      
  servo_L.write (180);
  stops();
  volt();
  }
  else if (spin == 1)
  {
  servo_R.write (0);      
  servo_L.write (0);
  stops();
  volt();
  }

  //to attack
  if (distance <= 9.2)   //when bot senses opponent
    {
    p=1;
    }
  } //close while
  
 while (p==1)        //if opponent sensed, go straight -- charge
  {
  servo_R.write (0);
  servo_L.write (180);
  stops();
  volt();
    if (distance > 9.2)   //if bot no longer senses opponent
    {
    p=0;
    }
  }//close while
}//close loop
