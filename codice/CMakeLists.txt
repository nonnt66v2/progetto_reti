cmake_minimum_required(VERSION 3.27)
project(codice C)

set(CMAKE_C_STANDARD 11)

add_executable(codice
        segreteria.c
        studente.c
        libreria/wrapper.c)
