include(../../mconfig.pri)
INCLUDEPATH += lockscreen statusarea

STYLE_HEADERS += lockscreen/lockscreenheaderstyle.h \
    lockscreen/lockscreenheaderwithpadlockstyle.h \
    lockscreen/lockscreenstatusareastyle.h \
    lockscreen/datestyle.h

HEADERS += \
    lockscreen/lockscreenbusinesslogic.h \
    lockscreen/lockscreenwindow.h \
    lockscreen/eventeater.h \
    lockscreen/lockscreenbusinesslogicadaptor.h \
    lockscreen/unlocknotifications.h \
    lockscreen/unlocknotificationsink.h \
    lockscreen/unlockmissedevents.h \
    lockscreen/unlockarea.h \
    lockscreen/lockscreenview.h \
    lockscreen/lockscreenwithpadlockview.h \
    lockscreen/lockscreen.h \
    lockscreen/lockscreenheaderview.h \
    lockscreen/lockscreenheaderstyle.h \
    lockscreen/lockscreenheaderwithpadlockview.h \
    lockscreen/lockscreenheaderwithpadlockstyle.h \
    lockscreen/lockscreenstatusareaview.h \
    lockscreen/lockscreenstatusareastyle.h \
    lockscreen/date.h \
    lockscreen/datestyle.h

SOURCES += \
    lockscreen/lockscreenbusinesslogic.cpp \
    lockscreen/lockscreenwindow.cpp \
    lockscreen/eventeater.cpp \
    lockscreen/lockscreenbusinesslogicadaptor.cpp \
    lockscreen/unlocknotifications.cpp \
    lockscreen/unlocknotificationsink.cpp \
    lockscreen/unlockmissedevents.cpp \
    lockscreen/unlockarea.cpp \
    lockscreen/lockscreenview.cpp \
    lockscreen/lockscreenwithpadlockview.cpp \
    lockscreen/lockscreen.cpp \
    lockscreen/lockscreenheaderview.cpp \
    lockscreen/lockscreenheaderwithpadlockview.cpp \
    lockscreen/lockscreenstatusareaview.cpp \
    lockscreen/date.cpp

