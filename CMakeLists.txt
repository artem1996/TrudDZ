cmake_minimum_required(VERSION 3.6)
project(ArinaTrudHW)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp MathModel.cpp MathModel.h Resistor.cpp Resistor.h Capacitor.cpp Capacitor.h Inductor.cpp Inductor.h ElectricitySource.cpp ElectricitySource.h gauss.h gauss.cpp VoltageSource.cpp VoltageSource.h BaseElement.cpp BaseElement.h)
add_executable(ArinaTrudHW ${SOURCE_FILES} MathModel.cpp MathModel.h Resistor.cpp Resistor.h Capacitor.cpp Capacitor.h Inductor.cpp Inductor.h ElectricitySource.cpp ElectricitySource.h gauss.h gauss.cpp VoltageSource.cpp VoltageSource.h BaseElement.cpp BaseElement.h)