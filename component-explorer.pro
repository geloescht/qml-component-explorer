# Define the project name
TEMPLATE = lib
TARGET = component-explorer
CONFIG += shared plugin no_plugin_name_prefix

xoviextension.target = xovi.cpp
xoviextension.commands = python3 $$(XOVI_REPO)/util/xovigen.py -o xovi.cpp -H xovi.h component-explorer.xovi
xoviextension.depends = component-explorer.xovi

QMAKE_EXTRA_TARGETS += xoviextension
PRE_TARGETDEPS += xovi.cpp

# Define the Qt modules required
QT += quick qml

# Define the C++ standard version
CONFIG += c++11

# Specify the source files
SOURCES += main.cpp xovi.cpp 

HEADERS += TypeList.h MethodList.h PropertyList.h EnumList.h

QMAKE_CXXFLAGS += -fPIC 

# QMAKE_CXX = aarch64-remarkable-linux-g++
