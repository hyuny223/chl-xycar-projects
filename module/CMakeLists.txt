cmake_minimum_required(VERSION 3.5)
project(module LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MODULE1_SOURCE_FILES
        src/drawLane.cpp
        src/drive.cpp
        src/histogram.cpp
        src/hyperparam.cpp
        src/polyfit.cpp
        src/warpProcessImage.cpp
        )

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)


add_library(${PROJECT_NAME} SHARED
        ${MODULE1_SOURCE_FILES}
        )


target_include_directories(${PROJECT_NAME} PUBLIC
        include
        ${OpenCV_INCLUDE_DIR}
        ${Eigen3_INCLUDE_DIR}
        )


target_link_libraries(${PROJECT_NAME} PUBLIC
        ${OpenCV_LIBS}
        Eigen3::Eigen
        )
