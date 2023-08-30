#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Qt Creator project file, using Qt Creator as a general-purpose C++ IDE.
#----------------------------------------------------------------------------

TARGET  = cryptobench
SOURCES = $$files(*.cpp, true)
HEADERS = $$files(*.h, true)

CONFIG *= thread
CONFIG *= largefile
CONFIG *= c++17
CONFIG *= unversioned_libname
CONFIG *= no_plugin_name_prefix
CONFIG -= qt
CONFIG -= debug_and_release
CONFIG -= app_bundle
CONFIG += sdk_no_version_check
CONFIG -= warn_off
CONFIG *= warn_on

QMAKE_CXXFLAGS_WARN_ON = -Werror -Wall -Wextra -Wno-unused-parameter

QMAKE_MACOSX_DEPLOYMENT_TARGET = 11

DEFINES += LTM_DESC
DEFINES += GMP_DESC

LIBS += -ltomcrypt -ltommath -lgnutls -lmbedcrypto -lcrypto -lgmp -lm

mac {
    exists(/opt/homebrew) {
        QMAKE_CXXFLAGS += -I/opt/homebrew/include
        QMAKE_LFLAGS += -L/opt/homebrew/lib
    }
    else {
        QMAKE_CXXFLAGS += -I/usr/local/include
        QMAKE_LFLAGS += -L/usr/local/lib
    }
}
