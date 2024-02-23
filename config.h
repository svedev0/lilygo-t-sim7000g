// SIMCom SIM7000G
#define TINY_GSM_MODEM_SIM7000SSL
#define TINY_GSM_RX_BUFFER  1024  // Set modem RX buffer to 1KB
#define SerialAT            Serial1

#define uS_TO_S_FACTOR  1000000ULL  // Conversion factor of ms to seconds
#define TIME_TO_SLEEP   60  // Sleep timeout for ESP32 (in seconds)
#define AT_DELAY        200 // Standard delay between AT commands

// General hardware connections
#define UART_BAUD  115200
#define PWR_PIN    4
#define LED_PIN    12

// Hardware interlink
#define PIN_DTR    25
#define PIN_TX     27
#define PIN_RX     26

// SD card connections
#define SD_MISO  2
#define SD_MOSI  15
#define SD_SCLK  14
#define SD_CS    13

// GSM PIN, if any
#define GSM_PIN ""

// GPRS credentials, if any
const char apn[] = "";
const char gprsUser[] = "";
const char gprsPass[] = "";

// WiFi access point
const char* apSsid = "SIM7000G AP";
const char* apPassword = "aptest123";

// Phone number for SMS test message. Must be in format "+123456789"
const char phoneNumber[] = "";

// URL for test GET request. Must be HTTP, not HTTPS
const char getUrl[] = "";
