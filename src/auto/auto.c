//
// Created by lzt11 on 2022/7/27.
//
#include "auto.h"

module_t *modules[] = {
        &cycle_module,
        &listening_module,
        [2]=&protocol_module,
        &http_request_module,
        NULL,
};

