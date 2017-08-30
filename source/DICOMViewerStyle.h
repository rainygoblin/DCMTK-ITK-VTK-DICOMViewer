#pragma once

#include "StatusMessage.h"

#include "vtkImageViewer2.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkTextMapper.h"
#include "vtkObjectFactory.h"

class DICOMViewerStyle : public vtkInteractorStyleImage
{
	DICOMViewerStyle();

public:
	static DICOMViewerStyle* New();
	vtkTypeMacro(DICOMViewerStyle, vtkInteractorStyleImage);

protected:
	vtkImageViewer2* _ImageViewer;
	vtkTextMapper* _StatusMapper;
	int _Slice;
	int _MinSlice;
	int _MaxSlice;

public:
	void SetImageViewer(vtkImageViewer2* imageViewer);
	void SetStatusMapper(vtkTextMapper* statusMapper);

protected:
	void MoveSliceForward();
	void MoveSliceBackward();
	void OnKeyDown() override;
	void OnMouseWheelForward() override;
	void OnMouseWheelBackward() override;
};

DICOMViewerStyle::DICOMViewerStyle() :
	_ImageViewer(nullptr),
	_StatusMapper(nullptr),
	_Slice(0),
	_MinSlice(0),
	_MaxSlice(0)
{ }

inline void DICOMViewerStyle::SetImageViewer(vtkImageViewer2* imageViewer) {
	_ImageViewer = imageViewer;
	_MinSlice = imageViewer->GetSliceMin();
	_MaxSlice = imageViewer->GetSliceMax();
	_Slice = _MinSlice;
	cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
}

inline void DICOMViewerStyle::SetStatusMapper(vtkTextMapper* statusMapper) {
	_StatusMapper = statusMapper;
}

inline void DICOMViewerStyle::MoveSliceForward() {
	if (_Slice < _MaxSlice) {
		_Slice += 1;
		cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
		_ImageViewer->SetSlice(_Slice);
		std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		_StatusMapper->SetInput(msg.c_str());
		_ImageViewer->Render();
	}
}

inline void DICOMViewerStyle::MoveSliceBackward() {
	if (_Slice > _MinSlice) {
		_Slice -= 1;
		cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
		_ImageViewer->SetSlice(_Slice);
		std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
		_StatusMapper->SetInput(msg.c_str());
		_ImageViewer->Render();
	}
}


inline void DICOMViewerStyle::OnKeyDown() {
	std::string key = this->GetInteractor()->GetKeySym();
	if (key.compare("Up") == 0) {
		//cout << "Up arrow key was pressed." << endl;
		MoveSliceForward();
	}
	else if (key.compare("Down") == 0) {
		//cout << "Down arrow key was pressed." << endl;
		MoveSliceBackward();
	}
	// forward event
	vtkInteractorStyleImage::OnKeyDown();
}


inline void DICOMViewerStyle::OnMouseWheelForward() {
	//std::cout << "Scrolled mouse wheel forward." << std::endl;
	MoveSliceForward();
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelForward();
}


inline void DICOMViewerStyle::OnMouseWheelBackward() {
	//std::cout << "Scrolled mouse wheel backward." << std::endl;
	if (_Slice > _MinSlice) {
		MoveSliceBackward();
	}
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelBackward();
}

vtkStandardNewMacro(DICOMViewerStyle);