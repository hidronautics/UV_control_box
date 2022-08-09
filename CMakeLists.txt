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

        Control/Control.cpp
        Control/ControlBase.cpp
        Control/ControlWindow.cpp
        Control/Joystick.cpp
        Control/Keyboard.cpp
        Control/Mouse3d.cpp

        Thruster/ThrusterWindow.cpp
        Thruster/Thruster.cpp

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

        Control/Control.h
        Control/ControlBase.h
        Control/ControlWindow.h
        Control/Joystick.h
        Control/Keyboard.h
        Control/Mouse3d.h

        Thruster/ThrusterWindow.h
        Thruster/Thruster.h

        PicFrame/picframe.h
        #----------------------------------
        mainwindow.ui
        Control/ControlWindow.ui
        PicFrame/picframe.ui
        Thruster/ThrusterWindow.ui
        Thruster/Thruster.ui
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

target_link_libraries(UV_control_box PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(UV_control_box PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UV_control_box)
endif()
