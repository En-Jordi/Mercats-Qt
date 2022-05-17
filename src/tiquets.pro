QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auxiliar_bbdd.cpp \
    auxiliar_bbdd_inserir.cpp \
    dialog_abouttiquets.cpp \
    dialog_banks.cpp \
    dialog_beneficiaris.cpp \
    dialog_consultaproductes.cpp \
    dialog_discernirbancs.cpp \
    dialog_inserirproductes.cpp \
    dialog_selecciooperacions.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    bbdd/auxiliar_bbdd.h \
    bbdd/auxiliar_bbdd_inserir.h \
    dialog_abouttiquets.h \
    dialog_banks.h \
    dialog_beneficiaris.h \
    dialog_consultaproductes.h \
    dialog_discernirbancs.h \
    dialog_inserirproductes.h \
    dialog_selecciooperacions.h \
    mainwindow.h \
    model/date.h \
    parametres_Configuracio.h \
    qframe_extended.h \
    util/dateutil.h

FORMS += \
    dialog_abouttiquets.ui \
    dialog_banks.ui \
    dialog_beneficiaris.ui \
    dialog_consultaproductes.ui \
    dialog_discernirbancs.ui \
    dialog_inserirproductes.ui \
    dialog_selecciooperacions.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

DISTFILES += \
    TODO
