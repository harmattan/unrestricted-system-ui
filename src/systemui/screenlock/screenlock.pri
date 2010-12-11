include(../../mconfig.pri)
INCLUDEPATH += screenlock statusarea

STYLE_HEADERS += screenlock/screenlockwindowstyle.h

HEADERS += \
    screenlock/screenlockbusinesslogic.h \
    screenlock/screenlockbusinesslogicadaptor.h \
    screenlock/screenlockwindow.h \
    screenlock/screenlockwindowstyle.h \
    screenlock/eventeater.h

SOURCES += \
    screenlock/screenlockbusinesslogic.cpp \
    screenlock/screenlockbusinesslogicadaptor.cpp \
    screenlock/screenlockwindow.cpp \
    screenlock/eventeater.cpp

headers.path = /usr/include/system-ui
headers.files += \
    screenlock/screenlockextensioninterface.h \
    screenlock/ScreenLockExtensionInterface

INSTALLS += \
    headers
