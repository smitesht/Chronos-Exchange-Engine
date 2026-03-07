cmake_minimum_required(VERSION 3.28)

project(ChronosExchangeEngine
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main 
                main.cpp 
                src/order_book.cpp
                src/order_model.h
                src/order_request.h
                src/order_execution_report.h)
