# VolumeRenderer

## Introduction
Volume Renderer is a volume rendering program based on VTK, CTK and Qt.

**File support**
- slc formate

**Renderer Support( Hot switch ）**
- vtkSlicerGPURayCastVolumeMapper( from Slicer https://www.slicer.org/ )
- vtkGPUVolumeRayCastMapper
- vtkSmartVolumeMapper (Default. Recommended. Microsoft Remote Desktop can downgrade OpenGL to 1.1 and it can switch to CPU)

**Transfer Function Interaction**
- Drag a point
- Delete a point
- Add a point
- Change RGB color of a demarcation point
- Digital setting mode supported

**Framerate Display**
- When the FPS is greater than 60 ,the color is black; otherwise red.

**Others**

Advanced interpolation, shade and material settings.

## Installation( Optional )
| Item        | Version   |  Remark   |
| --------   | -----:  | :----:  |
| VTK      | 8.0.1   |        |
| Qt        |   5.12.3   |   MSVC 2017   |
| CMake        |   3.14.4   |   GUI   |

**CMake settings for compiling VTK **  
```shell
set(VTK_DIR "C:/Users/chwangteng/Downloads/vtkqt8.0.1/vtk-v8.0.1-build" CACHE PATH "VTK directory") 
set(QT_QMAKE_EXECUTABLE "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64/bin/qmake.exe" CACHE PATH "Qt qmake.exe") 
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64" CACHE PATH "Qt directory")
set(VTK_QT_VERSION 5 CACHE STRING "Qt version") 
set(VTK_NO_LIBRARY_VERSION ON CACHE BOOL "VTK_NO_LIBRARY_VERSION") 
set(VTK_Group_Qt ON CACHE BOOL "VTK_Group_Qt") 
set(Module_vtkGUISupportQt ON CACHE BOOL "Module_vtkGUISupportQt") 
set(Module_vtkGUISupportQtOpenGL ON CACHE BOOL "Module_vtkGUISupportQtOpenGL") 
set(Module_vtkRenderingQt ON CACHE BOOL "Module_vtkRenderingQt") 
set(BUILD_TESTING OFF CACHE BOOL "BUILD_TESTING") 
set(VTK_RENDERING_BACKEND_DEFAULT "OpenGL")
```
**CMake settings for compiling the project **  
volume-renderer/VolumeRenderer/CMakeLists.txt  
```shell
set(VTK_DIR "C:/Users/chwangteng/Downloads/vtkqt8.0.1/vtk-v8.0.1-build" CACHE PATH "VTK directory")
set(QT_QMAKE_EXECUTABLE "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64/bin/qmake.exe" CACHE PATH "Qt qmake.exe")
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64" CACHE PATH "Qt directory")
```
## RUN
1. Open executable/VolumeRenderer/VolumeRenderer.exe in Windows( 64bit ).
2. Click "Open" and select a slc file( vm_foot.slc).
3. Have fun.

## TODO
- Bug fix
- Performance improvement
## Screenshots
![图片无法加载](https://raw.githubusercontent.com/chwangteng/VolumeRenderer/master/Demo/Screenshots/1.png) 
![图片无法加载](https://raw.githubusercontent.com/chwangteng/VolumeRenderer/master/Demo/Screenshots/2.png) 
## References
[VTK安装 win10+VS2017+VTK8.1.2+QT5.12编译安装](https://blog.csdn.net/qq_33766108/article/details/84431032)  
[VTK8.1 在 Qt5.9 环境下的配置、编译和安装](https://blog.csdn.net/jepco1/article/details/80628026#commentsedit)  
[VTK/Examples/Cxx/VolumeRendering/SmartVolumeMapper](https://vtk.org/Wiki/VTK/Examples/Cxx/VolumeRendering/SmartVolumeMapper)  
[VTK/Examples/Cxx/Utilities/FrameRate](https://vtk.org/Wiki/VTK/Examples/Cxx/Utilities/FrameRate)  
[vtkCommand Class Reference](https://vtk.org/doc/nightly/html/classvtkCommand.html)  
[QVTKWidget Class Reference](https://vtk.org/doc/nightly/html/classQVTKWidget.html)  
[ctkTransferFunctionWidget](http://www.commontk.org/index.php/Documentation/ctkTransferFunctionWidget)  
[vtkVolumeProperty Class Reference](https://vtk.org/doc/nightly/html/classvtkVolumeProperty.html)  
[volume-renderer](https://bitbucket.org/lsz/volume-renderer/src/default/)  
