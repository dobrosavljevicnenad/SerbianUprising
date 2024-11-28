cmake_minimum_required(VERSION 3.16)

project(Ustanak VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        resources.qrc

)

set(CMAKE_BUILD_TYPE Debug)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Ustanak
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/base/Test/testBattle.h src/base/Test/testBattle.cpp
        src/base/Entities/Army.h src/base/Entities/Army.cpp
        src/base/Mechanics/Battle.h src/base/Mechanics/Battle.cpp
        src/base/Mechanics/Strength.h src/base/Mechanics/Strength.cpp
        src/base/Entities/Terrain.h src/base/Entities/Terrain.cpp
        src/base/Entities/player.h src/base/Entities/player.cpp
        src/base/Mechanics/MoveArmy.h src/base/Mechanics/MoveArmy.cpp
        src/base/Entities/mergeArmies.h src/base/Entities/mergeArmies.cpp
        src/Interface/maplayer.h src/Interface/maplayer.cpp
        src/Interface/mapscene.h src/Interface/mapscene.cpp
        src/base/graph/edge.hpp src/base/graph/edge.cpp
        src/base/graph/graph.hpp src/base/graph/graph.cpp
        src/base/graph/vertex.hpp src/base/graph/vertex.cpp
        resources.qrc
        src/Interface/gamemanager.h src/Interface/gamemanager.cpp
        src/base/Entities/city.h src/base/Entities/city.cpp
        src/base/Entities/region.h src/base/Entities/region.cpp
        src/base/Mechanics/turn.h src/base/Mechanics/turn.cpp
        src/base/Mechanics/Action.h
        src/Interface/Items/customarrowitem.h src/Interface/Items/customarrowitem.cpp
        src/base/Mechanics/Action.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Ustanak APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
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



target_link_libraries(Ustanak PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)


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
