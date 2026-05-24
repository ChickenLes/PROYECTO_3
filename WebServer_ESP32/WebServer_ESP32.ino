/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <Wire.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "vayne_8923";  // Enter your SSID here
const char* password = "6573105524";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

// I2C
#define I2CSlaveAddress1 0x12
#define I2CSlaveAddress2 0x13

#define SDA_PIN 21
#define SCL_PIN 22

uint8_t error = 0;
// Temporización para consultar I2C cada 1 segundo
unsigned long ultimoI2C = 0;
const unsigned long intervaloI2C = 1000;   // ms

// Otras variables
uint8_t estadoParqueos[8];
int parqDisponibles = 0;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

  Wire.begin(SDA_PIN, SCL_PIN);   // Maestro
  Wire.setClock(100000);          // 100 kHz

  i2cScanner();
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  
  // Cada 1 segundo consulta a los dos STM32
  if (millis() - ultimoI2C >= intervaloI2C) {
    ultimoI2C = millis();
    callbackI2C();
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(estadoParqueos));
}
//************************************************************************************************
// Recibir y Enviar datos a los esclavos I2C
//************************************************************************************************
void callbackI2C(void) {
  Wire.beginTransmission(I2CSlaveAddress1);
  Wire.write(parqDisponibles);
  Serial.println(parqDisponibles);

  error = Wire.endTransmission(true);
  Serial.print("end Transmission: ");
  Serial.println(error);

  // Leer 8 bytes del esclavo
  uint8_t bytesReceived = Wire.requestFrom(I2CSlaveAddress2, 8);
  if (bytesReceived > 0) {
    for (int i = 0; i < bytesReceived; i++) {
      estadoParqueos[i] = Wire.read();
    }
  } else {
    Serial.println("No data received");
  }
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t* Parqueos) {
  parqDisponibles = 0;
  String pagina = pagina += "<!DOCTYPE html>\n";
  pagina += "<html lang=es>\n";
  pagina += "<head>\n";
  pagina += "<meta charset=UTF-8>\n";
  pagina += "<title>Parqueo-matic</title>\n";
  pagina += "<meta name=viewport content=\"width=device-width, initial-scale=1\">\n";
  pagina += "<meta http-equiv=\"refresh\" content=\"1\">\n"; // Recarga cada 2 segundos
  pagina += "<title>Parqueo-matic</title>\n";
  pagina += "<link href=https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css rel=stylesheet>\n";
  pagina += "<style>body{background-color:#f5f5f5}.parking-slot{width:120px;height:200px;background-color:#d9d9d9;border:3px solid #f4b400;border-radius:10px;position:relative;margin:20px}.car-img{width:90px;position:absolute;top:40px;left:50%;transform:translateX(-50%)}.status-btn{width:40px;height:40px;border-radius:50%;border:0}.red{background-color:red}.green{background-color:green}.counter-box{font-size:3rem;background:#e0e0e0;border-radius:15px;padding:20px;text-align:center;width:120px}</style>\n";
  pagina += "</head>\n";
  pagina += "<body>\n";
  pagina += "<div class=\"container text-center mt-4\">\n";
  pagina += "<h1>Parqueo-matic</h1>\n";
  pagina += "<div class=\"row justify-content-center mt-4\">\n";
  pagina += "<div class=col-md-8>\n";
  pagina += "<div class=\"d-flex flex-wrap justify-content-center\" id=parkingArea>\n";

  for (int i = 0; i < 8; i++) {
    uint8_t estado = Parqueos[i];
    pagina += "<div class=\"parking-slot d-flex flex-column align-items-center justify-content-end\">";
    if (estado == 0) {
      parqDisponibles++;
      pagina += "<button class=\"status-btn green mb-2\"></button>";
    }
    else if (estado == 1) {
      pagina += "<img src=\"https://www.freeiconspng.com/thumbs/car-icon-png/car-icon-png-25.png\" class=\"car-img\">";
      pagina += "<button class=\"status-btn red mb-2\"></button>";
    }
    pagina += "</div>";
  }

  pagina += "</div>\n";
  pagina += "</div>\n";
  pagina += "<div class=col-md-3>\n";
  pagina += "<h4>Parqueos disponibles</h4>\n";
  pagina += "<div class=counter-box>\n";
  pagina += String(parqDisponibles);
  pagina += "</div>\n";
  pagina += "</div>\n";
  pagina += "</div>\n";
  pagina += "</div>\n";
  pagina += "</body>\n";
  pagina += "</html>";

  return pagina;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

//************************************************************************************************
// I2C Scanner - Comprueba conexion
//************************************************************************************************
void i2cScanner() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if(error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}