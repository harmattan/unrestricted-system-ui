include(../../mconfig.pri)
INCLUDEPATH += lockscreen

HEADERS += \
    lockscreen/lockscreenbusinesslogic.h           \
    lockscreen/lockscreenwindow.h                  \
    lockscreen/eventeater.h                        \
    lockscreen/lockscreenbusinesslogicadaptor.h    \
    lockscreen/unlocknotifications.h               \
    lockscreen/unlocknotificationsink.h            \
    lockscreen/unlockmissedevents.h                \
    lockscreen/unlockwidgets.h                     \
    lockscreen/lockscreenview.h \
    lockscreen/lockscreenwithpadlockview.h \
    lockscreen/lockscreen.h

SOURCES += \
    lockscreen/lockscreenbusinesslogic.cpp         \
    lockscreen/lockscreenwindow.cpp                \
    lockscreen/eventeater.cpp                      \
    lockscreen/lockscreenbusinesslogicadaptor.cpp  \
    lockscreen/unlocknotifications.cpp             \
    lockscreen/unlocknotificationsink.cpp          \
    lockscreen/unlockmissedevents.cpp              \
    lockscreen/unlockwidgets.cpp                   \
    lockscreen/lockscreenview.cpp \
    lockscreen/lockscreenwithpadlockview.cpp \
    lockscreen/lockscreen.cpp
