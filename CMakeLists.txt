cmake_minimum_required(VERSION 3.10)
project(Lab_08_Vision)

find_package(OpenCV REQUIRED)

add_executable(f110_cam test.cpp camera.cpp)

target_include_directories(f110_cam PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(f110_cam ${OpenCV_LIBS})