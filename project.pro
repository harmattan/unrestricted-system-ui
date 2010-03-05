TEMPLATE = subdirs
SUBDIRS = src \
          tests \
          translations \
          themes \
          demos

QMAKE_CLEAN += configure-stamp build-stamp

include(doc/doc.pri)
