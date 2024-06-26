


#ifndef _filesystem_
#define _filesystem_

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>


void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void writeFile2(fs::FS &fs, const char * path, const char * message);
void deleteFile2(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);
void ReadConfig(fs::FS &fs, const char * path);

#endif /* _filesystem_ */