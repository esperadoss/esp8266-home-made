#include <ESP8266WebServer.h>

class WebServer {
  private:
    ESP8266WebServer server;
    
  public:
    void setup(int port) {
      server = ESP8266WebServer(port);
    }
    
    void handleClient() {
      server.handleClient();
    }
    
    void on(const char* uri, std::function<void()> handler) {
      server.on(uri, handler);
    }
    
    void on(const char* uri, HTTPMethod method, std::function<void()> handler) {
      server.on(uri, method, handler);
    }
    
    void send(int code, const char* content_type, const String& content) {
      server.send(code, content_type, content);
    }
    
    void send(int code, const char* content_type, const uint8_t* content, size_t content_length) {
      server.send(code, content_type, content, content_length);
    }
    
    void sendHeader(const char* headerName, const char* headerValue, bool first = false) {
      server.sendHeader(headerName, headerValue, first);
    }
    
    void setContentLength(size_t contentLength) {
      server.setContentLength(contentLength);
    }
};
