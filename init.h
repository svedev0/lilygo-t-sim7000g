/*
  Power on peripherals
*/
void init_Peripherals() {
  Serial.println("(i) Powering on peripherals...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(100);
  digitalWrite(PWR_PIN, LOW);
}

/*
  Initialise micro SD card
*/
void init_Sd() {
  Serial.println("(i) Initialising micro SD card...");
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);

  if (!SD.begin(SD_CS)) {
    Serial.println("(-) No micro SD card found");
  }
  else {
    unsigned long cardSize = SD.cardSize() / (1024 * 1024);
    String str = "(+) Micro SD card size: " + String(cardSize) + " MB";
    Serial.println(str);
  }
}

/*
  Initialise WiFi access point
*/
void init_WifiAp() {
  Serial.println("(i) Initialising WiFi access point...");
  WiFi.softAP(apSsid, apPassword);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("(i) Access point IP address: ");
  Serial.println(IP);
  
  server.begin();
  delay(100);
}

/*
  Initialise modem
*/
void init_Modem() {
  Serial.println("(i) Initialising modem...");
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(AT_DELAY);

  if (!modem.restart()) {
    Serial.println("(-) Failed to restart modem, attempting to continue without restarting");
  }
}
