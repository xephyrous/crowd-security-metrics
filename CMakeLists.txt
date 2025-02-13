cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(CrowdSecurityMetrics)

# libtorch
list(APPEND CMAKE_PREFIX_PATH "C:\\libtorch")
find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# fltk
add_subdirectory(fltk)
include_directories(${CMAKE_SOURCE_DIR}/fltk)

# Collect source files
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(CrowdSecurityMetrics ${SRC_FILES})

# Linking
target_link_libraries(CrowdSecurityMetrics fltk "${TORCH_LIBRARIES}")

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