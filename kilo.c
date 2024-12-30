#include <stdio.h>
#include <sys/time.h>

long long current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    // Convert to milliseconds
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}

int main() {
    long long millis = current_time_millis();
    printf("Current time in milliseconds: %lld\n", millis);
    return 0;
}
