#ifndef TINTERFACEWIRELESS
#define TINTERFACEWIRELESS
#include <Arduino.h>
#include <string.h>
#include "WiFi.h"
#include "lwip/apps/sntp.h"

class TinterfaceWireless
{

private:
    WiFiClient Wificlient;

public:
    static boolean conectaWifi()
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin("Pluviometro", "d4m14n0IPRJ");
            delay(1000);
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            initialize_sntp();
            delay(1000);
            return true;
        }
        else
        {
            return false;
        }
    }

    static void initialize_sntp(void)
    {
        sntp_stop();
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, NTP_SRV);
        setenv("TZ", "BRST+3", 1);
        tzset();
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
        sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
        sntp_init();
    }

    IPAddress getIP()
    {
        return WiFi.localIP();
    }

    IPAddress getGateway()
    {
        return WiFi.gatewayIP();
    }
};

#endif
