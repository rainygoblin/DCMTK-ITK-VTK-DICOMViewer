#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>

class DICOMViewer
{
	DICOMViewer();
	vtkSmartPointer<vtkImageViewer2> _mImageViewer;
	vtkSmartPointer<vtkRenderWindowInteractor> _mRenderWinInteractor;

public:
	DICOMViewer(vtkAlgorithmOutput* vtk_algorithm_output);
	void RenderImage() const;
};