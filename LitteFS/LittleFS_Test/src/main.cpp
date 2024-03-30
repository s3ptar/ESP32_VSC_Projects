#include <Arduino.h>
#include <LittleFS.h>

// put function declarations here:
int myFunction(int, int);

bool filesystemOK = false;
void InitFilesystem() {
  // Initialize LittleFS
  if (!LittleFS.begin(false /* false: Do not format if mount failed */)) {
    Serial.println("Failed to mount LittleFS");
    if (!LittleFS.begin(true /* true: format */)) {
      Serial.println("Failed to format LittleFS");
    } else {
      Serial.println("LittleFS formatted successfully");
      filesystemOK = true;
    }
  } else { // Initial mount success
    filesystemOK = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  InitFilesystem();
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}