#include <Arduino.h>
#include "SPIFFS.h"
#include "time.h"
#include "lwip/apps/sntp.h"

void lerArquivo()
{
    SPIFFS.begin();

    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {

        Serial.print("FILE: ");
        Serial.println(file.name());
        while (file.available())
        {
            Serial.write(file.read());
        }

        file = root.openNextFile();
        Serial.println();
    }
}

void formata()
{
    bool formatted = SPIFFS.format();

    if (formatted)
    {
        Serial.println("\n\nSuccess formatting");
    }
    else
    {
        Serial.println("\n\nError formatting");
    }
}
