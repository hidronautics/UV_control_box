cmake_minimum_required(VERSION 3.5)

project(UV_control_box VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/communication")
include_directories("${PROJECT_SOURCE_DIR}/control")
include_directories("${PROJECT_SOURCE_DIR}/PicFrame")
include_directories("${PROJECT_SOURCE_DIR}/thruster")
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
        UV/ituningdata.cpp
        UV/iuserinterfacedata.cpp
        UV/uv_controlcontour.cpp
        UV/uv_thruster.cpp
        UV/ibasicdata.cpp
        UV/iserverdata.cpp
        UV/uv_device.cpp
        UV/uv_state.cpp

        control/Control.cpp
        control/ControlBase.cpp
        control/ControlWindow.cpp
        control/Joystick.cpp
        control/Keyboard.cpp

        communication/UdpClient.cpp

        thruster/ThrusterWindow.cpp
        thruster/Thruster.cpp

        PicFrame/picframe.cpp
        #----------------------------------
        mainwindow.h

        UV/ibasicdata.h
        UV/icontroldata.h
        UV/iserverdata.h
        UV/ituningdata.h
        UV/iuserinterfacedata.h
        UV/uv_controlcontour.h
        UV/uv_device.h
        UV/uv_state.h

        control/Control.h
        control/ControlBase.h
        control/ControlWindow.h
        control/Joystick.h
        control/Keyboard.h

        communication/UdpClient.h

        thruster/ThrusterWindow.h
        thruster/Thruster.h

        PicFrame/picframe.h
        #----------------------------------
        mainwindow.ui
        control/ControlWindow.ui
        PicFrame/picframe.ui
        thruster/ThrusterWindow.ui
        thruster/Thruster.ui
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
        )

set_target_properties(UV_control_box PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UV_control_box)
endif()
