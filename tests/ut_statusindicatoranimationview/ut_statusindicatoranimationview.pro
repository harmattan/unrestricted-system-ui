include(../common_top.pri)
TARGET = ut_statusindicatoranimationview

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatoranimationstyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit
SOURCES += \
    ut_statusindicatoranimationview.cpp \
    $$SRCDIR/statusarea/statusindicatoranimationview.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatoranimationview.h \
    $$SRCDIR/statusarea/statusindicatoranimationview.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/inputmethodstatusindicatoradaptor.h \
    $$STYLE_HEADERS \
    $$MODEL_HEADERS

include(../common_bot.pri)
