
cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

# Установить стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти Qt6 необходимые компоненты (Core, Widgets, Gui, Charts - можно убрать, если не нужны)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Charts SerialPort)

# Пути к заголовочным файлам
include_directories(${PROJECT_SOURCE_DIR}/include)

# Собрать все исходники из src/ рекурсивно (без жесткого перечисления файлов)
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Линковка с Qt6 модулями
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Charts
    Qt6::SerialPort
)

# Включить автоматическую обработку MOC, UIC и RCC
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

