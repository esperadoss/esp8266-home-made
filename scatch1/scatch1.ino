#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "WebServer1.h"
#include "FirmwareUploader.h"
#include "Utils.h"
#include "wifi.h"

WebServer server(80);

// Обработчик запросов на главную страницу
void handleRoot() {
  String html;
  html += "<html><body>";
  html += "<h1>ESP8266 Firmware Updater</h1>";
  html += "<h2>Wi-Fi Settings</h2>";
  html += "<form method=\"post\" action=\"/wifi\">";
  html += "<input type=\"text\" name=\"ssid\" placeholder=\"Имя wifi сети\"><br>";
  html += "<input type=\"password\" name=\"password\" placeholder=\"Пароль wifi сети\"><br>";
  html += "<input type=\"submit\" value=\"Подключиться\">";
  html += "</form>";
  html += "<h2>Прошивка</h2>";
  html += "<p>Адрес скетча: ";
  html += getSketchUrl();
  html += "</p>";
  html += "<form method=\"post\" enctype=\"multipart/form-data\" action=\"/upload\">";
  html += "<input type=\"file\" name=\"update\"><br>";
  html += "<input type=\"submit\" value=\"Загрузить\">";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Обработчик запросов на подключение к Wi-Fi
void handleWifi() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  // Если имя сети не пустое
  if (ssid.length() > 0) {
    // Подключаемся к Wi-Fi
    WiFiManager::connect(ssid.c_str(), password.c_str());

    // Перезагружаем устройство после подключения к сети
    server.sendHeader("Location", "/");
    server.send(303);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Имя сети не может быть пустым");
  }
}

// Обработчик запросов на загрузку прошивки
void handleFirmwareUpload() {
  // Если файл был успешно загружен
  if (FirmwareUploader::handleUpload(server)) {
    // Перезагружаем устройство для применения новой прошивки
    server.sendHeader("Location", "/");
    server.send(303);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Ошибка загрузки файла прошивки");
  }
}

void setup() {
  // Инициализация светодиода
  Utils::initLed();

  // Начальное состояние светодиода
  Utils::setLedState(!WiFiManager::isConnected());

  // Создание точки доступа Wi-Fi и запуск веб-сервера
  WiFiManager::startAccessPoint("esperadoss", "76543210");
  server.on("/", handleRoot);
  server.on("/wifi",


  // Обработчик формы загрузки прошивки
server.on("/upload", HTTP_POST, [](){
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  ESP.restart();
}, [](){
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    Serial.printf("Start uploading file: %s\n", upload.filename.c_str());
    // Открываем файл для записи бинарных данных
    if(!Update.begin(UPDATE_SIZE_UNKNOWN)){
      Serial.println("Update.begin failed");
    }
  } else if(upload.status == UPLOAD_FILE_WRITE){
    // Записываем бинарные данные в открытый файл
    if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
      Serial.println("Update.write failed");
    }
  } else if(upload.status == UPLOAD_FILE_END){
    // Закрываем файл и выполняем прошивку
    if(Update.end(true)){
      Serial.printf("Update Success: %u B\n", upload.totalSize);
    } else {
      Serial.println("Update Error");
    }
  }
});

// Запуск веб-сервера
server.begin();
Serial.println("HTTP server started");
  server.on("/upload", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      WiFiUDP::stopAll();
      Serial.printf("Update: %s\n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) { // start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
    yield();
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}


