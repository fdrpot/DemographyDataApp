QT       += core gui core5compat charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addform.cpp \
    censusreader.cpp \
    citiestablewidget.cpp \
    cityviewform.cpp \
    inttableitem.cpp \
    main.cpp \
    citiesviewwidget.cpp

HEADERS += \
    addform.h \
    censusreader.h \
    citiestablewidget.h \
    citiesviewwidget.h \
    cityviewform.h \
    inttableitem.h

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
