QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbconnector.cpp \
    dlg_adddoctor.cpp \
    dlg_addmedicine.cpp \
    dlg_addpatient.cpp \
    dlg_addprescription.cpp \
    dlg_addregistration.cpp \
    dlg_patientprescriptions.cpp \
    dlg_prescriptiondetail.cpp \
    doctor.cpp \
    main.cpp \
    mainwindow.cpp \
    medicine.cpp \
    patient.cpp \
    prescription.cpp \
    registration.cpp

HEADERS += \
    dbconnector.h \
    dlg_adddoctor.h \
    dlg_addmedicine.h \
    dlg_addpatient.h \
    dlg_addprescription.h \
    dlg_addregistration.h \
    dlg_patientprescriptions.h \
    dlg_prescriptiondetail.h \
    doctor.h \
    mainwindow.h \
    medicine.h \
    patient.h \
    prescription.h \
    registration.h

FORMS += \
    dlg_adddoctor.ui \
    dlg_addmedicine.ui \
    dlg_addpatient.ui \
    dlg_addprescription.ui \
    dlg_addregistration.ui \
    dlg_patientprescriptions.ui \
    dlg_prescriptiondetail.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../../Stumgr/stumgr.cpp/res.qrc
