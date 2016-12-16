cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(depth_renderer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

find_package(PCL 1.7 REQUIRED COMPONENTS common io)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(SOIL REQUIRED)
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)

include_directories(
	include 
	${PCL_INCLUDE_DIRS}
	${EIGEN3_INCLUDE_DIR} 
	${OPENGL_INCLUDE_DIRS} 
	${GLEW_INCLUDE_DIRS} 
	${GLUT_INCLUDE_DIRS}
	${SOIL_INCLUDE_DIRS} )

link_directories(${PCL_LIBRARY_DIRS})

add_definitions(${PCL_DEFINITIONS})

set(LIBRARY_SOURCES
	src/GLController.cc  
	src/GLMatrix.cc  
	src/GLOptions.cc  
	src/GLParser.cc  
	src/GLScene.cc  
	src/GLShaderLoader.cc  
	src/GLTriangle.cc  
	src/GLUI.cc  
	src/GLVector.cc  
)

add_library(depth_renderer SHARED ${LIBRARY_SOURCES})

add_executable(mdepth_renderer src/main.cc)

target_link_libraries(depth_renderer 
	${PCL_COMMON_LIBRARIES} 
	${PCL_IO_LIBRARIES}  
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${OPENGL_LIBRARIES} 
	${GLEW_LIBRARIES} 
	${SOIL_LIBRARIES} 
	${GLUT_LIBRARY})

	target_link_libraries(mdepth_renderer 
	depth_renderer
	${PCL_COMMON_LIBRARIES} 
	${PCL_IO_LIBRARIES}  
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${OPENGL_LIBRARIES} 
	${GLUT_LIBRARY})

install(TARGETS mdepth_renderer depth_renderer
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY include/ DESTINATION include/depth_renderer
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY shaders/ DESTINATION include/depth_renderer
        FILES_MATCHING PATTERN "*.glsl")

install(TARGETS mdepth_renderer DESTINATION /usr/bin)