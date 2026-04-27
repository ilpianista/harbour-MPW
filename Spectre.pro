TARGET = harbour-spectre

CONFIG += sailfishapp

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

QT += sql

SOURCES += \
    src/main.cpp \
    src/asyncmasterkey.cpp \
    src/dbmanager.cpp \
    src/mpwmanager.cpp \
    src/sitessqlmodel.cpp \
    src/spectre_wrapper.c

HEADERS += \
    src/asyncmasterkey.h \
    src/dbmanager.h \
    src/mpwmanager.h \
    src/sitessqlmodel.h \
    src/spectre_wrapper.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    qml/pages/SiteDelegate.qml \
    qml/SPECTRE.qml \
    harbour-spectre.desktop \
    harbour-spectre.png \
    rpm/harbour-spectre.changes \
    rpm/harbour-spectre.spec \
    translations/*.ts

CONFIG += sailfishapp_i18n

TRANSLATIONS += \
    translations/harbour-spectre-ar.ts \
    translations/harbour-spectre-de.ts \
    translations/harbour-spectre-et.ts \
    translations/harbour-spectre-fi.ts \
    translations/harbour-spectre-fr.ts \
    translations/harbour-spectre-hr.ts \
    translations/harbour-spectre-it.ts \
    translations/harbour-spectre-nb_NO.ts \
    translations/harbour-spectre-nl_BE.ts \
    translations/harbour-spectre-nl.ts \
    translations/harbour-spectre-sv.ts \
    translations/harbour-spectre-tr.ts

#CONFIG += link_prl

# Read the GNU host triplet exported by the RPM spec file before %qmake5,
# e.g.: export MER_CROSS_COMPILE=%{_host}
# Falls back to a best-guess so the project still works outside the RPM build
# environment (e.g. when running qmake manually inside an SDK target).
SPECTRE_HOST = $$getenv(MER_CROSS_COMPILE)
isEmpty(SPECTRE_HOST): SPECTRE_HOST = $${QT_ARCH}-unknown-linux-gnueabi

# Per-architecture staging directories so that armv7hl, aarch64, and i486
# builds can coexist without overwriting each other's artifacts.
LIBSODIUM_STAGE = $$_PRO_FILE_PWD_/libsodium/build-$$QT_ARCH

libsodium_so.target   = $$LIBSODIUM_STAGE/usr/lib/libsodium.so
libsodium_so.commands = \
    cd $$_PRO_FILE_PWD_/libsodium && \
    DO_NOT_UPDATE_CONFIG_SCRIPTS=1 ./autogen.sh && \
    ./configure --host=$$SPECTRE_HOST --disable-static --prefix=/usr && \
    make && \
    make install DESTDIR=$$LIBSODIUM_STAGE

LIBS += -L$$LIBSODIUM_STAGE/usr/lib
INCLUDEPATH += $$LIBSODIUM_STAGE/usr/include

QMAKE_EXTRA_TARGETS += libsodium_so
PRE_TARGETDEPS += $$LIBSODIUM_STAGE/usr/lib/libsodium.so

libsodium_so_install.path = /usr/share/harbour-spectre/lib
libsodium_so_install.files = $$LIBSODIUM_STAGE/usr/lib/libsodium.so*
INSTALLS += libsodium_so_install

# libspectre gets its own per-arch output directory.
# The build script's haslib() probes -lsodium via a test-link, so it needs a
# proper -L flag in LDFLAGS; the headers go in CFLAGS via -I.
LIBSPECTRE_STAGE = $$_PRO_FILE_PWD_/libspectre/build-$$QT_ARCH

libspectre_so.target   = $$LIBSPECTRE_STAGE/libspectre.so
libspectre_so.commands = \
    mkdir -p $$LIBSPECTRE_STAGE && \
    cd $$_PRO_FILE_PWD_/libspectre && \
    CFLAGS=\"-fPIC -I$$LIBSODIUM_STAGE/usr/include\" \
    LDFLAGS=\"-L$$LIBSODIUM_STAGE/usr/lib\" \
    gcc -std=c11 -O3 -fPIC -I\"$$LIBSODIUM_STAGE/usr/include\" -DSPECTRE_SODIUM=1 -Ic -L\"$$LIBSODIUM_STAGE/usr/lib\" -lsodium -shared -o \"$$LIBSPECTRE_STAGE/libspectre.so\" c/aes.c c/spectre-algorithm.c c/spectre-algorithm_v0.c c/spectre-algorithm_v1.c c/spectre-algorithm_v2.c c/spectre-algorithm_v3.c c/spectre-types.c c/spectre-util.c c/spectre-marshal-util.c c/spectre-marshal.c
libspectre_so.depends = $$libsodium_so.target

LIBS += -L$$LIBSPECTRE_STAGE -lspectre
LIBS += -L$$LIBSODIUM_STAGE/usr/lib -lsodium
INCLUDEPATH += $$_PRO_FILE_PWD_/libspectre/c

QMAKE_EXTRA_TARGETS += libspectre_so
PRE_TARGETDEPS += $$LIBSPECTRE_STAGE/libspectre.so

libspectre_so_install.path = /usr/share/harbour-spectre/lib
libspectre_so_install.files = $$LIBSPECTRE_STAGE/libspectre.so*
INSTALLS += libspectre_so_install
