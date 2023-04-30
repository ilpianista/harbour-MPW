TARGET = harbour-mpw

CONFIG += sailfishapp

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

QT += sql

SOURCES += \
    src/main.cpp \
    src/asyncmasterkey.cpp \
    src/dbmanager.cpp \
    src/mpwmanager.cpp \
    src/sitessqlmodel.cpp

HEADERS += \
    src/asyncmasterkey.h \
    src/dbmanager.h \
    src/mpwmanager.h \
    src/sitessqlmodel.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    qml/pages/SiteDelegate.qml \
    qml/MPW.qml \
    harbour-mpw.desktop \
    harbour-mpw.png \
    rpm/harbour-mpw.changes \
    rpm/harbour-mpw.spec \
    rpm/harbour-mpw.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n

INCLUDEPATH += "/usr/include/mpw"

LIBS += -lmpw -lsodium

TRANSLATIONS += \
    translations/harbour-mpw-ar.ts \
    translations/harbour-mpw-de.ts \
    translations/harbour-mpw-et.ts \
    translations/harbour-mpw-fi.ts \
    translations/harbour-mpw-fr.ts \
    translations/harbour-mpw-hr.ts \
    translations/harbour-mpw-it.ts \
    translations/harbour-mpw-nb_NO.ts \
    translations/harbour-mpw-nl_BE.ts \
    translations/harbour-mpw-nl.ts \
    translations/harbour-mpw-sv.ts \
    translations/harbour-mpw-tr.ts
