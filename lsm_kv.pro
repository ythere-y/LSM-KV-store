QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bloomfilter.cpp \
    kvstore.cc \
    correctness.cc \
    memtable.cpp \
    skip_list.cpp \
    skip_list_mem.cpp \
    sstable.cpp \
    persistence.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    BitMap.h \
    MurmurHash3.h \
    arena.h \
    bloomfilter.h \
    kvstore.h \
    kvstore_api.h \
    memtable.h \
    skip_list.h \
    skip_list_mem.h \
    sstable.h \
    test.h \
    utils.h
