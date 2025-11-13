
cmake_minimum_required(VERSION 3.16)
project(Hui2App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Пути к include/
include_directories(${CMAKE_SOURCE_DIR}/include)

# ===== Найти Qt6 =====
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core SerialPort)

# ===== Найти Boost =====
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# ===== Добавляем исходники и хедеры =====
set(SOURCES
    src/main.cpp
    src/data/PacketProcessor.cpp
    src/serial/SerialPort.cpp
)

set(HEADERS
    include/serial/SerialPort.h
    include/data/PacketProcessor.h
)

# ===== Собираем =====
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# ===== Линковка =====
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::SerialPort
    Boost::system
)

# ===== Указываем директории для MOC =====
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# ===== Отладка =====
message(STATUS "Qt6_INCLUDE_DIRS = ${Qt6_INCLUDE_DIRS}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")

