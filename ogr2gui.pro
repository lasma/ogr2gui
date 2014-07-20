# ogr2gui 0.6

TEMPLATE = app
TARGET = ogr2gui
DEPENDPATH += . inc src
INCLUDEPATH += . inc src /usr/local/include /opt/PostgreSQL/9.2/include

# Input
HEADERS += config.h \
	   inc/App.h \
           inc/Dta.h \
           inc/Ogr.h \
	   inc/Inf.h \
           src/App.cpp \
           src/Ogr.cpp

SOURCES += src/App.cpp \
	   src/Ogr.cpp \
	   src/Inf.cpp

LIBS += -L/usr/loca/lib -lgdal -lproj
#LIBS += -L/usr/loca/lib -lgdal -lproj -lpq

CONFIG += static

QT += gui
QT += sql
