TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
unix:!macx {
    CONFIG += c++2a
}
macx {
    DEFINES += OS_MAC
    CONFIG += c++2a
}
SOURCES += \
        220329.cpp \
        220330.cpp \
        220331.cpp \
        220403.cpp \
        220404.cpp \
        220405.cpp \
        220406.cpp \
        220407.cpp \
        220408.cpp \
        220409.cpp \
        220410.cpp \
        220411.cpp \
        220412.cpp \
        220413.cpp \
        220414.cpp \
        220415.cpp \
        220416.cpp \
        220417.cpp \
        220418.cpp \
        220419.cpp \
        220420.cpp \
        220421.cpp \
        220423.cpp \
        220424.cpp \
        220425.cpp \
        220426.cpp \
        220427.cpp \
        220429.cpp \
        220430.cpp \
        220501.cpp \
        220502.cpp \
        220503.cpp \
        220504.cpp \
        220505.cpp \
        220506.cpp \
        220507.cpp \
        220508.cpp \
        220509.cpp \
        220510.cpp \
        220511.cpp \
        220512.cpp \
        220513.cpp \
        220514.cpp \
        220515.cpp \
        220516.cpp \
        220517.cpp \
        220518.cpp \
        220519.cpp \
        220520.cpp \
        220521.cpp \
        220522.cpp \
        220523.cpp \
        220524.cpp \
        220525.cpp \
        220526.cpp \
        220527.cpp \
        220528.cpp \
        220530.cpp \
        220531.cpp \
        220602.cpp \
        220604.cpp \
        220605.cpp \
        main.cpp

LIBS += -lpthread
