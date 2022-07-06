cmake_minimum_required(VERSION 3.20)
project(PlaylistMaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_SDK_DIR "/Users/xmcy0011/Qt/5.12.11/clang_64/")
set(CMAKE_PREFIX_PATH "/Users/zhangzhenhu/Qt/6.3.0/macos/lib/cmake")
#set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
#set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
#set(CMAKE_EXE_LINKER_FLAGS -static)
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")

set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_BUILD_RPATH "/Users/zhangzhenhu/Qt/6.3.0/macos/lib/")


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
#        multimediawidgets
        REQUIRED)

set(TS_FILES PlaylistMaker_zh_CN.ts)
set(ICON_NAME "logo.icns")
set(${PROJECT_NAME}_MAC_ICON "${PROJECT_SOURCE_DIR}/${ICON_NAME}")
set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
set_property(SOURCE "${${PROJECT_NAME}_MAC_ICON}" PROPERTY
        MACOSX_PACKAGE_LOCATION Resources)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        player/playlistmodel.cpp
        player/playlistmodel.h
        player/playercontrols.h
        player/playercontrols.cpp
        player/player.h
        player/player.cpp
        player/qmediaplaylist.cpp
        player/qmediaplaylist.h
        player/qmediaplaylist_p.h
        player/qplaylistfileparser.cpp
        player/qplaylistfileparser_p.h
        ${TS_FILES}
        player/playlistwidget.cpp player/playlistwidget.h
        player/m3uplaylistwriter.cpp player/m3uplaylistwriter.h
        filebrowser/filebrowserwidget.cpp filebrowser/filebrowserwidget.h)

qt_standard_project_setup()

add_executable(PlaylistMaker
        main.cpp icon.qrc "app_win32.rc"
        ${${PROJECT_NAME}_MAC_ICON}
#        MACOSX_BUNDLE
        ${PROJECT_SOURCES})
target_link_libraries(PlaylistMaker
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
#        Qt::MultimediaWidgets
        )

set_target_properties(PlaylistMaker PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )
#set_target_properties(PlaylistMaker PROPERTIES
#        MACOSX_BUNDLE TRUE
#        )
set_property(TARGET ${PROJECT_NAME} PROPERTY
        MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/macOS/Info.plist.in")

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PlaylistMaker)
endif()

