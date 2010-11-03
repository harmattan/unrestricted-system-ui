include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicatorlabelview

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatorlabelstyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit
SOURCES += \
    ut_statusindicatorlabelview.cpp \
    $$SRCDIR/statusarea/statusindicatorlabelview.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatorlabelview.h \
    $$SRCDIR/statusarea/statusindicatorlabelview.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/inputmethodstatusindicatoradaptor.h \
    $$STYLE_HEADERS \
    $$MODEL_HEADERS

include(../common_bot.pri)
