cmake_minimum_required(VERSION 3.5)

project(UV_control_box VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets SerialPort Network Test REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/communication")
include_directories("${PROJECT_SOURCE_DIR}/control")
include_directories("${PROJECT_SOURCE_DIR}/PicFrame")
include_directories("${PROJECT_SOURCE_DIR}/utils")
include_directories("${PROJECT_SOURCE_DIR}/UV")
include_directories("${PROJECT_SOURCE_DIR}")


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp

    UV/icontroldata.cpp
    UV/iuserinterfacedata.cpp
    UV/ibasicdata.cpp
    UV/iserverdata.cpp
    UV/uv_state.cpp

    control/ControlBase.cpp
    control/Joystick.cpp
    control/Step.cpp
    control/Gradation.cpp

    PicFrame/picframe.cpp
    #----------------------------------
    communication/pultcontrolsystemprotocols.h
    communication/udp_protocol.h
)


add_executable(UV_control_box
    ${PROJECT_SOURCES}
)

target_link_libraries(UV_control_box PRIVATE
    sfml-graphics sfml-window sfml-system
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)
