/*
  Send an attention (AT) command
*/
String sendCommand(String command, const int timeout, boolean debug) {
  String response = "";
  SerialAT.println(command);

  command.replace("\n", "");
  Serial.print("(i) ");
  Serial.print(command);
  
  unsigned long time = millis();
  while ((time + timeout) > millis()) {
    while (SerialAT.available()) {
      char c = SerialAT.read();
      response += c;
    }
  }

  if (debug) {
    response.replace("\n", "");
    response.replace("+HTTPACTION: 0,", " [");
    response.replace(",127", "]");
    Serial.print(" -> ");
    Serial.println(response);
  }

  return response;
}

/*
  Get basic modem information
*/
void modem_ReadBasicInfo() {
  String name = modem.getModemName();
  Serial.println("(i) Modem Name: " + name);
  delay(AT_DELAY);

  String modemInfo = modem.getModemInfo();
  Serial.println("(i) Modem Info: " + modemInfo);
}

/*
  Set preferred modem network mode
*/
void modem_SetNetworkMode() {
  String res;

  // 2 = Automatic
  // 13 = GSM only
  // 38 = LTE only
  // 51 = GSM and LTE only
  while (true) {
    res = modem.setNetworkMode(38);
    
    if (res != "1") {
      Serial.println("(-) Failed to set network mode!");
      delay(AT_DELAY);
      continue;
    }

    delay(AT_DELAY);
    break;
  }
}

/*
  Set preferred modem radio mode
*/
void modem_SetRadioMode() {
  String res;

  // 1 = CAT-M
  // 2 = NB-Iot
  // 3 = CAT-M and NB-IoT
  while (true) {
    res = modem.setPreferredMode(1);

    if (res != "1") {
      Serial.println("(-) Failed to set preferred modem radio mode!");
      delay(AT_DELAY);
      continue;
    }

    delay(AT_DELAY);
    break;
  }
}

/*
  Set the RF curcuit RX/TX power mode (circuit functionality or "CFUN") to full
  functionality
*/
void modem_SetRfFuncMode() {
  while (true) {
    modem.sendAT("+CFUN=1 ");

    if (modem.waitResponse(10000L) != 1) {
      Serial.println("(-) Failed to set RF circuit functionality mode!");
      delay(AT_DELAY);
      continue;
    }

    delay(AT_DELAY);
    break;
  }
}

/*
  Set the packet data protocol (PDP) context parameters such as PDP type (e.g.
  IP, IPV6, PPP, X.25, etc.), APN, data compression, header compression etc.
*/
void modem_SetPdpParams() {
  const int numberOfPieces = 24;
  int counter = 0;
  int lastIndex = 0;

  String pieces[24];
  String input;

  SerialAT.println("AT+CGDCONT?");
  delay(AT_DELAY);

  if (!SerialAT.available()) {
    Serial.println("(-) Failed to get PDP!");
    return;
  }

  input = SerialAT.readString();
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i + 1) == "\n") {
      pieces[counter] = input.substring(lastIndex, i);
      lastIndex = i + 1;
      counter++;
    }
    
    if (i == input.length() - 1) {
      pieces[counter] = input.substring(lastIndex, i);
    }
  }

  input = "";
  counter = 0;
  lastIndex = 0;

  for (int y = 0; y < numberOfPieces; y++) {
    for (int x = 0; x < pieces[y].length(); x++) {
      char c = pieces[y][x]; // Get one byte from buffer

      if (c == ',') {
        if (input.indexOf(": ") >= 0) {
          String data = input.substring((input.indexOf(": ") + 1));
          
          if ( data.toInt() > 0 && data.toInt() < 25) {
            modem.sendAT(
              "+CGDCONT=" +
              String(data.toInt()) +
              ",\"IP\",\"" +
              String(apn) +
              "\",\"0.0.0.0\",0,0,0,0"
            );
          }

          input = "";
          break;
        }

        input = "";
      }
      else {
        input += c;
      }
    }
  }
}

/*
  Check if network is connected
*/
void modem_ValidateNetwork() {
  while (true) {
    Serial.println("(i) Waiting for network...");

    if (!modem.waitForNetwork()) {
      delay(3000);
      continue;
    }

    if (modem.isNetworkConnected()) {
      Serial.println("(+) Network connected");
      delay(AT_DELAY);
      break;
    }
  }
}

/*
  Get network connection type and band
*/
void modem_ReadNetworkInfo() {
  String ccid = modem.getSimCCID();
  Serial.println("(i) CCID: " + ccid);

  String imei = modem.getIMEI();
  Serial.println("(i) IMEI: " + imei);

  String op = modem.getOperator();
  Serial.println("(i) Operator: " + op);

  IPAddress locIp = modem.localIP();
  Serial.println("(i) Local IP: " + String(locIp));

  int sq = modem.getSignalQuality();
  Serial.println("(i) Signal quality: " + String(sq));

  SerialAT.println("AT+CPSI?");
  delay(AT_DELAY);
  
  Serial.println("(i) Waiting for modem response...");
  while (!SerialAT.available()) {
    delay(50);
  }

  if (SerialAT.available()) {
    String result = SerialAT.readString();
    result.replace("\r", "");
    result.replace("\n", "");
    result.replace("+CPSI: ", "");

    String connectionType = getValue(result, ',', 0);
    Serial.println("(i) Connection type: " + connectionType);

    String connectionStatus = getValue(result, ',', 1);
    Serial.println("(i) Connection status: " + connectionStatus);
  }
}

/*
  Power on and enable GPS
*/
void modem_EnableGPS() {
  modem.sendAT("+CGPIO=0,48,1,1");
  if (modem.waitResponse(10000L) != 1) {
    Serial.println("(-) Failed to enable GPS!");
  }

  modem.enableGPS();
  Serial.println("(i) GPS is enabled");
}

/*
  Power off and disable GPS
*/
void modem_DisableGPS() {
  modem.sendAT("+CGPIO=0,48,1,0");
  if (modem.waitResponse(10000L) != 1) {
    Serial.println("(-) Failed to disable GPS!");
  }

  modem.disableGPS();
  Serial.println("(i) GPS is disabled");
}

/*
  Get GPS position
*/
void modem_GetGPSPosition() {
  float lat,  lon;
  if (modem.getGPS(&lat, &lon)) {
    Serial.println("GPS location:");
    Serial.println("Latitude: " + String(lat));
    Serial.println("Longitude: " + String(lon));
  }
  else {
    Serial.println("(-) Failed to get GPS location!");
  }
}
