#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* SSID = "YOUR-SSID";
const char* PASS = "YOUR-RASSWORD";

const char* PAGE = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>STM32</title>
<meta http-equiv="refresh" content="2">
<style>
 body{background:#1a1a2e;color:#eee;font-family:monospace;padding:12px}
 #log{background:#16213e;padding:8px;border-radius:8px;white-space:pre-wrap}
</style></head><body>
<h3>STM32 Monitor</h3>
<div id="log">%LOG%</div>
<p><a href=" ">清空</a ></p >
</body></html>
)rawliteral";

String logBuf;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);   // RX=16, TX=17 → STM32

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    String html = PAGE;
    html.replace("%LOG%", logBuf);
    server.send(200, "text/html", html);
  });
  server.on("/clr", HTTP_GET, []() {
    logBuf = "";
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.begin();
}

void loop() {
  server.handleClient();
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '\n')      logBuf += "<br>";
    else if (c != '\r') logBuf += c;
    if (logBuf.length() > 3000)
      logBuf = logBuf.substring(logBuf.length() - 2000);
  }
}
