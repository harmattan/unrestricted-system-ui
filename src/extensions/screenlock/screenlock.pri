include(../../mconfig.pri)
INCLUDEPATH += screenlock statusarea

STYLE_HEADERS += screenlock/screenlockheaderstyle.h \
    screenlock/screenlockheaderwithpadlockstyle.h \
    screenlock/screenlockstatusareastyle.h \
    screenlock/screenlockwindowstyle.h \
    screenlock/datestyle.h

HEADERS += \
    screenlock/screenlockbusinesslogic.h \
    screenlock/screenlockwindow.h \
    screenlock/screenlockwindowstyle.h \
    screenlock/eventeater.h \
    screenlock/screenlockbusinesslogicadaptor.h \
    screenlock/unlocknotifications.h \
    screenlock/unlocknotificationsink.h \
    screenlock/unlockmissedevents.h \
    screenlock/unlockarea.h \
    screenlock/screenlockview.h \
    screenlock/screenlockwithpadlockview.h \
    screenlock/screenlockwithoutpadlockview.h \
    screenlock/screenlock.h \
    screenlock/screenlockheaderview.h \
    screenlock/screenlockheaderstyle.h \
    screenlock/screenlockheaderwithpadlockview.h \
    screenlock/screenlockheaderwithpadlockstyle.h \
    screenlock/screenlockstatusareaview.h \
    screenlock/screenlockstatusareastyle.h \
    screenlock/date.h \
    screenlock/datestyle.h

SOURCES += \
    screenlock/screenlockbusinesslogic.cpp \
    screenlock/screenlockwindow.cpp \
    screenlock/eventeater.cpp \
    screenlock/screenlockbusinesslogicadaptor.cpp \
    screenlock/unlocknotifications.cpp \
    screenlock/unlocknotificationsink.cpp \
    screenlock/unlockmissedevents.cpp \
    screenlock/unlockarea.cpp \
    screenlock/screenlockview.cpp \
    screenlock/screenlockwithpadlockview.cpp \
    screenlock/screenlockwithoutpadlockview.cpp \
    screenlock/screenlock.cpp \
    screenlock/screenlockheaderview.cpp \
    screenlock/screenlockheaderwithpadlockview.cpp \
    screenlock/screenlockstatusareaview.cpp \
    screenlock/date.cpp

