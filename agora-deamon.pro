######################################################################
# Automatically generated by qmake (2.01a) ven. mars 15 21:02:56 2019
######################################################################

QT	+= core network

TEMPLATE = app
TARGET = agora-deamon

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG += c++11


# Input
SOURCES += 	\
			main.cpp \
			\
			minecraft/TestCrash.cpp \
			\
			discord/SendMsg.cpp \

HEADERS += \
			minecraft/TestCrash.hpp \
			\
			discord/SendMsg.hpp \