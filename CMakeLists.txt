cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(depth_renderer)

SET(CMAKE_INSTALL_PREFIX /usr)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}")

find_package(PCL 1.7 REQUIRED COMPONENTS common io)
find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
#find_package(GLAD REQUIRED)
find_package(glfw3 3.2 REQUIRED)
#find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)
find_package(tinyobjloader REQUIRED)
#find_package(tinyobjloader REQUIRED)
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
#find_package(Matlab COMPONENTS MX_LIBRARY REQUIRED)

include_directories(
	include
	${PCL_INCLUDE_DIRS}
	${EIGEN3_INCLUDE_DIR} 
	${OPENGL_INCLUDE_DIRS} 
#	${GLEW_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
#	${GLAD_INCLUDE_DIRS}
#	${GLUT_INCLUDE_DIRS}
	${SOIL_INCLUDE_DIRS}
    ${TINYOBJLOADER_INCLUDE_DIRS}
#    ${Matlab_INCLUDE_DIRS}
)

link_directories(${PCL_LIBRARY_DIRS})

add_definitions(${PCL_DEFINITIONS})

set(LIBRARY_SOURCES

        include/amath.h
        include/GLController.h
        include/GLOptions.h
        include/GLParser.h
        include/GLScene.h
        include/GLShaderLoader.h
        include/GLUI.h
        include/main.h
        include/GL/gl3w.h
        include/GL/glcorearb.h

	src/GLController.cc  
	src/GLOptions.cc
	src/GLParser.cc  
	src/GLScene.cc  
	src/GLShaderLoader.cc  
	src/GLUI.cc
    src/gl3w.c
)

add_library(depth_renderer SHARED ${LIBRARY_SOURCES})

add_executable(mdepth_renderer src/main.cc)

target_link_libraries(depth_renderer
#		mat
	${PCL_COMMON_LIBRARIES} 
	${PCL_IO_LIBRARIES}  
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${OPENGL_LIBRARIES} 
#	${GLEW_LIBRARIES}
	${SOIL_LIBRARIES} 
#	${GLUT_LIBRARY}
	${GLFW_LIBRARY}
    ${TINYOBJLOADER_LIBRARIES}
#    ${Matlab_LIBRARIES}
)

target_link_libraries(
	mdepth_renderer
	depth_renderer
#		mat
	${PCL_COMMON_LIBRARIES} 
	${PCL_IO_LIBRARIES}  
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${OPENGL_LIBRARIES} 
#	${GLUT_LIBRARY}
	${GLFW_LIBRARY}
    ${TINYOBJLOADER_LIBRARY_DIRS}
#    ${Matlab_LIBRARIES}
	glfw
)

install(TARGETS mdepth_renderer depth_renderer
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY include/ DESTINATION include/depth_renderer
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY shaders/ DESTINATION include/depth_renderer
        FILES_MATCHING PATTERN "*.glsl")

install(TARGETS mdepth_renderer DESTINATION /usr/bin)
