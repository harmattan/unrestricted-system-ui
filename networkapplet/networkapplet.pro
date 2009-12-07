TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    cellular-qt \
    silent \
    debug
LIBS += -lduicontrolpanel \
    -lcellular-qt
INCLUDEPATH += /usr/include/qt4/dui \
    /usr/include/duicontrolpanel
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = networkapplet.h \
    networkwidget.h \
    networkbrief.h \
    networktranslation.h \
    networkcontainer.h \
    networklist.h \
    roamingcontainer.h \
    networkbusinesslogic.h \
    dcpnetwork.h \
    networktechnology.h \
    systemuigconf.h
SOURCES = networkapplet.cpp \
    networkwidget.cpp \
    networkbrief.cpp \
    networkcontainer.cpp \
    networklist.cpp \
    roamingcontainer.cpp \
    networkbusinesslogic.cpp \
    networktechnology.cpp \
    systemuigconf.cpp
css.files = networkapplet.css
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
OTHER_FILES += networkapplet.css
