cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(CrowdSecurityMetrics)

# boost
set(Boost_ROOT "C:/boost")
set(Boost_INCLUDE_DIR "C:/boost")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# libtorch
list(APPEND CMAKE_PREFIX_PATH "C:\\libtorch")
find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# opencv
list(APPEND CMAKE_PREFIX_PATH "C:\\opencv\\build")
find_package(OpenCV REQUIRED)

# fltk
add_subdirectory(fltk)
include_directories(${CMAKE_SOURCE_DIR}/fltk)

# FrameServer build
set(FRAMESERVER_DIR ${CMAKE_SOURCE_DIR}/FrameServer)
set(FRAMESERVER_INCLUDE_DIR ${FRAMESERVER_DIR}/include)
set(FRAMESERVER_SRC_DIR ${FRAMESERVER_DIR}/src)

# Collect CrowdSecurityMetrics source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(CrowdSecurityMetrics ${SRC_FILES})

# Collect FrameServer source files
file(GLOB_RECURSE FRAMESERVER_SRC_FILES CONFIGURE_DEPENDS ${FRAMESERVER_SRC_DIR}/*.cpp)
add_executable(FrameServer ${FRAMESERVER_SRC_FILES})

# Include directories
target_include_directories(CrowdSecurityMetrics PRIVATE ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
target_include_directories(FrameServer PRIVATE ${FRAMESERVER_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

# Linking
target_link_libraries(CrowdSecurityMetrics fltk ${TORCH_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_link_libraries(FrameServer fltk ${OpenCV_LIBS} ${Boost_LIBRARIES})

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET CrowdSecurityMetrics
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:CrowdSecurityMetrics>)
endif (MSVC)