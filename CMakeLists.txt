add_subdirectory(C:/Users/yosef/Desktop/Libraries/glfw-3.4 C:/Users/yosef/Desktop/Libraries/glfw-3.4/build EXCLUDE_FROM_ALL)

link_libraries(glfw)

include_directories("${GLFW_SOURCE_DIR}/deps")

if(MATH_LIBRARY)
    link_libraries("${MATH_LIBRARY}")
endif()

# Workaround for the MS CRT deprecating parts of the standard library
if(MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(ICON glfw.rc)

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h")
set(GLAD_GLES2 "${GLFW_SOURCE_DIR}/deps/glad/gles2.h")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
    "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
    "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

add_executable(main WIN32 MACOSX_BUNDLE main.c ${ICON} ${GLAD_GL})
add_executable(webvga WIN32 MACOSX_BUNDLE webvga.cpp window.cpp pollable.cpp framesrv.cpp ${ICON} ${GLAD_GL})
add_executable(ring_buf_test test_ring_buf.cpp)

set(APPS main webvga ring_buf_test)

if(MSVC)
    # Tell MSVC to use main instead of WinMain
    set_target_properties(${APPS} PROPERTIES
        LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif(CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    # Tell Clang using MS CRT to use main instead of WinMain
    set_target_properties(${APPS} PROPERTIES
        LINK_FLAGS "-Wl,/entry:mainCRTStartup")
endif()
