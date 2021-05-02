

include_directories(../src)

set(SOURCE_FILES Test.cpp)

file(GLOB SOURCES
        *.h
        *.cpp

        utils/*.h
        utils/*.cpp

        abstractions/*.h
        abstractions/*.cpp
        )

add_executable(Test ${SOURCE_FILES} ${SOURCES}) # which files should produce executables

target_link_libraries(Test Src)

target_link_libraries(Test glfw)

target_link_libraries(Test GLEW)

find_package(OpenGL REQUIRED)

target_link_libraries(Test OpenGL::GL)
