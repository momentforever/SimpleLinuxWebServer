//
// Created by lzt11 on 2022/6/17.
//
#include "clock.h"

time_t current_monotonic_timestamp;
time_t current_realtime_timestamp;

// used for timer
void update_monotonic_time(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    current_monotonic_timestamp = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
};

void update_realtime_time(){
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    current_realtime_timestamp = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}