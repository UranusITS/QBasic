QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    expression/conexp.cpp \
    expression/expression.cpp \
    expression/optexp.cpp \
    expression/varexp.cpp \
    main.cpp \
    mainwindow.cpp \
    statement/endstm.cpp \
    statement/gotostm.cpp \
    statement/ifstm.cpp \
    statement/inputstm.cpp \
    statement/letstm.cpp \
    statement/printstm.cpp \
    statement/remstm.cpp \
    statement/statement.cpp

HEADERS += \
    expression/conexp.h \
    expression/expression.h \
    expression/optexp.h \
    expression/varexp.h \
    mainwindow.h \
    statement/endstm.h \
    statement/gotostm.h \
    statement/ifstm.h \
    statement/inputstm.h \
    statement/letstm.h \
    statement/printstm.h \
    statement/remstm.h \
    statement/statement.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
