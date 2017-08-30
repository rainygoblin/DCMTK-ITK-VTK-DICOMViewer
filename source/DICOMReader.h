#pragma once

#include "itkImage.h"

#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"

class DICOMReader
{
	// vtk dicom reader
	vtkSmartPointer<vtkDICOMImageReader> _mDCMReader;

public:

	// constructor
	DICOMReader();

	// read dicom files from specified directory
	vtkAlgorithmOutput* Read(std::string dicomDir) const;
};