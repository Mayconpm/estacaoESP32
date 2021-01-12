#ifndef TNTP
#define TNTP
#include <Arduino.h>
#include <string.h>

class TtempoNTP
{

public:
    void updateTime()
    {
        configTime(-3 * 3600, 0, NTP_SRV);
    }
};

#endif
