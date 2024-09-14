#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <String.h>
 

byte mac[]    = { 0x9A, 0x5A, 0x1E, 0x04, 0xB0, 0xDD };
//IPAddress ip(192,168,137,171);//(169,254,254,159);
IPAddress server(192,168,137,1);//(192,168,137,1);//(169,254,254,158);
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

 
void reconnect() {
  // Loop dok se ne povezemo
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Pokusaji povezivanja
    if (mqttClient.connect("arduinoClient", "arduinoClient", "1234")) {
      Serial.println("connected");

      int temp;
      int pressure;
      int humidity;
      char msg[11];

      while(1){
        temp = random(18,30);//Random biranje temperature od 18 do 30
        pressure = random(1013,1030);//Random biranje vazdusvog pritiska od 1013 do 1030
        humidity = random(40,60);//Random biranje vlaznosti vazduha od 40 do 60
        sprintf_P(msg, PSTR("%d,%d,%d"), temp, pressure, humidity);
        mqttClient.publish("sensors", msg);
        Serial.println(msg);
        delay(2000);//Ceka 2 sekunde izmedju dve poruke 
      }
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);//Ceka 5 sekundi do sledeceg pokusaja
    }
  }
}
 
void setup()
{
  Serial.begin(57600);
  Ethernet.begin(mac);
  Serial.println("Obtaining IP address");
  IPAddress ip = Ethernet.localIP();
  delay(3000);
  Serial.println(ip);
  mqttClient.setServer(server, 1883);
  
 
  Ethernet.begin(mac, ip);
  // Mora da odmori malo
  delay(1500);
}
 
void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}