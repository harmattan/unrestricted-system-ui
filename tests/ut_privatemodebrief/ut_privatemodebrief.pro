include(../common_top.pri)
TARGET = ut_privatemodebrief
INCLUDEPATH += $$ROOTSRCDIR/privatemodeapplet
DEFINES -= THEMEDIR=\\\"\"$$M_THEME_DIR\"\\\"
CONFIG += duicontrolpanel

# unit test and unit
SOURCES += \
    ut_privatemodebrief.cpp \
    $$ROOTSRCDIR/privatemodeapplet/privatemodebrief.cpp

# base classes
SOURCES += \

# unit test and unit
HEADERS += \
    ut_privatemodebrief.h \
    $$ROOTSRCDIR/privatemodeapplet/privatemodebrief.h

include(../common_bot.pri)
