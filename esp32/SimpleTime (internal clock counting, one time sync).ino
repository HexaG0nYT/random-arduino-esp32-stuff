#include <WiFi.h>
#include "time.h"

// Wi-Fi credentials
const char* ssid = "7567";
const char* password = "bucket06";

// Timezone settings
struct tm timeinfo;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 60*60*8; // GMT 8+
const int   daylightOffset_sec = 0;  // No daylight saving in UTC

bool syncTime(int retry_count){
  bool success = false;
  for (int i=0; i<retry_count; i++){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // obtain and adjust esp32 time from NTP server
    success = getLocalTime(&timeinfo);
    if (success) break; // break when successful
  }
  return success;
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize and get the time
  Serial.println("Attempting to sync time, please wait...");
  // try to sync time (10 attempts)
  if (!syncTime(10)){
    Serial.println("Failed to sync time, please restart to try again.");
  }

  // Print the current time
  Serial.println(&timeinfo, "UTC Time: %Y-%m-%d %H:%M:%S");
}

void loop() {
  getLocalTime(&timeinfo);
  Serial.printf("UTC Time (internal clock counting): %02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  Serial.println();
  delay(1000);  // Update every second in the loop
}
