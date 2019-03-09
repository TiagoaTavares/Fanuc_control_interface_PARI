#CMakeLists.txt
cmake_minimum_required (VERSION 2.8)
project (Projeto)
set(PROG Projeto)

#-- prepare and create library
set(SOURCES srcmyutils/pcolor.c)
set(SOURCES2 src/main.c src/callbacks.c src/myf.c)
add_library( myutils SHARED ${SOURCES})
#add_library( myutils ${SOURCES}) #uncomment and replace previous to create static library

#-- create a program that uses the newly create library----
include_directories ("${PROJECT_SOURCE_DIR}/srcmyutils")
add_executable (${PROG} src/main.c src/callbacks.c src/myf.c)
target_link_libraries (${PROG} myutils)

set (CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")   #default would be /usr/local
install(TARGETS ${PROG} myutils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
       )

#-- Target to set the libpath
#-- Without it path for library may be unreachable
#-- requires root previleges: make libpath
add_custom_target( libpath
        COMMAND sudo ldconfig "${PROJECT_SOURCE_DIR}/lib"
                )
#-----------------------------------------------
#Next line to force the compilation with C++ because of the openCV library which is in C++ after version 3 - podemos apagar se n tivermos com opencv
SET_SOURCE_FILES_PROPERTIES( ${SOURCES2} PROPERTIES LANGUAGE CXX )
set(GCC_COVERAGE_COMPILE_FLAGS "-Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

#-----------------------------------------------
#Cf. http://docs.opencv.org/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html
#Add an external library like OpenCV
#find_package( OpenCV 2.4.9.1 EXACT )  #for a very specifi version
find_package( OpenCV 3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


#The recommended way to find packages automatically using PkgConfig
#See more on http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries
find_package(PkgConfig REQUIRED)  #necessary for next command to work
pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

target_link_libraries(${PROG} ${GTK_LIBRARIES} ${OpenCV_LIBS} )

#-----------------------------------------------------
# --- add custom mtargets (and dependencies)
add_custom_target( cpfiles
                COMMAND cp "${PROJECT_SOURCE_DIR}/src/*.glade" "${PROJECT_BINARY_DIR}"
                COMMAND cp "${PROJECT_SOURCE_DIR}/src/*.png" "${PROJECT_BINARY_DIR}" || :
                 )

#Add dependencies
add_dependencies(${PROG} cpfiles)
#--------------------------------------------------------------------------------

#-------- -----------------------DOCUMENTATION------------
# The project version number. Used in documentation generation and similar
set(VERSION_MAJOR   1   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

#-- add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install doc (requires Doxygen)" ${DOXYGEN_FOUND})
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in" )
    set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    set(doxy_main_page "${CMAKE_CURRENT_SOURCE_DIR}/doc/mainpage.dox")
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()
#code inspired on: http://www.tty1.net/blog/2014/cmake-doxygen_en.html
#---------------------------------------------------
#---------------------------------------------------
