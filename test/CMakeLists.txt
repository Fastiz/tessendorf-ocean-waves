

include_directories(../src)

set(SOURCE_FILES Test.cpp)

file(GLOB SOURCES
        *.h
        *.cpp

        utils/*.h
        utils/*.cpp

        abstractions/*.h
        abstractions/*.cpp

        scene/*.h
        scene/*.cpp

        scene/fft/*.h
        scene/fft/*.cpp
        )

add_executable(Test ${SOURCE_FILES} ${SOURCES}) # which files should produce executables

target_link_libraries(Test Src)

target_link_libraries(Test glfw)

target_link_libraries(Test GLEW)

target_link_libraries(Test GLFFT)

find_package(OpenGL REQUIRED)

target_link_libraries(Test OpenGL::GL)
