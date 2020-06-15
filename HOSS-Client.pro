QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000# disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	CheckItem.cpp \
	CheckItem_EditDialog.cpp \
	Homework.cpp \
	RemoteAPI.cpp \
	RemoteAPI_ProgressDialog.cpp \
	Settings.cpp \
	Settings_EditDialog.cpp \
	Storage.cpp \
	main.cpp

HEADERS += \
	CheckItem.h \
	CheckItem_EditDialog.h \
	Homework.h \
	RemoteAPI.h \
	RemoteAPI_ProgressDialog.h \
	Settings.h \
	Settings_EditDialog.h \
	Storage.h

FORMS += \
	CheckItem.ui \
	CheckItem_EditDialog.ui \
	Homework.ui \
	RemoteAPI_ProgressDialog.ui \
	Settings_EditDialog.ui

RC_ICONS = logo.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	FontAwesome.min.qrc \
	logo.qrc
