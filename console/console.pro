TARGET = console

QT -= gui
QT += network concurrent sql

CONFIG += c++2a console
CONFIG -= app_bundle

INCLUDEPATH += ./include \
    ./include/kdbindings

SOURCES = main.cpp \
        src/law_of_demeter.cpp \
        src/meta/test_binary.cpp \
        src/meta/test_meta_ch2.cpp \
        src/meta/test_traits.cpp \
        src/test_adl.cpp \
        src/test_array.cpp \
        src/test_atomic.cpp \
        src/test_cbor.cpp \
        src/test_concurrency.cpp \
        src/test_concurrent.cpp \
        src/test_crtp.cpp \
        src/test_cxx17.cpp \
        src/test_cxx2a.cpp \
        src/test_duck_typing.cpp \
        src/test_emplace_back.cpp \
        src/test_forwarding.cpp \
        src/test_function.cpp \
        src/test_hash.cpp \
        src/test_implicitSharing.cpp \
        src/test_iodevice.cpp \
        src/test_json.cpp \
        src/test_lambda.cpp \
        src/test_locale.cpp \
        src/test_network.cpp \
        src/test_partial_function.cpp \
        src/test_promise.cpp \
        src/test_qcache.cpp \
        src/test_qsettings.cpp \
        src/test_qtcontainer.cpp \
        src/test_regexp.cpp \
        src/test_rreference.cpp \
        src/test_sfinae.cpp \
        src/test_sharedmemory.cpp \
        src/test_sql.cpp \
        src/test_stringview.cpp \
        src/test_kdbindings.cpp \
        src/test_typetraits.cpp \
        src/test_xml.cpp

HEADERS +=

RESOURCES += \
    console.qrc
