#include "DICOMViewer.h"

#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkRenderer.h>
#include <vtkActor2D.h>

#include "StatusMessage.h"
#include "DICOMViewerStyle.h"

DICOMViewer::DICOMViewer()
	:_mImageViewer(nullptr), _mRenderWinInteractor(nullptr)
{

}

DICOMViewer::DICOMViewer(vtkAlgorithmOutput* vtk_algorithm_output)
{
	_mImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	_mImageViewer->SetInputConnection(vtk_algorithm_output);

	// slice status message
	auto sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(20);
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	auto sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	auto msg = StatusMessage::Format(_mImageViewer->GetSliceMin(), _mImageViewer->GetSliceMax());
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	auto sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
	sliceTextActor->SetPosition(15, 10);

	// usage hint message
	auto usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	auto usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n  mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	auto usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

	// create an interactor with our own style (inherit from vtkInteractorStyleImage)
	// in order to catch mousewheel and key events
	_mRenderWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	auto myInteractorStyle = vtkSmartPointer<DICOMViewerStyle>::New();

	// make imageviewer2 and sliceTextMapper visible to our interactorstyle
	// to enable slice status message updates when scrolling through the slices
	myInteractorStyle->SetImageViewer(_mImageViewer);
	myInteractorStyle->SetStatusMapper(sliceTextMapper);

	_mImageViewer->SetupInteractor(_mRenderWinInteractor);
	// make the interactor use our own interactorstyle
	// cause SetupInteractor() is defining it's own default interatorstyle 
	// this must be called after SetupInteractor()
	_mRenderWinInteractor->SetInteractorStyle(myInteractorStyle);
	// add slice status message and usage hint message to the renderer
	_mImageViewer->GetRenderer()->AddActor2D(sliceTextActor);
	_mImageViewer->GetRenderer()->AddActor2D(usageTextActor);
}

void DICOMViewer::RenderImage() const
{
	// initialize rendering and interaction
	//imageViewer->GetRenderWindow()->SetSize(400, 300);
	//imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
	_mImageViewer->Render();
	_mImageViewer->GetRenderer()->ResetCamera();
	_mImageViewer->Render();
	_mRenderWinInteractor->Start();
}
