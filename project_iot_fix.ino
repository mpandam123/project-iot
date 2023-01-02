#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <stdlib.h>


WiFiClient espClient;
PubSubClient client(espClient);

//Connect to WiFi
const char* ssid = "WIFI ERROR";
const char* password = "1slkmzwa8awa";
const char* mqtt_server = "riset.revolusi-it.com";

const char* topik = "iot/G_231_19_0226";

String messages_terima;
String messages_kirim;






void konek_wifi()
{
WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnect() {
//Loop until we're reconnected
while (!client.connected()){
Serial.print("menghubungkan diri ke MQTT Server : "+(String)mqtt_server);
//Attempt to connect
if (client.connect("G.231.19.0226-23123")){
  Serial.println("connected");
  //... and  subscribe to topic
  client.subscribe(topik);
}else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println("coba lagi dalam 5 detik...");
 //Wait 5 seconds before retrying
 delay(5000);
 }
} 
}

void callback(char*topic, byte*payload, unsigned int length){
  
  Serial.print("Pesan dari MQTT [");
  Serial.print(topic);
  Serial.print("] ");
  messages_terima="";
  for (int i=0;i<length;i++){ //susun jadi string saja
    char receivedChar = (char)payload[i];
    messages_terima = messages_terima+(char)payload[i]; // ambil pesannya masukkan dalam string
    }
    
    Serial.println(messages_terima);

//kirim pesan ke mqtt server

float kelembaban = dht.readHumidity();
float suhu = dht.readTemperature();
Serial.println("Suhu: "+(String)suhu+"- Kelembaban:"+(String)kelembaban);
delay(1000);


messages_kirim=(String)suhu;
//Serial.println(messages_kirim);
//perhatikan cara ngirim variabel string lewat client.publish ini gak bisa langsung...
 char attributes[100];
messages_kirim.toCharArray(attributes,100);
client.publish(topik,attributes,true);
Serial.println("messages :"+messages_kirim+" terkirim...");
delay(1000);

}

 //..................... begin ....................... //
void setup() 
{
  Serial.begin(9600);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led1,0);
  digitalWrite(led2,0);
  digitalWrite(led3,0);
  dht.begin();
}

void loop() {
  if(WiFi.status() !=WL_CONNECTED) {konek_wifi();} //jika tidak konek wifi maka di reconnect
  if(!client.connected()){reconnect();} //reconnect apabila belum konek

  client.loop();


}