cmake_minimum_required(VERSION 3.16)

project(Ustanak VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        resources.qrc

)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Ustanak
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/base/testBattle.h
        src/base/testBattle.cpp
        src/base/Army.cpp
        src/base/Battle.cpp
        src/base/Strength.cpp
        src/base/Terrain.cpp
        src/base/Terrain.h
        src/base/player.cpp
        src/base/player.h
        src/base/mergeArmies.cpp
        src/base/mergeArmies.h
        src/gui/maplayer.h src/gui/maplayer.cpp
        src/gui/mapscene.h src/gui/mapscene.cpp
        src/graph/edge.hpp src/graph/edge.cpp
        src/graph/graph.hpp src/graph/graph.cpp
        src/graph/vertex.hpp src/graph/vertex.cpp
        resources.qrc


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Ustanak APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()

    add_executable(Ustanak
        ${PROJECT_SOURCES}
        src/base/testBattle.cpp
        src/base/Army.cpp
        src/base/Battle.cpp
        src/base/Strength.cpp
    )
    if(ANDROID)
        add_library(Ustanak SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Ustanak
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Ustanak PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Ustanak)
endif()
set_target_properties(Ustanak PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Ustanak
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Ustanak)
endif()
