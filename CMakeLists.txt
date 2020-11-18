cmake_minimum_required(VERSION 3.9)
project(seg_box)
set(CMAKE_CXX_STANDARD 14)
set(Torch_DIR /home/wy/libtorch/share/cmake/Torch)
SET(CMAKE_CXX_FLAGS "-Wl,-rpath=3rdparty/opencv-3.4.1/lib")


INCLUDE_DIRECTORIES(3rdparty/opencv-3.4.1/include)
LINK_DIRECTORIES(3rdparty/opencv-3.4.1/lib)
LINK_LIBRARIES(libopencv_core.so libopencv_highgui.so libopencv_imgproc.so libopencv_imgcodecs.so libopencv_videoio.so)

find_package(OpenCV)
find_package(Torch REQUIRED)



add_executable(seg_box main.cpp dfi.cpp dfi.h calib.h calib.cpp camera.cpp camera.hpp
        cal_height.h after_net.cpp after_net.hpp cal_height.cpp use_montage.h use_montage.cpp
        data.h collage.cpp collage.h maxflow/block.h maxflow/graph.cpp maxflow/graph.h
        maxflow/instances.inc maxflow/maxflow.inc)

target_link_libraries(seg_box ${TORCH_LIBRARIES})