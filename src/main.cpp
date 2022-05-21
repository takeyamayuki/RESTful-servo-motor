#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ESPmDNS.h>
#include <ssid_define.h>

Servo servo1; // create four servo objects
WebServer server(80);

const int servo1Pin = 15;
// Published values for SG90 servos; adjust if needed
int minUs = 0;
int maxUs = 5000;
bool to0_flag = false;
// angle > angle0 >= 0にすること
int angle = 60;
int angle0 = 5;

const char *ssid = MY_SSID;     // 自分のSSIDに書き換える
const char *password = MY_SSID_PASS; // 自分のパスワードに書き換える
const char *host_name = "rsm"; //RESTful servo motor
String target = "0"; // この変数をPUTメソッドで書き換える

void flag_check()
{
  // to0_flag
  int status = servo1.read();
  if (status > (angle+angle0)/2) to0_flag = true; //現在angle側なので0に持っていく
  else if (status < (angle+angle0)/2) to0_flag = false; //現在angle0側なので30に持っていく
}

// mode=true:angle0,angleのスイッチ, mode=false:自由角度への移動
void move_sg90(bool mode, int k)
{
  if (mode)
  {
    flag_check();
    if (to0_flag) servo1.write(angle0);
    else servo1.write(angle);
  }
  else servo1.write(k);
}

void handleRoot(void)
{
  // PUT
  if (server.method() == HTTP_PUT) 
  { 
    target = server.arg("plain"); // server.arg("plain")でリクエストボディが取れる。targetに格納
    if (target=="switch") move_sg90(true,0);
    else
    {
      int i=target.toInt();
      move_sg90(false, i);
    }
    server.send(200, "text/plain", "Moved servo "+target);   //リクエストされた角度を返す
  }
  // GET
  else if(server.method() == HTTP_GET)
  {
    int status = servo1.read();
    server.send(200, "text/plain", String(status)); // statusをクライアントに返す 
  } 
}

void handleNotFound(void)
{
  server.send(404, "text/plain", "Not Found.");
}

void setup()
{
  Serial.begin(9600);
  servo1.setPeriodHertz(50); // Standard 50hz servo
  servo1.attach(servo1Pin, minUs, maxUs);
  flag_check();
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
  MDNS.begin(host_name); // ホスト名 host_name.local
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // root
  server.on("/", handleRoot);
  // onNotFound
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop()
{
  server.handleClient();
}
