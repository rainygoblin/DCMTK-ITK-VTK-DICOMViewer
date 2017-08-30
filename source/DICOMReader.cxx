#include "DICOMReader.h"

// constructor
DICOMReader::DICOMReader()
{
	// initialize dicom image reader
	_mDCMReader = vtkSmartPointer<vtkDICOMImageReader>::New();
}

// read dicom files from specified directory
vtkAlgorithmOutput* DICOMReader::Read(std::string dicomDir) const
{
	try
	{
		// 1. Read all the DICOM files in the specified directory
		_mDCMReader->SetDirectoryName(dicomDir.c_str());
		_mDCMReader->Update();

		// return vtkAlgorithmOutput
		return _mDCMReader->GetOutputPort();
	}
	catch (itk::ExceptionObject & e)
	{
		std::cerr << e << std::endl;
		return nullptr;
	}
}
