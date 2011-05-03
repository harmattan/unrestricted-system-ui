include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicator

INCLUDEPATH += $$SRCDIR/statusarea

MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
SOURCES += ut_statusindicator.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$STUBSDIR/testcontextitem.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += ut_statusindicator.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/applicationcontext.h  \
    $$SRCDIR/statusarea/statusindicatormodel.h

# helper classes
HEADERS +=  \
    $$STUBSDIR/testcontextitem.h
