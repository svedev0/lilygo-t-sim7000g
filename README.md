# lilygo-t-sim7000g

This repository is a demonstration of the features offered by the Lilygo T-SIM7000G. This includes features of the Espressif ESP32, SIMCom SIM7000G, and SPI SD card interface.

### Configuration

By default, the modem will be configured to only use LTE connections. This can be customised by changing the line `modem.setNetworkMode(int);`. The possible options are:

| Value | Network Mode         |
|-------|----------------------|
| 2     | Automatic            |
| 13    | GSM only             |
| 38    | LTE only             |
| 51    | GSM and LTE only     |

Please note that some modification is required before compiling and running this code. At the end of the `config.h` header file, you must supply your own information for the GSM and GPRS credentials. You can also configure a phone number and a URL for some of the demos. Additionally, it is recommended to change the default password for the access point.

### Sample output:

```
(i) Powering on peripherals...
(i) Initialising micro SD card...
(+) Micro SD card size: 2048 MB
(i) Initialising WiFi access point...
(i) Access point IP address: 192.168.4.1
(i) Initialising modem...
(i) Modem Name: SIMCOM SIM7000G
(i) Modem Info: SIM7000G R1529
(i) Waiting for network...
(+) Network connected
(i) CCID: 89464180175212605297
(i) IMEI: 862951635848925
(i) Operator: MyNetworkProvider
(i) Local IP: 0
(i) Signal quality: 27
(i) Waiting for modem response...
(i) Connection type: LTE CAT-M1
(i) Connection status: Online
```
