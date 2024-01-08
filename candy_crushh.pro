TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        src/animations.cpp \
        src/components/button.cpp \
        src/level_manager.cpp


include(MinGL2_IUT_AIX/mingl.pri);

HEADERS += \
    hdrs/animations.h \
    hdrs/components/button.h \
    hdrs/globals.h \
    hdrs/level_manager.h \
    hdrs/utils.h
