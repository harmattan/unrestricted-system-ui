STUB_PREFIX = ../tests/stubs/
TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    silent \
    debug
LIBS += -lduicontrolpanel
INCLUDEPATH += /usr/include/qt4/dui \
    /usr/include/duicontrolpanel \
    $$STUB_PREFIX
QT += dbus
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = displayapplet.h \
    displaywidget.h \
    displaybrief.h \
    displaytranslation.h \
    displaybusinesslogic.h \
    slidercontainer.h \
    $$STUB_PREFIX/displaystatestub.h
SOURCES = displayapplet.cpp \
    displaywidget.cpp \
    displaybrief.cpp \
    displaybusinesslogic.cpp \
    slidercontainer.cpp \
    $$STUB_PREFIX/displaystatestub.cpp
css.files = displayapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/duicontrolpanel/themes/style
message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
    css \
    desktop
OTHER_FILES += displayapplet.css
