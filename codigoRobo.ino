#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Buzzer
int speakerPin = 13;

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

  pinMode(speakerPin, OUTPUT);
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
  moveMotors(-10, -10);
  delay(100);  
}

void seguirLinha(){
  static int estado=0;
  static boolean outInercia1=true, outInercia2=true, tryCorrec1=false, tryCorrec2=false;

  switch(estado){
    case 0: if(valSensor()==0){
              estado=1;
              
              if(!tryCorrec1){
                moveMotors(60, 0);
                delay(250);
              }
              else{
                moveMotors(60, 0);
                delay(100);
              }
              
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
                moveMotors(0, 60);
                delay(250); 
              }
              else{
                moveMotors(0, 60);
                delay(100); 
              }
              
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

void GameOfThrones(){
  static byte estado=0, cont=0;
  static unsigned int tempo=millis();

  switch(estado){
    case 0: if(millis()-tempo<=500){
              tone(speakerPin, NOTE_G4);
              tempo=millis();
            }
            else if(millis()-tempo<=500){
              tone(speakerPin, NOTE_C4);
              tempo=millis();          
            }
            else if(millis()-tempo<=250){
              tone(speakerPin, NOTE_DS4);
              tempo=millis();          
            }
            else if(millis()-tempo<=250){
              tone(speakerPin, NOTE_F4);
              tempo=millis();
            }
            cont++;
            if(cont>=4){
              cont=0;
              estado=1;
            }
            break;

    case 1: if(millis()-tempo<=500){
              tone(speakerPin, NOTE_G4);
              tempo=millis();
            }
            else if(millis()-tempo<=500){
              tone(speakerPin, NOTE_C4);
              tempo=millis();          
            }
            else if(millis()-tempo<=250){
              tone(speakerPin, NOTE_E4);
              tempo=millis();          
            }
            else if(millis()-tempo<=250){
              tone(speakerPin, NOTE_F4);
              tempo=millis();
            }
            cont++;
            if(cont>=4){
              cont=0;
              estado=2;
            }
            break;

                    
  }
  
  tone(speakerPin, NOTE_G4);
  delay(500);
  noTone(speakerPin);
     
  tone(speakerPin, NOTE_C4);
  delay(500);
  
  tone(speakerPin, NOTE_DS4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_F4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_D4);
  delay(500);
  noTone(speakerPin);
  
  for(int i=0; i<3; i++) {
    tone(speakerPin, NOTE_G3);
    delay(500);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_AS3);
    delay(250);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_C4);
    delay(250);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_D4);
    delay(500);
    noTone(speakerPin);
  }
  
  tone(speakerPin, NOTE_G3);
  delay(500);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_AS3);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_C4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_D4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_F4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_AS3);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_DS4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_D4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_F4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_AS3);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_DS4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_D4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_C4);
  delay(500);
  noTone(speakerPin);
  
  for(int i=0; i<3; i++) {
    tone(speakerPin, NOTE_GS3);
    delay(250);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_AS3);
    delay(250);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_C4);
    delay(500);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_F3);
    delay(500);
    noTone(speakerPin);
  }
  
  tone(speakerPin, NOTE_G4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_C4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_DS4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_F4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_G4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_C4);
  delay(1000);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_DS4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_F4);
  delay(250);
  noTone(speakerPin);
  
  tone(speakerPin, NOTE_D4);
  delay(500);
  noTone(speakerPin);
  
  for(int i=0; i<4; i++) {
    tone(speakerPin, NOTE_G3);
    delay(500);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_AS3);
    delay(250);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_C4);
    delay(250);
    noTone(speakerPin);
    
    tone(speakerPin, NOTE_D4);
    delay(500);
    noTone(speakerPin);
  }
}

void loop() {
  seguirLinha();

  GameOfThrones();
}
