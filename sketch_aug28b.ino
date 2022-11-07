#include <Servo.h>
#include <HCSR04.h>

Servo meuservo;
int echopin = 3;
int trigerpin = 4;
int inA1 = 10;
int inA2 = 11;
int inB1 = 5;
int inB2 = 6;
int BUZZER = 12;
unsigned long pulsetime = 0
unsigned long distancia = 0
int distLimite = 20

void setup() {
  Serial.begin(9600);
  meuservo.attach(9);
  pinMode(inA1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);
  pinMode(trigerpin, OUTPUT);
  pinMode(echopin, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  set_motors(0, 0);
  Serial.println("Parado");
  Alerta(BUZZER, 2000, 100);
  delay(5000);
}

void loop() {
  distancia = LerDistancia ();
  Serial.print("DistF.:");
  Serial.print(distancia);
  Serial.println(" cm");
  if (distancia > distLimite) {
    Serial.println("SEGUE EM FRENTE");
    set_motors(100, 100);
  }
  else {
    Serial.println("PARADO");
    set_motors(0, 0);
    Alerta (BUZZER , 2000, 100);
    int distEsquerda = 0;
    int distDireita = 0;
    meuservo.write(0);
    delay(1000);
    distEsquerda = lerDistancia();
    Serial.print("DistE.:");
    Serial.print(distEsquerda);
    Serial.println(" cm");
    meuservo.write(90);
  }

  if (distEsquerda > distLimite) {
    Serial.println("GIRO A ESQUERDA");
    set_motors(-50, 50);
    delay(200);
    set_motors(0, 0);
  }
  else {
    meuservo.write(180);
    delay(1000);
    distDireita = lerDistancia();
    Serial.print("DistD.:");
    Serial.print(distDireita);
    Serial.println(" cm");
    meuservo.write(90);

    if (distDireita > distLimite) {
      Serial.println("GIRO A DIREITA");
      set_motors(50, -50);
      delay(200);
      set_motors(0, 0);
    }
  }
  if ((distDireita <= distLimite) && (distEsquerda <= distLimite)) {
    Serial.println("GIRO A 180 GRAUS");
    set_motors(50,-50)
    delay(400);
    set_motors(0,0);
    meuservo.write(90);
    }
}
void set_motors(int left_speed, int right_speed){
  if(right_speed >= 0 && left_speed >= 0){
    analogWrite(inA1, 0);
    analogWrite(inA2, right_speed);
    analogWrite(inB1, 0);
    analogWrite(inB2, left_speed);
  }
  if(right_speed >= 0 && left_speed < 0){
    left_speed = -left_speed;
    analogWrite(inA1, 0);
    analogWrite(inA2, right_speed);
    analogWrite(inB1, left_speed);
    analogWrite(inB2, 0);
  }
  if(right_speed < 0 && left_speed >= 0){
    right_speed = -right_speed;
    analogWrite(inA1, right_speed);
    analogWrite(inA2, 0);
    analogWrite(inB1, 0);
    analogWrite(inB2, left_speed);
  } 
}

int lerDistancia(){
  digitalWrite(trigerpin, LOW);        
  digitalWrite(trigerpin, HIGH);      
  delayMicroseconds(10);               
  digitalWrite(trigerpin,LOW);         
  pulsetime = pulseIn(echopin, HIGH);  
  return pulsetime/58;                                                        
}

void Alerta (unsigned char BUZZER, int frequencia, long tempoMilisegundos){
  int Contador, x=0;                                          
  long pausa = (long)(1000000/frequencia);                    
  long loopTempo = (long)((tempoMilisegundos*1000)/(pausa*2));
  while(x<3){                                                 
    for (Contador=0;Contador<loopTempo;Contador++){           
      digitalWrite(BUZZER,HIGH);                              
      delayMicroseconds(pausa);                               
      digitalWrite(BUZZER,LOW);                               
      delayMicroseconds(pausa);                               
    }
    delay(100);                                                
    x++;                                                       
  }
}
