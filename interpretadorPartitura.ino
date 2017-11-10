#define ANDAMENTO 100 //Em BPM

#define MAXNOTAS 100 //Quantidade máxima de notas por compasso

#define NUMCOMPASSO 1 //Quantidade de compassos da música

struct Nota{
  int figuraRitmica;
  String nota;
};

struct Compasso{
  int numerador, denominador;
  
  struct Nota notas[MAXNOTAS];
};

struct Compasso compasso[NUMCOMPASSO];

void gerarCompassos(){
  compasso[0].numerador=4;
  compasso[0].denominador=4;

  compasso[0].notas[0].figuraRitmica=4;
  compasso[0].notas[1].figuraRitmica=4;
  compasso[0].notas[2].figuraRitmica=4;
  compasso[0].notas[3].figuraRitmica=4;
  
  compasso[0].notas[0].nota="A";
  compasso[0].notas[1].nota="B#";
  compasso[0].notas[2].nota="Cb";
  compasso[0].notas[3].nota="D";
}

void setup(){
  Serial.begin(9600);

  gerarCompassos();
}

boolean pulse(){
  static unsigned int inicioIntervalo=0;

  if(millis()-inicioIntervalo>=ANDAMENTO*16.6666){
    inicioIntervalo=millis();

    return true;
  }

  return false;
}

void loop(){
  static unsigned int inicioIntervaloPulse=0, contNotas=0;
  static int contPulse=1, contComp=1;

  if(pulse()){
    contPulse++;

    if(contPulse>compasso[contComp-1].numerador){
      contPulse=1;
      contComp++;

      if(contComp>NUMCOMPASSO) contComp=1;        

      contNotas=0;
    }
  }

  if(millis()-inicioIntervaloPulse<=(compasso[contComp-1].denominador/compasso[contComp-1].notas[contNotas].figuraRitmica)*ANDAMENTO*16.6666){
    Serial.println(compasso[contComp-1].notas[contNotas].nota);
  }
  else{
    inicioIntervaloPulse=millis();
    
    contNotas++;
  }

  Serial.println(contPulse);
}

