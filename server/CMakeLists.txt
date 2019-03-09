#My CMakeLists.txt
cmake_minimum_required (VERSION 2.8)
project (Projeto)
#
set(PROG server)

set(SOURCESSERV src/server.c src/auxfuncs.c)
set(SOURCES srcmyutils/pcolor.c)


#-- prepare and create library

add_library( myutils SHARED ${SOURCES})
#add_library( myutils ${SOURCES}) #uncomment and replace previous to create static library


#-- create a program that uses the newly create library----
include_directories ("${PROJECT_SOURCE_DIR}/srcmyutils")
add_executable(${PROG} ${SOURCESSERV})

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



#-------------------------------------

#Next lines generate the prototypes but it is not automatic.
#You must force a make prototypes.h to generate them
#it is better this way to avoid unnecessary recompilations.
#If you got an error during compilation and if it is likely due to a
#bad or missing prototype, just redo a make prototypes.h
#This follows the commom use of defining the prototypes manually.

add_custom_target( servproto
        DEPENDS ${SOURCESSERV}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMAND echo "Rebuild prototypes.h"
        COMMAND echo '/*File generated automatically. Do not edit*/'  > "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo '/*Generated on' `hostname` ',' `date` '*/'      >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo '\#ifdef __cplusplus'                            >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo 'extern \"C\" {'                                 >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo '\#endif'                                        >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND cproto -q -O /dev/null ${SOURCESSERV}                     >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo '\#ifdef __cplusplus'                            >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo '}'                                              >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
        COMMAND echo '\#endif'                                        >> "${PROJECT_SOURCE_DIR}/src/server/prototypes.h"
                 )

add_custom_target( clientproto
        DEPENDS ${SOURCESCLI}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMAND echo "Rebuild prototypes.h"
        COMMAND echo '/*File generated automatically. Do not edit*/'  > "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo '/*Generated on' `hostname` ',' `date` '*/'      >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo '\#ifdef __cplusplus'                            >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo 'extern \"C\" {'                                 >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo '\#endif'                                        >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND cproto -q -O /dev/null ${SOURCESCLI}                    >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo '\#ifdef __cplusplus'                            >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo '}'                                              >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        COMMAND echo '\#endif'                                        >> "${PROJECT_SOURCE_DIR}/src/client/prototypes.h"
        )

add_custom_target(prototypes.h
        DEPENDS servproto clientproto
        )
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
