TEMPLATE = app
TARGET = collatinusd
VERSION = "10.1"
DEFINES += VERSION=\\\"$$VERSION\\\"
DEPENDPATH += .
INCLUDEPATH += .

OBJECTS_DIR= obj/
MOC_DIR = moc/

CONFIG += qt console
CONFIG += release_binary

# Input
HEADERS     += src/*.h 
SOURCES     += src/*.cpp

QT          -= gui
QT          += xmlpatterns
QT          += network
