cmake_minimum_required(VERSION 3.26)
project(ProjetoAssinatura C)

set(CMAKE_C_STANDARD 11)

add_executable(ProjetoAssinatura main.c
        include/app.h
        include/tools.h)
target_include_directories(ProjetoAssinatura PUBLIC /include)