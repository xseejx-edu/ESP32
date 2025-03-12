#include <SPIFFS.h>

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
  
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }


    if (!SPIFFS.begin(true, "/spiffs")) {
    Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }


    // Open file from SPIFFS
    File file = SPIFFS.open("/test.html");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    // Read file content
    String fileContent = "";
    while (file.available()) {
        fileContent += (char)file.read();
    }
    file.close();

    // Print file content
    Serial.println("File Content:");
    Serial.println(fileContent);
}

void loop() {
    // Your loop code here
}
