// Universidade Estácio
// Aplicação de cloud, IoT e indústria 4.0 em Python
// Grupo:
// Cecilia Cordeiro da Silva / Matrícula 202001466673 
// Ana Beatriz Barbosa da Silva / Matrícula 202003301949 

#include <Servo.h>  // Biblioteca de servomotor
#include <ESP8266WiFi.h>

int posicao = 0;
int funcao = 0;
int portao = 0;
int portaoFechado = 0;
int portaoAberto = 1;
int pino_trigger = D2;
int pino_echo = D1;

Servo servomotor; // Servo motor
WiFiServer server(80);  // Servidor web na porta 80
WiFiClient client;  // Cliente do servidor

String SSID = "FLOR_1"; // Nome da rede WiFi 
String Senha = "46004600";  // Senha

IPAddress local_IP(192, 168, 1, 115);    // IP estático
IPAddress gateway(192, 168, 1, 1);    // Gateway
IPAddress subnet(255, 255, 0, 0);   // Máscara de sub-rede
IPAddress primaryDNS(8, 8, 8, 8);   // DNS primário
IPAddress secondaryDNS(8, 8, 4, 4);   // DNS secundário

const int abrir = 0;
const int fechar = 1;
const int button = D0;
const int buzzer = D8;
const int pinoMotor = D7;
const int atencao1 = D4;
const int atencao2 = D3;
const int freqAbrir = 500;
const int freqFechar = 1200;


// Função para enviar uma onda de ultrassom no pino trigger e medir o tempo que ela leva para voltar através do pino echo.
// O pino trigger deve ter um pulso com duração de 10 microssegundos.
// O tempo de ida e volta do ultrassom é medido em microssegundos.

long ultrasonic_timing(int triggerPin, int echoPin) {
  // Pino trigger em LOW, antes de começar o pulso.
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Pulso em HIGH por 10 microssegundos
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  // Lê o pino de eco e retorna o tempo em microssegundos.
  return pulseIn(echoPin, HIGH);
}


// Função para abrir o portão

void AbrePortao() {
  int pos;
  tone(buzzer,freqAbrir); // Liga o buzzer.
  for (pos = posicao; pos <= 90; pos+=1) { // Vai de 0 a 90 graus.
    servomotor.write(2*pos); // Comanda o servo passando o ângulo.
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
  noTone(buzzer); // Desliga o buzzer.
  funcao = fechar;
  portao = portaoAberto;
}


// Função para fechar o portão.

void FechaPortao() {
  int pos;
  tone(buzzer,freqFechar); // Liga o buzzer.
  for (pos = posicao; pos >= 0; pos-=1) { // Vai de 90 a 0 graus.      
    long microsec = ultrasonic_timing(pino_trigger,pino_echo); // Pega o tempo em microssegundos que o sinal foi e voltou.
    float distancia = 0.01723*microsec; // Calcula a distância em cm.
    if (distancia < 10) {
      posicao = pos;
      portao = portaoAberto;
      break;
    }
    servomotor.write(2*pos); // Comanda o servo passando o ângulo.
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
    portao = portaoFechado;
  }
  noTone(buzzer); // Desliga o buzzer.
  funcao = abrir;
}


// Configuração inicial

void setup() {
  // Inicializa o servo motor e os pinos do Arduino.
  pinMode(pinoMotor,OUTPUT);
  digitalWrite(pinoMotor,LOW);
  servomotor.attach(pinoMotor); // Servo motor ligado no pino D7
  servomotor.write(0);  // Ajusta posição inicial do servo motor.
  pinMode(button,INPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);
  pinMode(atencao1,OUTPUT);
  digitalWrite(atencao1,HIGH);
  pinMode(atencao2,OUTPUT);
  digitalWrite(atencao2,HIGH);
  pinMode(pino_trigger,OUTPUT);
  pinMode(pino_echo,INPUT);
  funcao = abrir;

  // Conecta o sistema no WiFi:
  Serial.begin(9600); // Velocidade de conexão com a serial
  Serial.println("Iniciando o Sistema de Automação MAXIMUS");
  WiFi.begin(SSID, Senha);
  Serial.print("Conectando à rede ");
  Serial.println(SSID);
  // Configura o endereço de IP estático.
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("A configuração de IP estático falhou.");
  }
  while (WiFi.status() != WL_CONNECTED) {  // Espera 500 ms enquanto a conexão é feita.
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado com o IP: ");
  Serial.println(WiFi.localIP()); // Mostra o endereço IP.
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}


// Loop

void loop() {
  int pos;

  // Controle do portão por botão físico:
  if (digitalRead(button) == HIGH) {
    switch(funcao) {
      case abrir:
      AbrePortao();
      break;

      case fechar:
      FechaPortao();
      break;
    }
  }  

  // Servidor web:
  // Monitora se existe um cliente usando o navegador.
  client = server.available();
  if (!client) {
    return;
  }
  // Servidor fica em estado de espera para alguém enviar o comando.
  Serial.println("Aguardando comandos.");
  while(!client.available()){
    delay(1);
  }
  // AQUI COMEÇA O CÓDIGO HTML junto com a chamada HTTP.
  // Faz a leitura da chamada HTTP e mostra os valores.
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  // Compara o pedido de Chamada HTTP e verifica se bate com o programa que fizemos.
  if (request.indexOf("/abrir") != -1) {
    if (funcao == abrir) {
      AbrePortao();
    }
  }
  if (request.indexOf("/fechar") != -1) {
    if (funcao == fechar) {
      FechaPortao();
    }
  }
  if (request.indexOf("/index") != -1) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
  }
  if (request.indexOf("/") != -1) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
  }
  // Se o Servidor conseguiu entender a chamada que fizemos acima, retorna o valor lido e mostra no navegador. 
  client.println("<!DOCTYPE html>");
  client.println("<html lang='pt-BR'>");
  client.println("<meta charset='UTF-8'>");
  client.println("<head>");
  client.println("    <title>Sistema de Automação MAXIMUS</title>");
  client.println("    <style>");
  client.println("        body {");
  client.println("            font-family: Arial, sans-serif;");
  client.println("            text-align: center;");
  client.println("            margin: 20px;");
  client.println("        }");
  client.println("        h1 {");
  client.println("            color: #333;");
  client.println("        }");
  client.println("        .status {");
  client.println("            font-size: 24px;");
  client.println("            margin: 20px 0;");
  client.println("        }");
  client.println("        .gate {");
  client.println("            width: 100px;");
  client.println("            height: 100px;");
  client.println("            border: 2px solid #333;");
  client.println("            border-radius: 50%;");
  client.println("            margin: 0 auto;");
  client.println("            display: flex;");
  client.println("            justify-content: center;");
  client.println("            align-items: center;");
  client.println("        }");
  client.println("        .gate.open {");
  client.println("            background-color: #ff6347; /* cor para portão aberto */");
  client.println("        }");
  client.println("        .gate.closed {");
  client.println("            background-color: #32cd32; /* cor para portão fechado */");
  client.println("        }");
  client.println("        button {");
  client.println("            padding: 10px 20px;");
  client.println("            font-size: 16px;");
  client.println("            background-color: #3498db;");
  client.println("            color: #fff;");
  client.println("            border: none;");
  client.println("            border-radius: 4px;");
  client.println("            cursor: pointer;");
  client.println("        }");
  client.println(".team-list {");
  client.println("            margin-top: 50px;");
  client.println("            border-top: 1px solid #ccc;");
  client.println("            padding-top: 20px;");
  client.println("            text-align: left;");
  client.println("            opacity: 0.7;");
  client.println("        }");
  client.println("        .team-list ul {");
  client.println("            list-style: none;");
  client.println("            padding: 0;");
  client.println("            margin: 0; /* Adicionando margem zero para alinhar à esquerda */");
  client.println("        }");
  client.println("        .team-list ul li {");
  client.println("            margin-bottom: 10px;");
  client.println("        }");
  client.println("    </style>");
  client.println("</head>");
  client.println("<body>");
  client.println("  <h1>Controle do Portão</h1>");
  client.println("  <br>");
  client.println("  <br>");
  client.println("  <div class='status'>");
  client.println("        <p>Status do Portão:</p>");
  if (portao == portaoAberto) {
    client.println("        <div class='gate.open' id='gateStatus'>Aberto</div>");
    client.println("    </div>");
  }
  else {
    client.println("        <div class='gate.closed' id='gateStatus'>Fechado</div>");
    client.println("    </div>");
  }
  if (funcao == fechar) {
    client.println("    <a href='/fechar'><button>Fechar Portão</button></a>");
  }
  else {
    client.println("    <a href='/abrir'><button>Abrir Portão</button></a>");
  }
  client.println("");
  client.println("    <div class='team-list'>");
  client.println("          <h2>Time de Desenvolvimento:</h2>");
  client.println("          <ul>");
  client.println("              <li>Cecilia Cordeiro da Silva / Matrícula 202001466673</li>");
  client.println("              <li>Ana Beatriz Barbosa da Silva / Matrícula 202003301949</li>");
  client.println("              <li>Universidade Estácio</li>");
  client.println("              <li>Aplicação de cloud, IoT e indústria 4.0 em Python</li>");
  client.println("              <li>Recife, 13 de novembro de 2023</li>");
  client.println("          </ul>");
  client.println("      </div>");
  client.println("  <br>");
  client.println("</body>");
  client.println("</html>");
  delay(10);
  Serial.println("O cliente desconectou.");
  Serial.println("");  

}
