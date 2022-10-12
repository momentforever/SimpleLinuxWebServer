cmake_minimum_required(VERSION 2.8)
project(Socket C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DWITH_DEBUG")


include_directories(
        common/
        lib/
        stl/
        core/
        protocol/http/

        auto/
        test/
        )

add_executable(Socket
        main.c

        lib/lib.c
        lib/cJSON.c

        stl/rbtree.c
        stl/list.c
        stl/queue.c

        common/clock.c
        common/timer.c
        common/memory.c

        common/config.c
        common/module.c

        core/cycle_module.c
        core/event_module.c
        core/listening_module.c
        core/epoll_module.c

        protocol/http/self_httpd.c

        auto/auto.c

        test/test.c
        )

target_link_libraries(Socket rt)
target_link_libraries(Socket pthread)
#target_link_libraries(Socket yaml)