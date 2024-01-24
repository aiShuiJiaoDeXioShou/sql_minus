cmake_minimum_required(VERSION 3.25)
project(sql_minus C)

set(CMAKE_C_STANDARD 11)

add_executable(sql_minus main.c lexer.c parser.c parser.h engine.c engine.h)
