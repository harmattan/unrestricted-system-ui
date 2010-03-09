include(../../duiconfig.pri)

INCLUDEPATH += statusarea

SOURCES += statusarea/statusarea.cpp \
    statusarea/statusareaview.cpp \
    statusarea/statusareawindow.cpp \
    statusarea/clock.cpp \
    statusarea/clockview.cpp \
    statusarea/contextframeworkcontext.cpp \
    statusarea/notifier.cpp

HEADERS += statusarea/statusarea.h \
    statusarea/statusareaview.h \
    statusarea/statusareawindow.h \
    statusarea/statusareamodel.h \
    statusarea/statusareastyle.h \
    statusarea/clock.h \
    statusarea/clockview.h \
    statusarea/clockstyle.h \
    statusarea/clockmodel.h \
    statusarea/contextframeworkcontext.h \
    statusarea/notifier.h \
    statusarea/applicationcontext.h

MODEL_HEADERS +=     statusarea/statusareamodel.h \
      statusarea/clockmodel.h

STYLE_HEADERS += statusarea/statusareastyle.h \
    statusarea/clockstyle.h


