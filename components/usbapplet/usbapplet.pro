TEMPLATE      = lib
CONFIG       += plugin \
                gui \
                dui \
                silent \
                debug
LIBS += -lduicontrolpanel
INCLUDEPATH += /usr/include/qt4/dui \
               /usr/include/duicontrolpanel
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
MOC_DIR	      = .moc
OBJECTS_DIR   = .objects

HEADERS += usbapplet.h \
           usbbrief.h \
           usbmodes.h \
           usbview.h
SOURCES += usbapplet.cpp \
           usbbrief.cpp \
           usbmodes.cpp \
           usbview.cpp

css.files = usbapplet.css
DESTDIR = lib
desktop.files += usbapplet.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path +=  $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/dui/duicontrolpanel/style

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
            desktop

OTHER_FILES += usbapplet.css

