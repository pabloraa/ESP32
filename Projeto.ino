#include <WiFi.h> // Inclui biblioteca WiFi.h
#include <ESP32Servo.h> // Inclui biblioteca ESP32Servo.h
#include <DHT.h> // Inclui biblioteca DHT.h
 
ESP32Servo motorgaragem;
 
DHT dht(5, DHT11);
 
float t; // Inicia Variáveld e temperatura
float h; // Inicia variável de umidade
 
// Dados para conexão Wi-Fi
const char* ssid = "USINA 01"; // Inserir nome da rede
const char* password = "usina105601"; // Inserir senha da rede
 
WiFiServer server(80);
 
// Dados para definição de IP Fixo
IPAddress local_IP(192, 168, 1, 130);
IPAddress gateway(192, 168, 0, 4);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1,1 ,1 ,1 );
IPAddress secondaryDNS(2, 8, 8, 8);
 
String header;
 
String cozinha = "Desligada";
String sala = "Desligada";
String garagem = "Desligada";
String varanda = "Desligada";
String quarto = "Desligada";
String sotao = "Desligada";
String porta = "Fechada";
String alarme = "Desativado";
 
// Define os pinos utilizados pelo ESP32
const int luzsala = 4;
const int luzvaranda = 12;
static const int servo = 13;
const int luzgaragem = 14;
const int luzcozinha = 15;
const int luzquarto = 16;
const int luzsotao = 17;
 
#define pir 18
#define som 19
#define sinalalarme 2
 
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
int angulo = 116;
int i;
 
void setup() {
Serial.begin(115200);
dht.begin();
 
// Define os pinos como saída de sinal
pinMode(luzcozinha, OUTPUT);
pinMode(luzsala, OUTPUT);
pinMode(luzgaragem, OUTPUT);
pinMode(luzvaranda, OUTPUT);
pinMode(luzquarto, OUTPUT);
pinMode(luzsotao, OUTPUT);
pinMode(som, OUTPUT);
pinMode(sinalalarme, OUTPUT);
pinMode(pir, INPUT);
 
motorgaragem.attach(servo);
 
// Inicia os pinos em nível baixo (LOW)
digitalWrite(luzcozinha, LOW);
digitalWrite(luzsala, LOW);
digitalWrite(luzgaragem, LOW);
digitalWrite(luzvaranda, LOW);
digitalWrite(luzquarto, LOW);
digitalWrite(luzsotao, LOW);
digitalWrite(som, HIGH);
digitalWrite(sinalalarme, LOW);
 
motorgaragem.write(116, 60);
 
Serial.begin(115200); // Define o baudrate utilizado para comunicação serial
 
if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
Serial.println("STA Failed to configure");
}
 
Serial.println("Conectando a ");
Serial.println(ssid);
 
//Conecta à Rede Wifi indicada anteriormente
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
 
Serial.println("");
Serial.println("WiFi conectado ..!");
Serial.print("IP obtido: ");
Serial.println(WiFi.localIP());
 
server.begin();
Serial.println("Servidor HTTP iniciado");
}
 
void loop() {
WiFiClient client = server.available();
 
if (client) {
currentTime = millis();
previousTime = currentTime;
 
Serial.println("New Client.");
String currentLine = "";
 
while (client.connected() && currentTime - previousTime <= timeoutTime) {
currentTime = millis();
 
if (client.available()) {
char c = client.read();
Serial.write(c);
header += c;
 
if (c == '\n') {
 
if (currentLine.length() == 0) {
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
 
if (header.indexOf("GET /cozinha/ligado") >= 0) {
cozinha = "Ligado";
digitalWrite(luzcozinha, HIGH);
}
 
else if (header.indexOf("GET /cozinha/desligado") >= 0) {
cozinha = "Desligado";
digitalWrite(luzcozinha, LOW);
}
 
else if (header.indexOf("GET /sala/ligado") >= 0) {
sala = "Ligado";
digitalWrite(luzsala, HIGH);
}
 
else if (header.indexOf("GET /sala/desligado") >= 0) {
sala = "Desligado";
digitalWrite(luzsala, LOW);
}
 
else if (header.indexOf("GET /garagem/ligado") >= 0) {
garagem = "Ligado";
digitalWrite(luzgaragem, HIGH);
}
 
else if (header.indexOf("GET /garagem/desligado") >= 0) {
garagem = "Desligado";
digitalWrite(luzgaragem, LOW);
}
 
else if (header.indexOf("GET /varanda/ligado") >= 0) {
varanda = "Ligado";
digitalWrite(luzvaranda, HIGH);
}
 
else if (header.indexOf("GET /varanda/desligado") >= 0) {
varanda = "Desligado";
digitalWrite(luzvaranda, LOW);
}
 
else if (header.indexOf("GET /quarto/ligado") >= 0) {
quarto = "Ligado";
digitalWrite(luzquarto, HIGH);
}
 
else if (header.indexOf("GET /quarto/desligado") >= 0) {
quarto = "Desligado";
digitalWrite(luzquarto, LOW);
}
 
else if (header.indexOf("GET /sotao/ligado") >= 0) {
sotao = "Ligado";
digitalWrite(luzsotao, HIGH);
}
 
else if (header.indexOf("GET /sotao/desligado") >= 0) {
sotao = "Desligado";
digitalWrite(luzsotao, LOW);
}
 
else if (header.indexOf("GET /porta/aberta") >= 0) {
porta = "Aberta";
motorgaragem.write(16, 60);
}
 
else if (header.indexOf("GET /porta/fechada") >= 0) {
porta = "Fechada";
motorgaragem.write(116, 60);
}
 
else if (header.indexOf("GET /alarme/ativado") >= 0) {
alarme = "Ativado";
digitalWrite(sinalalarme, HIGH);
Serial.println("Alarme Acionado");
}
 
else if (header.indexOf("GET /alarme/desativado") >= 0) {
alarme = "Desativado";
digitalWrite(sinalalarme, LOW);
}
 
h = dht.readHumidity();
t = dht.readTemperature();
 
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
 
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
 
client.println(".button { background-color: #B84F4F; border: none; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".button2 {background-color: #4CAF50;}</style></head>");
 
client.println("<body><h1>CONTROLE PARA CASA</h1>");
client.println("<hr>");
client.println("<hr>");
client.println("<h3>Temperatura: " + (String)t + "</h3>");
client.println("<h3>Umidade: " + (String)h + "</h3>");
 
client.println("<hr>");
client.println("<h3>Luz Cozinha - " + cozinha + "</h3>");
if (cozinha == "Ligado") {
client.println("<p><a href=\"/cozinha/desligado\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/cozinha/ligado\"><button class=\"button button2\">LIGAR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Luz Sala - " + sala + "</h3>");
if (sala == "Ligado") {
client.println("<p><a href=\"/sala/desligado\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/sala/ligado\"><button class=\"button button2\">LIGAR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Luz Garagem - " + garagem + "</h3>");
if (garagem == "Ligado") {
client.println("<p><a href=\"/garagem/desligado\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/garagem/ligado\"><button class=\"button button2\">LIGAR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Luz Varanda - " + varanda + "</h3>");
if (varanda == "Ligado") {
client.println("<p><a href=\"/varanda/desligado#varanda\" name=\"varanda\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/varanda/ligado#varanda\" name=\"varanda\"><button class=\"button button2\">LIGAR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Luz Quarto - " + quarto + "</h3>");
if (quarto == "Ligado") {
client.println("<p><a href=\"/quarto/desligado#quarto\" name=\"quarto\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/quarto/ligado#quarto\" name=\"quarto\"><button class=\"button button2\">LIGAR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Luz Sotao - " + sotao + "</h3>");
if (sotao == "Ligado") {
client.println("<p><a href=\"/sotao/desligado#sotao\" name=\"sotao\"><button class=\"button\">DESLIGAR</button></a></p>");
} else {
client.println("<p><a href=\"/sotao/ligado#sotao\" name=\"sotao\"><button class=\"button button2\">LIGAR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Porta Garagem - " + porta + "</h3>");
if (porta == "Aberta") {
client.println("<p><a href=\"/porta/fechada#porta\" name=\"porta\"><button class=\"button\">FECHAR</button></a></p>");
} else {
client.println("<p><a href=\"/porta/aberta#porta\" name=\"porta\"><button class=\"button button2\">ABRIR</button></a></p>");
}
 
client.println("<hr>");
client.println("<h3>Alarme - " + alarme + "</h3>");
if (alarme == "Ativado") {
client.println("<p><a href=\"/alarme/desativado#alarme\" name=\"alarme\"><button class=\"button\">DESATIVAR</button></a></p>");
} else {
client.println("<p><a href=\"/alarme/ativado#alarme\" name=\"alarme\"><button class=\"button button2\">ATIVAR</button></a></p>");
}
 
client.println("</body></html>");
 
client.println();
break;
} else {
currentLine = "";
}
} else if (c != '\r') {
currentLine += c;
}
}
}
 
header = "";
 
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
 
if (digitalRead(pir) == LOW && digitalRead(sinalalarme) == LOW) {
digitalWrite (som, HIGH);
}
 
if (digitalRead(pir) == HIGH && digitalRead(sinalalarme) == HIGH) {
digitalWrite (som, LOW);
delay (1000);
digitalWrite (som, HIGH);
delay (1000);
digitalWrite (som, LOW);
delay (1000);
digitalWrite (som, HIGH);
delay (1000);
digitalWrite (som, LOW);
delay (1000);
digitalWrite (som, HIGH);
delay (1000);
digitalWrite (som, LOW);
delay (1000);
digitalWrite (som, HIGH);
delay (1000);
}
}
