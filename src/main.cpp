#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ESPmDNS.h>

Servo servo1; // create four servo objects
const int servo1Pin = 15;
// Published values for SG90 servos; adjust if needed
int minUs = 0;
int maxUs = 5000;
bool to0_flag = false;

const char *ssid = "XXXX";     // 自分のSSIDに書き換える
const char *password = "XXXX"; // 自分のパスワードに書き換える
WebServer server(80);
String target = "0"; // この変数をPOSTメソッドで書き換える

// k=0:0°,30°のスイッチ, k!=0:自由角度への移動
void sg90_switch(int k)
{
  // k=0:switchbot mode, k!=0:任意角度への移動
  if (k == 0)
  {
    // to0_flag
    int status = servo1.read();
    if (status > 15) to0_flag = true; //現在30なので0に持っていく
    else if (status < 15) to0_flag = false; //現在0なので30に持っていく
    // servo.write
    if (to0_flag) servo1.write(1);
    else servo1.write(30);
  }
  else
  {
    servo1.write(k);
  }
}
void setup()
{
  Serial.begin(9600);
  servo1.setPeriodHertz(50); // Standard 50hz servo
  servo1.attach(servo1Pin, minUs, maxUs);
  int status = servo1.read();
  if (status > 15) to0_flag = true; //現在30なので0に持っていく
  else if (status < 15) to0_flag = false; //現在0なので30に持っていく
  // シリアルコンソールのセットアップ
  Serial.begin(115200);
  delay(10);
  Serial.println();

  // WiFiに接続
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  MDNS.begin("esp32"); // ホスト名 esp32.local

  // root
  server.on("/", HTTP_ANY, [](){
    // PUT
    if (server.method() == HTTP_PUT) { 
      target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
      int i=target.toInt();
      sg90_switch(i);
      server.send(200, "text/plain", target);   //リクエストされた角度を返す
    }
    // GET
    else if(server.method() == HTTP_GET){
      int status = servo1.read();
      server.send(200, "text/plain", String(status)); // statusをクライアントに返す 
    } 
  });

  // onNotFound
  server.onNotFound([](){
    server.send(404, "text/plain", "Not Found."); // 404を返す 
  });

  server.begin();
}

void loop()
{
  server.handleClient();
}
