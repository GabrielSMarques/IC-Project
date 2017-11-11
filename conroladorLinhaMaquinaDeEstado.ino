#define Motor_E1 8
#define Motor_E2 7
#define Motor_EV 6
#define Motor_DV 5
#define Motor_D2 4
#define Motor_D1 3
#define SENOP 2

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

void seguirLinha(){
  static int estado=0;

  switch(estado){
    case 0: if(valSensor()==0) estado=1;
    
            moveMotors(100, 0);
            delay(50);
            break;
            
    case 1: if(valSensor()==0) estado=0;
            
            moveMotors(0, 100);
            delay(50);
            break;
  }
}

void loop() {
  seguirLinha();
}

