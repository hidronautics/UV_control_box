cmake_minimum_required(VERSION 3.5)

project(UV_control_box VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets SerialPort Network Test REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/communication")
include_directories("${PROJECT_SOURCE_DIR}/control")
include_directories("${PROJECT_SOURCE_DIR}/PicFrame")
include_directories("${PROJECT_SOURCE_DIR}/utils")
include_directories("${PROJECT_SOURCE_DIR}/UV")

#SFML
if(WIN32)
    add_subdirectory(SFML/SFML)
else()
    set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/")
    find_package(SFML 2.4 REQUIRED graphics window system audio)
    include_directories(${SFML_INCLUDE_DIR})
endif()

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
        mainwindow.h

        UV/icontroldata.h
        UV/iuserinterfacedata.h
        UV/ibasicdata.h
        UV/iserverdata.h
        UV/uv_state.h

        control/ControlBase.h
        control/Joystick.h
        control/Step.h
        control/Gradation.h

        communication/pultcontrolsystemprotocols.h
        communication/udp_protocol.h

        PicFrame/picframe.h
        #----------------------------------
        mainwindow.ui
        PicFrame/picframe.ui
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UV_control_box
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(UV_control_box SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(UV_control_box
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(UV_control_box PRIVATE
        sfml-graphics sfml-window sfml-system
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        )

set_target_properties(UV_control_box PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UV_control_box)
endif()
