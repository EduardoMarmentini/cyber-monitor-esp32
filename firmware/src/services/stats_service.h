#pragma once

#include <Arduino.h>
#include "models/stats.h"

class StatsService {
public:
    static void begin();
    static Stats getStats();

private:
    static unsigned long startTime;
};
