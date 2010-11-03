include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicatoriconview

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatoranimationstyle.h \
    $$SRCDIR/statusarea/statusindicatoriconstyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit
SOURCES += \
    ut_statusindicatoriconview.cpp \
    $$SRCDIR/statusarea/statusindicatoriconview.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatoriconview.h \
    $$SRCDIR/statusarea/statusindicatoranimationview.h \
    $$SRCDIR/statusarea/statusindicatoriconview.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/inputmethodstatusindicatoradaptor.h \
    $$STYLE_HEADERS \
    $$MODEL_HEADERS

include(../common_bot.pri)
