cmake_minimum_required(VERSION 3.28)

project(ChronosExchangeEngine
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(main 
                src/main.cpp 
                src/orderbook/order_book.cpp
                # src/order_model.h
                # src/order_request.h
                # src/order_execution_report.h
                src/engine/matching_engine.cpp
                src/gateway/order_gateway.cpp
                src/utils/parser.cpp
                src/utils/logger.cpp
                src/utils/utils.cpp
                )

target_link_libraries(main Boost::system)
