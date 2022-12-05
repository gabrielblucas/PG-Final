#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "wifi"; // Enter your WiFi name
const char *password = "senha42164100";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "ff029006.us-east-1.emqx.cloud";
const char *topic = "Rele";
const char *mqtt_username = "Esp";
const char *mqtt_password = "noesnograu";
const int mqtt_port = 15004;

int rele = 32 ;
int medidor = 35;
volatile long pulse = 0;
double calculo = 0;
float fluxoAcumulado = 0;
double metroCubico = 0;
float calculoL = 0;
char chang [8];

WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
 pinMode(rele ,OUTPUT);
 pinMode (medidor, INPUT);
 Serial.begin(115200);
 attachInterrupt (digitalPinToInterrupt(medidor) , increase, RISING);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe





}





void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(rele, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    Serial.println("Rele on: " + String((char)payload[0]));
  } else if ((char)payload[0] == '4'){
    digitalWrite(rele, LOW);  // Turn the LED off by making the voltage HIGH
    Serial.println("Rele off: " + String((char)payload[0]));
  }




 Serial.println();
 Serial.println("-----------------------");
}




void loop() {
 client.loop();
 // publish and subscribe
  calculo = (pulse * 2.25);
  //calculoL = (calculo/1000);
  fluxoAcumulado = (pulse * 2.25) / 1000 ;
  metroCubico = fluxoAcumulado / 1000.000;
  
 

  Serial.println("Pulsos dados pelo sensor :" + String(pulse) );

  Serial.println("Litros por minuto :" + String(calculo));
 
  Serial.println("Gasto em Litros :" + String(fluxoAcumulado) );
 
  Serial.println("Metros cubicos : " + String(metroCubico) );
  
  char *valor = dtostrf(fluxoAcumulado, 4, 4,chang);
  
   client.publish("pg",valor);
   client.subscribe(topic);

  delay(5000);
 
 
}
void increase (){

  pulse++;
}
