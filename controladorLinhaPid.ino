#define Motor_E1 8
#define Motor_E2 7
#define Motor_EV 6
#define Motor_DV 5
#define Motor_D2 4
#define Motor_D1 3
#define SENOP 2

#define SETPOINT 0
#define KP 50
#define KD 0

#define VELR 50
#define VELL 50

#define INT 100

#define INTERRORINIT 500

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

void changeSide(double error, int *side){
  static unsigned int timeError=0, intError=INTERRORINIT;

  if(error!=0){
    if(millis()-timeError>=intError){
      *(side)*=-1;

      intError*=2;
      
      timeError=millis();
    }
  }else{
    timeError=millis();

    intError=INTERRORINIT;
  }
}

void pid(){
  static int side=1;
  static double error=0, preError=0, P=0, D=0, PD=0;
  static unsigned int t1=0;
  
  error=(SETPOINT-valSensor())*side;
  
  changeSide(error, &side);
  
  P=error*KP;
  
  D=(error-preError)*KD;
  if(millis()-t1>=INT){
    preError=error;

    t1=millis();
  }

  PD=P+D; 
  
  moveMotors(VELR+PD, VELL-PD);
}

void loop() {
  pid();
}
