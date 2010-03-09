include(../common_top.pri)
TARGET = ut_statusindicatorimageview

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatorimagestyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit
SOURCES += \
    ut_statusindicatorimageview.cpp \
    $$SRCDIR/statusarea/statusindicatorimageview.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatorimageview.h \
    $$SRCDIR/statusarea/statusindicatorimageview.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$STYLE_HEADERS \
    $$MODEL_HEADERS

include(../common_bot.pri)
