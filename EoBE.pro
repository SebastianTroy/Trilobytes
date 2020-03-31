QT       += core gui widgets

CONFIG += c++17

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
    Egg.cpp \
    EnergyPool.cpp \
    Entity.cpp \
    FoodPellet.cpp \
    LineGraph.cpp \
    NeuralNetwork.cpp \
    QuadTree.cpp \
    Random.cpp \
    Seed.cpp \
    Sensors/Sense.cpp \
    Sensors/SenseEntityDistance.cpp \
    Sensors/SenseEntityPresence.cpp \
    Sensors/SenseMagneticField.cpp \
    Sensors/SenseRandom.cpp \
    Swimmer.cpp \
    Universe.cpp \
    Utils.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Egg.h \
    Energy.h \
    EnergyPool.h \
    Entity.h \
    EntityContainerInterface.h \
    FoodPellet.h \
    LineGraph.h \
    MainWindow.h \
    NeuralNetwork.h \
    QuadTree.h \
    Random.h \
    Seed.h \
    Sensors/Sense.h \
    Sensors/SenseEntityDistance.h \
    Sensors/SenseEntityPresence.h \
    Sensors/SenseMagneticField.h \
    Sensors/SenseRandom.h \
    Shape.h \
    Swimmer.h \
    Universe.h \
    Utils.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
