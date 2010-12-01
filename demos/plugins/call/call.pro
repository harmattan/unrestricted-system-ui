MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../../mconfig.pri)

TEMPLATE      = lib
CONFIG       += plugin gui meegotouch system-ui silent
INCLUDEPATH  += \
    ../../../src/statusindicatormenu

HEADERS = \
    callplugin.h \
    call.h \

SOURCES = \
    callplugin.cpp \
    call.cpp \

TARGET        = $$qtLibraryTarget(statusindicatormenu-call)
DESTDIR       = ../../../lib

target.path += $$M_APPLICATION_EXTENSION_DIR

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop

INSTALLS += \
    target \
    desktop_entry
