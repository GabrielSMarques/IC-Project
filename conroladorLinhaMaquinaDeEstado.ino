#include "Ultrasonic.h"
#define echoPin 10 //Pino 10 recebe o pulso do echo
#define trigPin 9 //Pino 9 envia o pulso para gerar o echo

#define Motor_E1 8
#define Motor_E2 7
#define Motor_EV 6
#define Motor_DV 5
#define Motor_D2 4
#define Motor_D1 3
#define SENOP 2

#define TEMPO 1000

Ultrasonic ultrasonic(9,10);

void setup(){
  pinMode(echoPin, INPUT); // define o pino 10 como entrada (recebe)
  pinMode(trigPin, OUTPUT); // define o pino 9 como saida (envia)
   initPins();
}

void initPins(){
  pinMode(Motor_E1, OUTPUT);
  pinMode(Motor_E2, OUTPUT);
  pinMode(Motor_EV, OUTPUT);
  pinMode(Motor_DV, OUTPUT);
  pinMode(Motor_D1, OUTPUT);
  pinMode(Motor_D2, OUTPUT);

  pinMode(SENOP, INPUT);
}

int valSensor(){
  return int(digitalRead(SENOP));
}

void moveMotors(double left, double right){
  if(left>0){
    digitalWrite(Motor_E1, LOW);
    digitalWrite(Motor_E2, HIGH);
  }else{
    digitalWrite(Motor_E1, HIGH);
    digitalWrite(Motor_E2, LOW);
  }

  if(right>0){
    digitalWrite(Motor_D1, HIGH);
    digitalWrite(Motor_D2, LOW);
  }else{
    digitalWrite(Motor_D1, LOW);
    digitalWrite(Motor_D2, HIGH);
  }

  analogWrite(Motor_EV, abs(left));
  analogWrite(Motor_DV, abs(right));
}

void stopMotors(){
  moveMotors(-50, -50);
  delay(10);
  moveMotors(50, 50);
  delay(10);
  moveMotors(0, 0);
}

int calDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  int distancia = (ultrasonic.Ranging(CM));

  return distancia;
}

void seguirLinha(){
  static int estado=0;
  static boolean outInercia1=true, outInercia2=true, tryCorrec1=false, tryCorrec2=false;

  switch(estado){
    case 0: if(valSensor()==0){
              estado=1;
              
              //if(!tryCorrec1){
                moveMotors(80, 0);
                delay(50);
              //}
              
              /*else{
                stopMotors();
                delay(250);
                moveMotors(80, 0);
                delay(100);
              }*/
              
              stopMotors();

              outInercia1=true;

              tryCorrec1=false;
            }
            else{
              tryCorrec1=true;
              
              if(outInercia1){
                moveMotors(100, 0);
                delay(50);

                outInercia1=false;
              }
              
              moveMotors(50, 0);
            }
            break;
            
    case 1: if(valSensor()==0){
              estado=0;
              
              if(!tryCorrec2){
                moveMotors(0, 80);
                delay(50); 
              }
              
              /*else{
                stopMotors();
                delay(250);
                moveMotors(0, 80);
                delay(100);
              }*/
              
              stopMotors();

              outInercia2=true;

              tryCorrec2=false;
            }
            else{
              tryCorrec2=true;
              
              if(outInercia2){
                moveMotors(0, 100);
                delay(50);

                outInercia2=false;
              }
              
              moveMotors(0, 50);
            }
            break;
  }
}

void boost(char side){
  switch(side){
    case 'd': moveMotors(100, -100);
            break;

    case 'e': moveMotors(-100, 100);
            break;        

    case 'f': moveMotors(100, 100);
            break;        
  }

  delay(50);
}

void mover(char side, int tempo){
  boost(side);
  
  switch(side){
    case 'd': moveMotors(50, -50);
              break;

    case 'e': moveMotors(-50, 50);
              break;   

    case 'f': moveMotors(50, 60);
              break;             
  }

  delay(tempo);
}

void desviarObstaculo(){
    stopMotors();

    mover('e', 0);
    while(calDist()<20);
    mover('e', TEMPO/5);
    stopMotors();

    mover('f', TEMPO);
    stopMotors();

    mover('d', TEMPO/2);
    stopMotors();

    mover('f', TEMPO);
    stopMotors();

    mover('e', TEMPO/5);
    stopMotors();

    mover('f', 0);
    while(valSensor()==1);
    stopMotors();
}

void loop() {
  int distancia=calDist();

  if(distancia<=8) desviarObstaculo();
  else seguirLinha();
}
