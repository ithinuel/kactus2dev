# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = VerilogImport

QT += core xml widgets gui
CONFIG += c++11 release

DEFINES += VERILOGIMPORT_LIB

INCLUDEPATH += ./../.. \
    ./generatedFiles \
    . \
    ./generatedFiles/Debug
LIBS += -L"./../../executable" \
    -lIPXACTmodels

DESTDIR = ../../executable/Plugins

DEPENDPATH += .
MOC_DIR += ./generatedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./generatedFiles
RCC_DIR += ./generatedFiles

include(VerilogImport.pri)

target.path = $$plugin_path
INSTALLS += target

