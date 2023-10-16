// Universidade Estácio
// Aplicação de cloud, IoT e indústria 4.0 em Python
// Grupo:
// Cecilia Cordeiro da Silva / Matrícula 202001466673 
// Ana Beatriz Barbosa da Silva / Matrícula 202003301949 

#include <Servo.h>
//#include <Ultrasonic.h>

int posicao = 0;
int funcao = 0;
int pino_trigger = 4;
int pino_echo = 3;

Servo servomotor;

//Ultrasonic ultrasonic(pino_trigger,pino_echo);

const int abrir = 0;
const int fechar = 1;
const int button = 2;
const int buzzer = 11;
const int atencao1 = 6;
const int atencao2 = 5;
const int freqAbrir = 500;
const int freqFechar = 1200;

long ultrasonic_timing(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Pulso em HIGH por 10 microssegundos
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  // Lê o pino de eco e retorna o tempo em microssegundos.
  return pulseIn(echoPin, HIGH);
}

void setup() {
  //servo_9.attach(9, 500, 2500); // Servo motor ligado no pino 9
  servomotor.attach(9); // Servo motor ligado no pino 9
  servomotor.write(1);
  pinMode(button,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(atencao1,OUTPUT);
  digitalWrite(atencao1,HIGH);
  pinMode(atencao2,OUTPUT);
  digitalWrite(atencao2,HIGH);
  pinMode(pino_trigger,OUTPUT);
  pinMode(pino_echo,INPUT);
  funcao = abrir;
}

void loop() {
  int pos;
  
  if (digitalRead(button)==HIGH) {
      switch(funcao) {
      case abrir:
      //digitalWrite(buzzer,HIGH); // Liga o buzzer.
      tone(buzzer,freqAbrir); // Liga o buzzer.
      for (pos = posicao; pos <= 90; pos+=1) { // Vai de 0 a 90 graus.
        servomotor.write(pos); // Comanda o servo passando o ângulo.
        digitalWrite(atencao1,LOW); // Acende o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
        noTone(buzzer); // Desliga o buzzer.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,LOW); // Acende o led laranja.
        tone(buzzer,freqAbrir); // Liga o buzzer.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
        posicao = pos;
      }
      //digitalWrite(buzzer,LOW); // Desliga o buzzer.
      noTone(buzzer); // Desliga o buzzer.
      funcao = fechar;
      break;

      case fechar:
      //digitalWrite(buzzer,HIGH); // Liga o buzzer
      tone(buzzer,freqFechar); // Liga o buzzer.
      for (pos = posicao; pos >= 0; pos-=1) { // Vai de 90 a 0 graus.
        
        //long microsec = ultrasonic.timing(); // Pega o tempo em microssegundos que o sinal foi e voltou.
        //float distancia = ultrasonic.convert(microsec,Ultrasonic::CM); // Calcula a distância em cm.
        long microsec = ultrasonic_timing(pino_trigger,pino_echo); // Pega o tempo em microssegundos que o sinal foi e voltou.
        float distancia = 0.01723*microsec; // Calcula a distância em cm.

        if (distancia < 50) {
          posicao = pos;
          break;
        }
        
        servomotor.write(pos); // Comanda o servo passando o ângulo.
        digitalWrite(atencao1,LOW); // Acende o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
        noTone(buzzer); // Desliga o buzzer.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,LOW); // Acende o led laranja.
        tone(buzzer,freqFechar); // Liga o buzzer.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
        posicao = pos;
      }
      //digitalWrite(buzzer,LOW); // Desliga o buzzer
      noTone(buzzer); // Desliga o buzzer.
      funcao = abrir;
      break;
    }

  }
  
}
