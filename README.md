# JCA_IOT

Die Bibliothek basiert auf den folgenden Grundgedanken
- Der Code soll auf mehrere IOT's geladen werden können
- Die Konfiguration soll über Files erfolgen
- Die Funktion des IOT's soll ebenfalls in einem File definiert werden
- Der Code soll für unabhängig von der Hardware sein
- Anpassungen soll per WebServer hochgeladen werden können
- Die Kommunikation zwischen den IOT's soll in einem eigenen Mesh-Netzwerk erfolgen
- Daten werden immer aktiv gesendet bzw. beim Parter zum Senden angemeldet
- Mittels einer Root-Node soll Anbindung an das Heim-WLAN erfolgen


### includes

Als Mesh-Infrastruktur wird auf die [painlessMesh](https://gitlab.com/painlessMesh/painlessMesh) Library zurück gegriffen. Diese erfordert die Libraries:
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [TaskScheduler](https://github.com/arkhipenko/TaskScheduler)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) (ESP8266)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) (ESP32)

Als Webserver kommt [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) zum Einsatz, dieser greift ebenfalls auf [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) / [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) zurück.
