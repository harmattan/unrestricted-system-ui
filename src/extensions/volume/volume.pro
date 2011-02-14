MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
LOCALLIBSDIR = ../../../lib

include(../../../mconfig.pri)
include(../../../localconfig.pri)

TEMPLATE      = lib
CONFIG       += plugin gui meegotouch system-ui link_pkgconfig
QT           += dbus
INCLUDEPATH  += \
    ../../libnotificationsystem \
    ../../systemui \
    /usr/include/resource/qt4

QMAKE_LIBDIR += $$LOCALLIBSDIR

STYLE_HEADERS += volumebarstyle.h

HEADERS += \
    volumeextension.h \
    volumebarlogic.h \
    volumebar.h \
    volumebarwindow.h \
    volumebarstyle.h \
    ../../systemui/closeeventeater.h

SOURCES += \
    volumeextension.cpp \
    volumebarlogic.cpp \
    volumebar.cpp \
    volumebarwindow.cpp \
    ../../systemui/closeeventeater.cpp

TARGET        = $$qtLibraryTarget(sysuid-volume)
DESTDIR       = $$LOCALLIBSDIR

contains(DEFINES, HAVE_QMSYSTEM) {
    PKGCONFIG += qmsystem2
}

contains(DEFINES, HAVE_LIBRESOURCEQT) {
    PKGCONFIG += libresourceqt1
}

PKGCONFIG += \
    dbus-1 \
    dbus-glib-1

target.path += $$M_APPLICATION_EXTENSION_DIR

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop

INSTALLS += \
    target \
    desktop_entry
