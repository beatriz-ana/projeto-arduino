// Universidade Estácio
// Aplicação de cloud, IoT e indústria 4.0 em Python
// Grupo:
// Cecilia Cordeiro da Silva / Matrícula 202001466673 
// Ana Beatriz Barbosa da Silva / Matrícula 202003301949 

#include <Servo.h>

int pos = 0;
int funcao = 0;

Servo servomotor;

const int abrir = 0;
const int fechar = 1;
const int button = 2;
const int buzzer = 11;
const int atencao1 = 6;
const int atencao2 = 5;


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
}

void loop() {
  if (digitalRead(button)==HIGH) {
    if (funcao==abrir) {
      digitalWrite(buzzer,HIGH); // Liga o buzzer
      for (pos = 0; pos <= 90; pos+=1) { // Vai de 0 a 90 graus.
        servomotor.write(pos); // Comanda o servo passando o ângulo.
        digitalWrite(atencao1,LOW); // Acende o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,LOW); // Acende o led laranja.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
      }
      digitalWrite(buzzer,LOW); // Desliga o buzzer
      funcao = fechar;
    }
    else if (funcao==fechar) {
      digitalWrite(buzzer,HIGH); // Liga o buzzer
      for (pos = 90; pos >= 0; pos-=1) { // Vai de 90 a 0 graus.
        servomotor.write(pos); // Comanda o servo passando o ângulo.
        digitalWrite(atencao1,LOW); // Acende o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,LOW); // Acende o led laranja.
        delay(30); // Espera 30ms para o servo chegar na posição.
        digitalWrite(atencao1,HIGH); // Apaga o led vermelho.
        digitalWrite(atencao2,HIGH); // Apaga o led laranja.
      }
      digitalWrite(buzzer,LOW); // Desliga o buzzer
      funcao = abrir;
    }
  }
  
}