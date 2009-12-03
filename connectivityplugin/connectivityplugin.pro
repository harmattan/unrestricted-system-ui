MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
LIBS += -lduicontrolpanel \
    -lcellular-qt
TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    duistatusindicatormenu \
    cellular-qt
INCLUDEPATH += /usr/include/duistatusindicatormenu \
    /usr/include/duicontrolpanel \
    ../src \
    ../networkapplet
QT += dbus
HEADERS = connectivityplugin.h \
    connectivity.h \
    phonenetworkcontainer.h \
    offlinecontainer.h \
    networksignalstrengthicon.h \
    ../src/systemuigconf.h \
    ../networkapplet/networktechnology.h \
    linklabel.h
SOURCES = connectivityplugin.cpp \
    connectivity.cpp \
    phonenetworkcontainer.cpp \
    offlinecontainer.cpp \
    networksignalstrengthicon.cpp \
    ../src/systemuigconf.cpp \
    ../networkapplet/networktechnology.cpp \
    linklabel.cpp
TARGET = $$qtLibraryTarget(connectivity)
css.files = connectivityplugin.css
css.path += /usr/share/duistatusindicatormenu/themes/style/
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins/
INSTALLS += target \
    css
