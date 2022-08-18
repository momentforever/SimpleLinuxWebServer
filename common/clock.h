//
// Created by lzt11 on 2022/6/17.
//
#ifndef SOCKET_CLOCK_H
#define SOCKET_CLOCK_H

#include "lib.h"

extern time_t current_monotonic_timestamp;
extern time_t current_realtime_timestamp;

void update_monotonic_time();
void update_realtime_time();

#define update_time() \
        update_monotonic_time(); \
        update_realtime_time;

#endif //SOCKET_CLOCK_H
