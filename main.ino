#include "config.h"

#include <TinyGsmClient.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>
#include <array>
#include <WiFi.h>

#include "utils.h"
#include "html.h"

// Modem
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
String res;

// HTTP server
WiFiServer server(80);

#include "init.h"
#include "modem.h"

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("");

  // Initialise components
  init_Peripherals();
  init_Sd();
  init_WifiAp();
  init_Modem();

  // Verify status
  modem_ReadBasicInfo();

  // Configure modem
  modem_SetNetworkMode();
  modem_SetRadioMode();
  modem_SetRfFuncMode();
  modem_SetPdpParams();

  // Verify modem configuration
  modem_ValidateNetwork();
  modem_ReadNetworkInfo();
}

void loop() {
  modem.maintain();
  WiFiClient client = server.available();
  runHttpServer(client);
}

void runHttpServer(WiFiClient client) {
  String header;

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (!client.available()) {
        continue;
      }

      char c = client.read();
      // Serial.write(c);
      header += c;
      if (c == '\n') {
        if (currentLine.length() == 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html; charset=utf-8");
          client.println("Connection: close");
          client.println();
          
          if (header.indexOf("GET /send-sms") >= 0) {
            Serial.println("(i) Sending SMS...");
            String message = "Test message :)";
            res = modem.sendSMS(phoneNumber, message);
            
            if (res) {
              Serial.println("(+) SMS sent successfully");
            }
            else {
              Serial.println("(-) Failed to send SMS");
            }
            delay(AT_DELAY);
          }
          else if (header.indexOf("GET /send-get") >= 0) {
            Serial.println("(i) Sending HTTP GET request...");
            sendCommand("AT+SAPBR=3,1,\"APN\",\"" + String(apn) +  "\"", 500, true);
            sendCommand("AT+SAPBR=1,1", 500, true);
            sendCommand("AT+HTTPINIT", 500, true);
            sendCommand("AT+HTTPPARA=\"URL\",\"" + String(getUrl) + "\"", 500, true);
            sendCommand("AT+HTTPPARA=\"CID\",1", 500, true);
            sendCommand("AT+HTTPACTION=0", 500, true);
            sendCommand("AT+HTTPREAD", 2000, true);
            sendCommand("AT+HTTPTERM", 500, true);
          }
          
          client.println(html);
          client.println();
          break;
        }
        else {
          currentLine = "";
        }
      }
      else if (c != '\r') {
        currentLine += c;
      }
    }

    header = "";
    client.stop();
  }
}
