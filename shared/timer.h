#ifndef TIMER_H
#define TIMER_H

typedef struct  {
    long long start;
    long long end;
    long long pauseStart;
} Timer;

void initTimer(Timer* timer);
void startTimer(Timer* timer, long long durationInMilli);
void pauseTimer(Timer* timer);
void unpauseTimer(Timer* timer);
long long getElapsedTime(Timer* timer);
long long getRemainingTime(Timer* timer);
long long current_time_millis();

#endif