TEMPLATE = subdirs
SUBDIRS = src \
          translations \
          themes \
          demos \
          tests

QMAKE_CLEAN += configure-stamp build-stamp

include(doc/doc.pri)
