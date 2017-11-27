#include "Ultrasonic.h"
#define echoPin 10 //Pino 10 recebe o pulso do echo
#define trigPin 9 //Pino 9 envia o pulso para gerar o echo

Ultrasonic ultrasonic(9,10);

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

#define TEMPO 500

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

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(SENOP, INPUT);
}

void seguidorDeLinha(){
  static int estado = 0;
  static double forcaD, forcaE, linhaE, linhaD;

  if(valSensor()==0){    
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

void loop() {
  int distancia=calDist();

  if(distancia<=8) desviarObstaculo();
  else seguidorDeLinha();
}

void boost(char side){
  switch(side){
    case 'd': moverMotor(100, -100);
            break;

    case 'e': moverMotor(-100, 100);
            break;        

    case 'f': moverMotor(100, 100);
            break;        
  }

  delay(50);
}

void stopMotors(){
  moverMotor(-50, -50);
  delay(10);
  moverMotor(50, 50);
  delay(10);
  moverMotor(0, 0);
}

void mover(char side, int tempo){
  boost(side);
  
  switch(side){
    case 'd': moverMotor(50, -50);
              break;

    case 'e': moverMotor(-50, 50);
              break;   

    case 'f': moverMotor(50, 50);
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

int valSensor(){
  return int(digitalRead(SENOP));
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
