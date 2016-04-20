TARGET = harbour-mpw

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/mpwmanager.cpp

HEADERS += \
    src/mpwmanager.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    qml/MPW.qml \
    harbour-mpw.desktop \
    harbour-mpw.png \
    rpm/harbour-mpw.changes \
    rpm/harbour-mpw.spec \
    rpm/harbour-mpw.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n

INCLUDEPATH += "/usr/include/mpw"

LIBS += -lmpw -lcrypto
