cmake_minimum_required(VERSION 2.8)
project(SimpleWebServer C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DWITH_DEBUG")


include_directories(
        src/common/
        src/lib/
        src/stl/
        src/core/
        src/protocol/http/

        src/auto/
        test/
        )

add_executable(SimpleWebServer src/main.c

        src/lib/lib.c
        src/lib/cJSON.c

        src/stl/rbtree.c
        src/stl/list.c
        src/stl/queue.c

        src/common/clock.c
        src/common/timer.c
        src/common/memory.c

        src/common/config.c
        src/common/module.c

        src/core/cycle_module.c
        src/core/event_module.c
        src/core/listening_module.c
        src/core/epoll_module.c
        src/core/request_module.c
        src/core/session_module.c

        src/protocol/http/self_httpd.c

        src/auto/auto.c

        test/test.c
        )

target_link_libraries(SimpleWebServer rt)
target_link_libraries(SimpleWebServer pthread)
#target_link_libraries(SimpleWebServer yaml)