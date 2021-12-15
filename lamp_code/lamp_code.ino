#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192,168,1,1); // The default android DNS
DNSServer dnsServer;
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void redirectMain();
void handleNotFound();

volatile int pwmLevel = 0;
volatile int realPwm = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP... ");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  boolean result = WiFi.softAP("Lampa", "12345678");
  if(result == true) {
    Serial.println("Ready");
  } else {
    Serial.println("Failed!");
  }

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("connectivitycheck.android.com", redirectMain);
  server.on("connectivitycheck.gstatic.com", redirectMain);
  server.on("clients3.google.com", redirectMain);
  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/updateLamp", HTTP_OPTIONS, corsOptions);
  server.on("/updateLamp", HTTP_GET, updateLamp);
  server.on("/getJas", HTTP_OPTIONS, corsOptions);
  server.on("/getJas", HTTP_GET, getJas);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
  analogWriteResolution(12);
}

const uint16_t gama_lut[256] = {
     0,   0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   5,   6,   7,   8,
     9,  11,  12,  14,  15,  17,  19,  21,  23,  25,  27,  29,  32,  34,  37,  40,
    43,  46,  49,  52,  55,  59,  62,  66,  70,  73,  77,  82,  86,  90,  95,  99,
   104, 109, 114, 119, 124, 129, 135, 140, 146, 152, 158, 164, 170, 176, 182, 189,
   196, 202, 209, 216, 224, 231, 238, 246, 254, 261, 269, 277, 286, 294, 302, 311,
   320, 328, 337, 347, 356, 365, 375, 384, 394, 404, 414, 424, 435, 445, 456, 467,
   477, 488, 500, 511, 522, 534, 545, 557, 569, 581, 594, 606, 619, 631, 644, 657,
   670, 683, 697, 710, 724, 738, 752, 766, 780, 794, 809, 823, 838, 853, 868, 884,
   899, 914, 930, 946, 962, 978, 994,1011,1027,1044,1061,1078,1095,1112,1130,1147,
  1165,1183,1201,1219,1237,1256,1274,1293,1312,1331,1350,1370,1389,1409,1429,1449,
  1469,1489,1509,1530,1551,1572,1593,1614,1635,1657,1678,1700,1722,1744,1766,1789,
  1811,1834,1857,1880,1903,1926,1950,1974,1997,2021,2045,2070,2094,2119,2143,2168,
  2193,2219,2244,2270,2295,2321,2347,2373,2400,2426,2453,2479,2506,2534,2561,2588,
  2616,2644,2671,2700,2728,2756,2785,2813,2842,2871,2900,2930,2959,2989,3019,3049,
  3079,3109,3140,3170,3201,3232,3263,3295,3326,3358,3390,3421,3454,3486,3518,3551,
  3584,3617,3650,3683,3716,3750,3784,3818,3852,3886,3920,3955,3990,4025,4060,4095,
  };
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  static unsigned long lastPwmUpdate = 0;
  if(millis()-lastPwmUpdate>=10) {
    if(digitalRead(4) && pwmLevel < 255) {
      pwmLevel++;
    }
    if(digitalRead(5) && pwmLevel > 0) {
      pwmLevel--;
    }
    if(realPwm < pwmLevel) {
      realPwm++;
    } else if(realPwm > pwmLevel) {
      realPwm--;
    }
    analogWrite(16, gama_lut[realPwm]);
    lastPwmUpdate = millis();
  }
}

void handleRoot() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/html", F(R"=====(
#include "lamp_web.html"
  )====="));
}

void redirectMain() {
  server.sendHeader("Location", "http://lampa.cz", true);
  server.sendHeader("Connection", "close");
  server.send(302, "text/plain", "");
  server.client().stop();
}

// https://www.mischianti.org/2020/07/15/how-to-create-a-rest-server-on-esp8266-or-esp32-cors-request-option-and-get-part-4/
void corsOptions() {
  server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
  server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "");
  server.client().stop();
}

void updateLamp() {
  server.sendHeader("Connection", "close");
  if(server.args() == 0) {
    server.send(404, "text/plain", "Missing argument"); 
  } else {
    setLampPwm(server.arg(0).toInt());
    server.send(200, "text/plain", "OK");
  }
}

void getJas() {
  server.sendHeader("Connection", "close");
  char cstr[16];
  itoa(pwmLevel, cstr, 10);
  server.send(200, "text/plain", cstr);
}

void setLampPwm(int val) {
  pwmLevel = val;
  Serial.print("New lamp pwm is ");
  Serial.print(val);
  Serial.println();
}

void handleNotFound(){
  server.sendHeader("Connection", "close");
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
