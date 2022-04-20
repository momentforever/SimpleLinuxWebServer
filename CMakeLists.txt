cmake_minimum_required(VERSION 2.8)
project(Socket C)

set(CMAKE_C_STANDARD 11)

include_directories("./lib/")

add_executable(Socket main.c ./lib/httpd.c ./lib/self_httpd.c)

