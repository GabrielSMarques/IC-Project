/*#include "Ultrasonic.h"
#define echoPin 10 //Pino 10 recebe o pulso do echo
#define trigPin 9 //Pino 9 envia o pulso para gerar o echo
*/
#define Motor_E1 8
#define Motor_E2 7
#define Motor_EV 6
#define Motor_DV 5
#define Motor_D2 4
#define Motor_D1 3
#define SENOP 2

#define FORCAM 55
#define FORCAm 0
#define FORCAmed 15

void setup(){
   initPins();
}

void initPins(){
  pinMode(Motor_E1, OUTPUT);
  pinMode(Motor_E2, OUTPUT);
  pinMode(Motor_EV, OUTPUT);
  pinMode(Motor_DV, OUTPUT);
  pinMode(Motor_D1, OUTPUT);
  pinMode(Motor_D2, OUTPUT);

  /*pinMode(echoPin, INPUT);
  pinMode(trigPin. OUTPUT);
*/
  pinMode(SENOP, INPUT);
}

void loop() {
  static int estado = 0;
  static double forcaD, forcaE, linhaE, linhaD;
  
  /*
  linhaD = 50, linhaE = 15;
  if(valSensor()==0) {
    moverMotor(linhaE,linhaD);  
  } else {
    moverMotor(linhaD,linhaE);
  }*/

  if(valSensor()==0) {
    moverMotor(linhaE, linhaD);
    while(valSensor()==0);
    switch (estado) {
      case 0:
        //agora ele está na esquerda
        forcaE = FORCAM;
        forcaD = FORCAm;
        linhaE = FORCAM;
        linhaD = FORCAmed;
        //mudar o estado
        estado = 1;
        break;
      case 1:
        //agora ele está na direita
        forcaE = FORCAm;
        forcaD = FORCAM;
        linhaD = FORCAM;
        linhaE = FORCAmed;
        //mudar o estado
        estado = 0;
        break;
    }
  }
  moverMotor(forcaE,forcaD);
}

int valSensor(){
  return int(digitalRead(SENOP));
}
/*
int calDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  int distancia = (ultrasonic.Ranging(CM));

  return distancia;
}*/

void moverMotor(double left, double right){
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
