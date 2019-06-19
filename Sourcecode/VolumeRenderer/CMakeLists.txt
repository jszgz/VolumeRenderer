cmake_minimum_required(VERSION 2.8.11)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

set(VTK_DIR "C:/Users/chwangteng/Downloads/vtkqt8.0.1/vtk-v8.0.1-build" CACHE PATH "VTK directory")
set(QT_QMAKE_EXECUTABLE "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64/bin/qmake.exe" CACHE PATH "Qt qmake.exe")
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64" CACHE PATH "Qt directory")

# CTK paths
set(CTK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CTK" CACHE PATH "CTK directory")
set(CTK_Libs_Core_DIR "${CTK_DIR}/Libs/Core")
set(CTK_Libs_Widgets_DIR "${CTK_DIR}/Libs/Widgets")
set(CTK_Libs_Visualization_VTK_Core_DIR "${CTK_DIR}/Libs/Visualization/VTK/Core")
set(CTK_Libs_Visualization_VTK_Widgets_DIR "${CTK_DIR}/Libs/Visualization/VTK/Widgets")
file(GLOB CTK_header_files
    ${CTK_Libs_Core_DIR}/*.h
    ${CTK_Libs_Widgets_DIR}/*.h
    ${CTK_Libs_Visualization_VTK_Core_DIR}/*.h
    ${CTK_Libs_Visualization_VTK_Widgets_DIR}/*.h
)
file(GLOB CTK_source_files
    ${CTK_Libs_Core_DIR}/*.h
    ${CTK_Libs_Core_DIR}/*.cpp
    ${CTK_Libs_Widgets_DIR}/*.h
    ${CTK_Libs_Widgets_DIR}/*.cpp
    ${CTK_Libs_Visualization_VTK_Core_DIR}/*.h
    ${CTK_Libs_Visualization_VTK_Core_DIR}/*.cpp
    ${CTK_Libs_Visualization_VTK_Widgets_DIR}/*.h
    ${CTK_Libs_Visualization_VTK_Widgets_DIR}/*.cpp
)
file(GLOB CTK_qrc_files
    ${CTK_Libs_Widgets_DIR}/Resources/*.qrc
    ${CTK_Libs_Visualization_VTK_Widgets_DIR}/Resources/*.qrc
)
file(GLOB CTK_ui_files
    ${CTK_Libs_Widgets_DIR}/Resources/UI/*.ui
    ${CTK_Libs_Visualization_VTK_Widgets_DIR}/Resources/UI/*.ui
)

project(VolumeRenderer)

find_package(OpenGL REQUIRED)
message(STATUS "OPENGL_LIBRARIES=" ${OPENGL_LIBRARIES})

# VTK is required
find_package(VTK REQUIRED)
vtk_module_config(VTK
  vtkCommonCore
  vtkChartsCore
  vtkViewsContext2D
  vtkInteractionWidgets
  vtkGUISupportQt
  vtkRenderingContext${VTK_RENDERING_BACKEND}
  vtkRendering${VTK_RENDERING_BACKEND}
  vtkRenderingVolume${VTK_RENDERING_BACKEND}
)
include(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

link_directories("${VTK_DIR}/lib")
message(STATUS "VTK_USE_FILE=" ${VTK_USE_FILE})
message(STATUS "VTK_DEFINITIONS=" ${VTK_DEFINITIONS})
message(STATUS "VTK_INCLUDE_DIRS=" ${VTK_INCLUDE_DIRS})
message(STATUS "VTK_LIBRARIES=" ${VTK_LIBRARIES})
message(STATUS "link_directories: " "${VTK_DIR}/lib")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR=" "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR=" "${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "vtk-${VTK_VERSION} ${VTK_MAJOR_VERSION} ${VTK_MINOR_VERSION}")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
## Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Widgets_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# CTK
source_group(CTK FILES ${CTK_header_files} ${CTK_source_files})
message(STATUS "CTK header files: " ${CTK_header_files})
message(STATUS "CTK source files: " ${CTK_source_files})
QT5_WRAP_CPP(CTK_moc_source_files ${CTK_header_files})
message(STATUS "CTK moc source files: " ${CTK_moc_source_files})
source_group(CTK_moc FILES ${CTK_moc_source_files})
QT5_WRAP_UI(CTK_ui_header_files ${CTK_ui_files})
message(STATUS "CTK ui header files: " ${CTK_ui_header_files})
QT5_ADD_RESOURCES(CTK_qrc_complied ${CTK_qrc_files})
source_group(CTK_ui FILES ${CTK_ui_header_files} ${CTK_ui_files} ${CTK_qrc_files} ${CTK_qrc_complied})

# Slicer
set(Slicer_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Slicer/Modules/Loadable/VolumeRendering/VolumeRenderingReplacements)
file(GLOB Slicer_files
    ${Slicer_DIR}/*.h
  ${Slicer_DIR}/*.cxx
)
source_group(Slicer FILES ${Slicer_files})

# Qt and project source code
file(GLOB Project_source_files
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB Project_header_files
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
QT5_WRAP_CPP(Project_moc_source_files ${Project_header_files})
file(GLOB Project_ui_files
  ${CMAKE_CURRENT_SOURCE_DIR}/*.ui
)
QT5_WRAP_UI(Project_ui_header_files ${Project_ui_files})
source_group(ui FILES ${Project_ui_files} ${Project_ui_header_files})
source_group(moc FILES ${Project_moc_source_files})
#SET(Project_qrc_files YourProject.qrc)
#QT5_ADD_RESOURCES(Project_qrc_compiled ${Project_qrc_files})

# Add the include directories for VTK and Qt 5 Widgets module to the compile lines.
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Widgets_INCLUDE_DIRS}
	${VTK_INCLUDE_DIRS} "${VTK_DIR}/include/vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}"
	${CTK_Libs_Core_DIR} ${CTK_Libs_Widgets_DIR} ${CTK_Libs_Visualization_VTK_Core_DIR} ${CTK_Libs_Visualization_VTK_Widgets_DIR}
	${Slicer_DIR}
)

add_executable(${PROJECT_NAME} ${Project_source_files} ${Project_ui_header_files}
	${Project_header_files} ${Project_moc_source_files}
	#${Project_qrc_compiled}
	${CTK_header_files} ${CTK_source_files} ${CTK_moc_source_files} ${CTK_ui_header_files} ${CTK_qrc_complied}
	${Slicer_files}
)

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::OpenGL ${VTK_LIBRARIES} ${OPENGL_LIBRARIES})

if(WIN32) # Check if we are on Windows
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		COMMAND xcopy /d /f /y "\"${Qt5Widgets_DIR}/../../../bin/*.dll\"" "\"$<TARGET_FILE_DIR:${PROJECT_NAME}>\""
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND xcopy /d /f /y "\"${VTK_DIR}/bin/$<CONFIGURATION>/*.dll\"" "\"$<TARGET_FILE_DIR:${PROJECT_NAME}>\""
	)
endif()
