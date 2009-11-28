# Project description
project(spaint)
set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)

# Policies
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
      cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)
add_definitions(-D__VERSION="${VERSION_STRING}")
set(VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

# CMake build type: Release|Debug
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
   set(CMAKE_BUILD_TYPE     Release)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set(CMAKE_COLOR_MAKEFILE ON)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# Source files
set(project_src         main.cpp
                        canvas.cpp
                        canvasmgr.cpp
                        canvascontainment.cpp
                        networkservice.cpp
                        mainwindow.cpp
                        gesturehandler.cpp
                        mousegesturerecognizer.cpp
                        )

set(project_headers     canvas.h
                        canvasmgr.h
                        canvascontainment.h
                        networkservice.h
                        mainwindow.h
                        gesturehandler.h
                        mousegesturerecognizer.h
                        defines.h
                        )

# Qt4
set(QT_MIN_VERSION 4.6.0)
find_package(Qt4 REQUIRED)

qt4_automoc(${project_src})
qt4_add_resources(project_src       icons/icons.qrc)
#qt4_add_translation(project_i18n    i18n/${PROJECT_NAME}_cs.ts)


# Includes
include(${QT_USE_FILE})
include_directories( ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${QT_INCLUDES}
                     )

# Targets
add_executable(${PROJECT_NAME}        ${project_src}
                                      ${project_i18n}
                                      )
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}
                                      )

# Install targets
install( TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION bin
         )
install( FILES ${project_headers}
         DESTINATION include/${PROJECT_NAME}
         )

# Install - UNIX
if(${UNIX})
   # Install localization
   #install(FILES ${project_i18n} DESTINATION share/${PROJECT_NAME}/locale COMPONENT locales)
endif(${UNIX})

# Install - WIN32
if(${WIN32})
   install(FILES ${project_i18n} DESTINATION locale COMPONENT locales)
endif(${WIN32})

# Uninstall
CONFIGURE_FILE(
          "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
          "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
          IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
          "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

