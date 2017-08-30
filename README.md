# DCMTK-ITK-VTK-DICOMViewer

Used ITK and VTK opensource library to build dicom viewer. 
Following are the steps to follow to compile and run the DICOMViewer project.

# Important Note:
1. Use x64 configuration to build DCMTK, ITK and VTK library. 
- libraries compiled with x86 configuration won't let you load large DICOM datasets.
2. Build DCMTK, VTK, and ITK in order. DO NOT build ITK before VTK.  

# Build DCMTK3.6.2
1. Download dcmtk-3.6.2.zip (http://dicom.offis.de/download/dcmtk/dcmtk362/)
2. Download supported libraries (http://dicom.offis.de/download/dcmtk/dcmtk362/support/)
3. Unzip dcmtk-3.6.2.zip into 'C:\dcmtk-3.6.2\source' directory
4. Unzip supported libraries into 'C:\dcmtk-3.6.2' directory
5. Create new directory for build 'C:\dcmtk-3.6.2\build'
6. Use CMake to build DCMTK3.6.2 
- Use 'C:\dcmtk-3.6.2\source' and 'C:\dcmtk-3.6.2\build' directory for source and binaries respectively
7. CMake automatically find supported libraries otherwise enter path for required supported libraries
8. Open DCMTK.sln
9. Build the solution using x64-debug configuration

# Build VTK-8.0.0
1. Download VTK-8.0.0.zip (http://www.vtk.org/download/)
2. Unzip VTK-8.0.0.zip into 'C:\VTK-8.0.0\source' directory
3. Create new directory for build 'C:\VTK-8.0.0\build'
4. Use CMake to build VTK-8.0.0 
- Use 'C:\VTK-8.0.0\source' and 'C:\VTK-8.0.0\build' directory for source and binaries respectively
5. Open VTK.sln
6. Build the solution using x64-debug configuration

# Build ITK-4.12.1
1. Download InsightToolkit-4.12.1.zip (https://itk.org/ITK/resources/software.html)
2. Unzip InsightToolkit-4.12.1.zip into 'C:\ITK-4.12.1\source' directory
3. Create new directory for build 'C:\ITK-4.12.1\build'
4. Use CMake to buildITK-4.12.1 
- Use 'C:\ITK-4.12.1\source' and 'C:\ITK-4.12.1\build' directory for source and binaries respectively
- NOTE: Do not forget to select Module_ITKVtkGlue and other necessary modules 
5. Open ITK.sln
6. Build the solution using x64-debug configuration

# Build DICOMViewer
1. Download source (https://github.com/tgajera1016/ITK-VTK-DICOMViewer.git)
2. Create new directory for build
3. Use CMake to build DICOMViewer
4. Open DICOMViewer.sln and build it using x64-debug configuration

# Run DICOMViewer
1. Open command prompt 
2. Navigate to bin directory where DICOMViewer.exe is located
3. > DICOMViewer.exe "path to dicom series folder"

