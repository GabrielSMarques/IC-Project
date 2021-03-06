#include "Ultrasonic.h"

Ultrasonic ultrasonic(9,10);

#define ledPinE 12
#define ledPinD 11

#define echoPin 10
#define trigPin 9

#define Motor_E1 8
#define Motor_E2 7
#define Motor_EV 6
#define Motor_DV 5
#define Motor_D2 4
#define Motor_D1 3
#define SENOP 2

#define TEMPO 500

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

void setup(){
   initPins();
}

void swap(int *val1, int *val2){
  int aux=*val1;

  *val1=*val2;

  *val2=aux;
}

void sort(int vetor[10]){
   for(int i=0; i<10; i++){
      for(int j=0; j<9; j++){
         if(vetor[j]>vetor[j+1]) swap(&vetor[j], &vetor[j+1]);
      }
   }
}

int calDist(boolean filtro){
  int distancia, vetor[10];
  
  if(filtro){
    for(int c1=0; c1<10; c1++){
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);  
  
      vetor[c1] = (int)(ultrasonic.Ranging(CM));
    }

    sort(vetor);

    distancia=vetor[4];
  }
  else{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);  

    distancia = (int)(ultrasonic.Ranging(CM));
  }
  
  return distancia;
}

void moverMotor(char side, double forca){
  if(side=='e'){
    
    if(forca>0){
      digitalWrite(Motor_E1, LOW);
      digitalWrite(Motor_E2, HIGH);
    }else{
      digitalWrite(Motor_E1, HIGH);
      digitalWrite(Motor_E2, LOW);
    } 

     analogWrite(Motor_EV, abs(forca));
     
  }
  else{
    
    if(forca>0){
      digitalWrite(Motor_D1, HIGH);
      digitalWrite(Motor_D2, LOW);
    }else{
      digitalWrite(Motor_D1, LOW);
      digitalWrite(Motor_D2, HIGH);
    }

    analogWrite(Motor_DV, abs(forca));
    
  }
}

void stopMotors(){
  digitalWrite(ledPinE, HIGH);
  digitalWrite(ledPinD, HIGH);

  moverMotor('e', 0);
  moverMotor('d', 0);
}

void boostMotor(char side, char direc){
  if(direc=='f') moverMotor(side, 100);
  else moverMotor(side, -100);

  delay(25);
}

void mover(char side, int tempo){
  switch(side){
    case 'd': moverMotor('d', -30);
              moverMotor('e', 30);
              break;

    case 'e': moverMotor('d', 30);
              moverMotor('e', -30);
              break;   

    case 'f': moverMotor('d', 30);
              moverMotor('e', 30);
              break;             
  }

  delay(tempo);
}

void desviarObstaculo(){
    stopMotors();
  
    boostMotor('e', 't');
    moverMotor('e', -40);
    while(calDist(true)<20);
    delay(500);
    stopMotors();

    boostMotor('e', 'f');
    boostMotor('d', 'f');
    moverMotor('d', 45);
    moverMotor('e', 55);
    delay(1000);
    stopMotors();

    boostMotor('e', 'f');
    moverMotor('e', 45);
    delay(500);
    stopMotors();

    delay(2000);

    boostMotor('e', 'f');
    boostMotor('d', 'f');
    moverMotor('d', 45);
    moverMotor('e', 45);
    delay(500);
    stopMotors();

    delay(2000);

    boostMotor('e', 'f');
    moverMotor('e', 45);
    delay(200);
    stopMotors();

    delay(2000);

    boostMotor('e', 'f');
    boostMotor('d', 'f');
    moverMotor('d', 45);
    moverMotor('e', 55);
    while(valSensor()==1);
    stopMotors();

    boostMotor('d', 'f');
    moverMotor('d', 45);
    delay(200);

    while(valSensor()==1);    
}

int valSensor(){
  return int(digitalRead(SENOP));
}

void seguidorDeLinha(){
  static int estado=0, velOutLine1=0, velOutLine2=65, velInLine1=0, velInLine2=45;
  static boolean darBoost=true;
  static unsigned int tempo=millis();
  
  switch(estado){
    case 0: if(darBoost){
              if(velOutLine2>velOutLine1) boostMotor('d', 'f');
              else boostMotor('e', 'f');
      
              darBoost=false;
            }

            if(millis()-tempo>=500){
              if(velOutLine2>velOutLine1) velOutLine2=35;
              else velOutLine1=35;
            }
            
            moverMotor('e', velOutLine1);
            moverMotor('d', velOutLine2);
            
            if(valSensor()==0){
              estado=1;

              if(velOutLine2>velOutLine1) velOutLine2=45;
              else velOutLine1=45;
            }
            
            break;
            
    case 1: moverMotor('e', velInLine1);
            moverMotor('d', velInLine2);

            if(valSensor()==1){
              estado=0;

              swap(&velOutLine1, &velOutLine2);

              swap(&velInLine1, &velInLine2);

              darBoost=true;

              tempo=millis();
            }
            
            break;
  }
}

void loop() {
  int distancia=calDist(false);

  if(distancia<=8) desviarObstaculo();
  else seguidorDeLinha();
}
