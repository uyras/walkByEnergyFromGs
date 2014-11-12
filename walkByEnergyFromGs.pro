TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -L$$PWD/../PartsEngine -lPartsEngine

INCLUDEPATH += ../PartsEngine/

OTHER_FILES += \
    README.md \
    .gitignore
