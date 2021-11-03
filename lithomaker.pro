TEMPLATE = app
TARGET = Lithomaker
DEPENDPATH += .
INCLUDEPATH += .
CONFIG +=
RESOURCES += lithomaker.qrc
RC_FILE = lithomaker.rc
QT += widgets
TRANSLATIONS = lithomaker_da_DK.ts
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

include(./VERSION)
DEFINES+=VERSION=\\\"$$VERSION\\\"

# Input
HEADERS += src/mainwindow.h \
           src/lineedit.h \
           src/combobox.h \
           src/checkbox.h \
           src/configpages.h \
           src/configdialog.h \
           src/aboutbox.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/lineedit.cpp \
           src/combobox.cpp \
           src/checkbox.cpp \
           src/configpages.cpp \
           src/configdialog.cpp \
           src/aboutbox.cpp
