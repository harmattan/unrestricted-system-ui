system(./configure)

TEMPLATE = subdirs
SUBDIRS = src \
          translations \
          themes \
          demos \
          tests

QMAKE_CLEAN += configure-stamp build-stamp localconfig.pri

include(doc/doc.pri)
