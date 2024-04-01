cmake_minimum_required(VERSION 2.8)
project(SimpleLinuxWebServer C)

enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O -g")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DWITH_DEBUG")

# ------------------- Link -------------------

include_directories(
        src/common/
        src/lib/
        src/stl/
        src/core/
        src/protocol/
        src/protocol/http/

        src/auto/
        )

add_library(lib OBJECT
        src/lib/lib.c
        src/lib/cJSON.c
        
        src/stl/rbtree.c
        src/stl/list.c
        src/stl/queue.c
        )

add_library(core OBJECT
        src/common/clock.c
        src/common/timer.c
        src/common/memory.c
        src/common/util.c
        src/common/config.c
        src/common/module.c
        
        src/core/cycle_module.c
        src/core/event_module.c
        src/core/listening_module.c
        src/core/epoll_module.c
        src/core/protocol_module.c

        src/auto/auto.c

        src/protocol/http/self_httpd.c
        src/protocol/http/request_module.c
        src/protocol/http/session_module.c
        )


# ------------------- Test -------------------

add_executable(memory_test test/memory_test.c)

target_link_libraries(memory_test PRIVATE lib core)

add_test(NAME memory_test
         COMMAND memory_test)


# ------------------- Execute -------------------

add_executable(slws src/main.c
)

target_link_libraries(slws PRIVATE lib core)

target_link_libraries(slws PRIVATE rt)
target_link_libraries(slws PRIVATE pthread)

add_test(NAME slws
         COMMAND slws -v)


# cmake_minimum_required(VERSION 2.8)
# project(SimpleWebServer C)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DWITH_DEBUG")


# include_directories(
#         src/common/
#         src/lib/
#         src/stl/
#         src/core/
#         src/protocol/http/
#         src/protocol/
#         src/auto/
#         test/
#         )

# add_executable(slws src/main.c
#         src/lib/lib.c
#         src/lib/cJSON.c

#         src/stl/rbtree.c
#         src/stl/list.c
#         src/stl/queue.c

#         src/common/clock.c
#         src/common/timer.c
#         src/common/memory.c
#         src/common/util.c

#         src/common/config.c
#         src/common/module.c

#         src/core/cycle_module.c
#         src/core/event_module.c
#         src/core/listening_module.c
#         src/core/epoll_module.c
#         src/core/protocol_module.c

#         src/protocol/http/request_module.c
#         src/protocol/http/session_module.c
#         src/protocol/http/self_httpd.c

#         src/auto/auto.c
#         )

# target_link_libraries(slws rt)
# target_link_libraries(slws pthread)