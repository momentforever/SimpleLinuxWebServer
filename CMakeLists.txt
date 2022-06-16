cmake_minimum_required(VERSION 2.8)
project(Socket C)

set(CMAKE_C_STANDARD 11)


include_directories(
        ./lib/
        ./common/
        ./global/
        ./event/
        )

add_executable(Socket
        main.c
        lib/rbtree.c
        lib/list.c
        lib/queue.c
        event/self_httpd.c
        common/config.c
        common/cycle.c
        common/event.c
        common/config.c
        common/connection.c
        common/listening.c
        )

