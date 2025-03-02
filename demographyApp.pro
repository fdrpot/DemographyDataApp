QT       += core gui core5compat charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sources/addform.cpp \
    sources/censusreader.cpp \
    sources/citiestablewidget.cpp \
    sources/cityviewform.cpp \
    sources/inttableitem.cpp \
    sources/main.cpp \
    sources/citiesviewwidget.cpp

HEADERS += \
    headers/addform.h \
    headers/censusreader.h \
    headers/citiestablewidget.h \
    headers/citiesviewwidget.h \
    headers/cityviewform.h \
    headers/inttableitem.h

RC_ICONS = images/icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
