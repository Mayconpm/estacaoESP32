#include <Arduino.h>
#include <sys/time.h>
#include <WiFiMulti.h>
#include <NTPClient.h>

WiFiMulti wifiMulti;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600);

void connectToNetwork()
{
    wifiMulti.addAP("Pluviometro", "d4m14n0IPRJ");
    wifiMulti.addAP("uerj1", "vp4d2ac9");

    Serial.println("Estabelecendo conexão WiFi...");

    if (wifiMulti.run() == WL_CONNECTED)
    {
        delay(500);
        Serial.print("WiFi conectada:");
        Serial.println(WiFi.SSID());
        Serial.print("Endereço de IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());
    }
}

void updateTime(NTPClient ntpClient)
{
    ntpClient.begin(); // Inicia o NTP.
    ntpClient.forceUpdate();

    int epoch_time = ntpClient.getEpochTime();
    timeval epoch = {epoch_time, 0};
    const timeval *tv = &epoch;
    settimeofday(tv, NULL);
}

void setup()
{
    Serial.begin(115200);
    delay(10);
    connectToNetwork();
    updateTime(timeClient);
}

void loop()
{
    struct tm now;
    getLocalTime(&now, 0);
    Serial.println(&now, "%B %d %Y %H:%M:%S (%A)");
    delay(1000);
}
