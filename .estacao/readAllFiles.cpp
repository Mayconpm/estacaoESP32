#include "SPIFFS.h"

void lerArquivo()
{
    SPIFFS.begin();

    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {

        Serial.print("FILE: ");
        Serial.println(file.name());

        file = root.openNextFile();
    }
}
