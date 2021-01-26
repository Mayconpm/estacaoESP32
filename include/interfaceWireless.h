#ifndef TINTERFACEWIRELESS
#define TINTERFACEWIRELESS
#include <Arduino.h>
#include <string.h>
#include "WiFi.h"
#include "ESP32Ping.h "
#include "lwip/apps/sntp.h"

#define DEBUG false

class TinterfaceWireless
{
private:
    WiFiClient Wificlient;
    boolean primeiraConexao = true;

    static void initialize_sntp(void)
    {
        sntp_stop();
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, strdup(SERVIDORNTP));
        setenv("TZ", TIMEZONE, 1);
        tzset();
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
        sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
        sntp_init();
        delay(1000);
    }

public:
    void conectaWifi()
    {
        while (primeiraConexao)
        {
            WiFi.begin(SSIDWIFI, PASSWORDWIFI);
            Serial.println("Tentando se conectar a rede Wireless.");
            delay(1000);
            if (Ping.ping("www.google.com", 3))
            {
                this->primeiraConexao = false;
                Serial.println("WiFi conectada.");
                initialize_sntp();
            }
            else
            {
                WiFi.begin(SSIDWIFI, PASSWORDWIFI);
                Serial.println("Tentando se conectar a rede Wireless.");
                delay(1000);
            }
        }
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin(SSIDWIFI, PASSWORDWIFI);
            Serial.println("Tentando se conectar a rede Wireless.");
            delay(1000);
        }
    }
};
#endif
