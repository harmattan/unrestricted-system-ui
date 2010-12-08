include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicator

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatoranimationstyle.h \
    $$SRCDIR/statusarea/statusindicatorlabelstyle.h \
    $$SRCDIR/statusarea/statusindicatoriconstyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
SOURCES += ut_statusindicator.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$SRCDIR/statusarea/statusindicatoranimationview.cpp \
    $$SRCDIR/statusarea/statusindicatorlabelview.cpp \
    $$SRCDIR/statusarea/statusindicatoriconview.cpp
# helper classes
SOURCES +=  \
    $$STUBSDIR/testcontextitem.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += ut_statusindicator.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/applicationcontext.h  \
    $$SRCDIR/statusarea/statusindicatoranimationstyle.h \
    $$SRCDIR/statusarea/statusindicatorlabelstyle.h \
    $$SRCDIR/statusarea/statusindicatoriconstyle.h \
    $$SRCDIR/statusarea/statusindicatormodel.h

# helper classes
HEADERS +=  \
    $$STUBSDIR/testcontextitem.h

# base classes
# HEADERS += \
# service classes
HEADERS += $$SRCDIR/statusarea/statusindicatoranimationview.h \
    $$SRCDIR/statusarea/statusindicatorlabelview.h \
    $$SRCDIR/statusarea/statusindicatoriconview.h

