
INCLUDEPATH += \
    volumecontrol/ \
    /usr/include/resource/qt4 

STYLE_HEADERS += \
    volumecontrol/volumebarstyle.h

HEADERS+= \
    volumecontrol/volumebarlogic.h \
    volumecontrol/volumebar.h \
    volumecontrol/volumebarwindow.h \
    $$STYLE_HEADERS

SOURCES+= \
    volumecontrol/volumebarlogic.cpp \
    volumecontrol/volumebar.cpp \
    volumecontrol/volumebarwindow.cpp

