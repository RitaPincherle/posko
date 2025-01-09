#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "timer.h"

void initTimer(Timer* timer) {
    timer->start = 0;
    timer->end = 0;
    timer->pauseStart = 0;
}

long long current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

void startTimer(Timer* timer, long long durationInMilli) {
    if (timer->pauseStart == 0) {
        timer->start = current_time_millis();
        timer->end = timer->start + durationInMilli;
    }
}

void pauseTimer(Timer* timer) {
    if (timer->pauseStart == 0 && timer->start != 0) {
        timer->pauseStart = current_time_millis();
    }
}

void unpauseTimer(Timer* timer) {
    if (timer->pauseStart != 0) {
        long long delta = current_time_millis() - timer->pauseStart;

        timer->start += delta;
        timer->end += delta;

        timer->pauseStart = 0;
    }
}

long long getElapsedTime(Timer* timer) {
    return current_time_millis() - timer->start;
}

long long getRemainingTime(Timer* timer) {
    return timer->end - current_time_millis();
}