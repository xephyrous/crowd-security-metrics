cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(CrowdSecurityMetrics)

# Boost
set(Boost_ROOT "C:/boost")
set(Boost_INCLUDE_DIR "C:/boost")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Libtorch
list(APPEND CMAKE_PREFIX_PATH "C:\\libtorch")
find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV
set(APPEND CMAKE_PREFIX_PATH "C:\\opencv\\build")
find_package(OpenCV REQUIRED)

# FLTK
add_subdirectory(fltk)
include_directories(${CMAKE_SOURCE_DIR}/fltk)

# Collect source files for CrowdSecurityMetrics
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(CrowdSecurityMetrics ${SRC_FILES})

# Include directories
target_include_directories(CrowdSecurityMetrics PRIVATE ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})

# Linking
target_link_libraries(CrowdSecurityMetrics fltk ${TORCH_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

# FrameServer executable
file(GLOB_RECURSE FRAME_SERVER_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/FrameServer/src/*.cpp)
add_executable(FrameServer ${FRAME_SERVER_SRC} ${CMAKE_SOURCE_DIR}/FrameServer/main.cpp)

# Include directories for FrameServer
target_include_directories(FrameServer PRIVATE ${CMAKE_SOURCE_DIR}/FrameServer/include ${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

# Linking for FrameServer
target_link_libraries(FrameServer fltk ${TORCH_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

# DLL copying for Windows
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    foreach(EXE_TARGET CrowdSecurityMetrics FrameServer)
        add_custom_command(TARGET ${EXE_TARGET}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${TORCH_DLLS}
                $<TARGET_FILE_DIR:${EXE_TARGET}>)
    endforeach()
endif (MSVC)