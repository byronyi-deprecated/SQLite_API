TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    SQLite/sqlite3.c \
    SpectraSTLib.cpp \
    SpectraSTLibImporter.cpp \
    SpectraSTLibEntry.cpp \
    SpectraSTMzXMLLibImporter.cpp \
    SpectraSTMspLibImporter.cpp \
    SpectraSTSearchTask.cpp \
    SpectraSTCreateParams.cpp \
    SpectraSTSearchParams.cpp \
    SpectraSTPeakList.cpp

OTHER_FILES += \
    database.db

HEADERS += \
    SQLite/sqlite3.h \
    SpectraSTLib.hpp \
    SpectraSTLibImporter.hpp \
    SpectraSTLibEntry.hpp \
    SpectraSTMzXMLLibImporter.hpp \
    SpectraSTMspLibImporter.hpp \
    SpectraSTSearchTask.hpp \
    SpectraSTCreateParams.hpp \
    SpectraSTSearchParams.hpp \
    SpectraSTPeakList.hpp

