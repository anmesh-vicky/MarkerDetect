cmake_minimum_required(VERSION 2.8)
project(Marker_Detect)

SET("OpenCV_DIR" "/usr/share/OpenCV")
find_package( OpenCV REQUIRED )

add_executable(Marker_Detect Marker_Detect.cpp)
target_link_libraries(Marker_Detect ${OpenCV_LIBS})
