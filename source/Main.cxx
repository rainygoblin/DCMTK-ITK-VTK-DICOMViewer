#include "itkMinimumMaximumImageFilter.h"
#include "itkDCMTKImageIO.h"
#include "itkDCMTKSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

#include "filesystem"

#include <dcmtk/dcmdata/dcfilefo.h>
#include "DICOMDecoder.h"
#include "DICOMReader.h"
#include "StatusMessage.h"
#include "DICOMViewer.h"

namespace fs = std::experimental::filesystem;
const char* _gDICOM_Dir;

// remove directory where compressed DICOM images were uncompressed
void CleanUp(const char* dir)
{
	// remove uncompress DICOM data directory
	char tempDir[1024];
	sprintf_s(tempDir, "%s\\temp", dir);
	
	// remove directory
	if (std::string(tempDir).length() == 0)
		fs::remove(tempDir);
}

int main(int argc, char **argv) {

	try
	{
		if(argc < 1)
			throw itk::ExceptionObject("Please input DICOM directory. How to use: DICOMViewer.exe <DICOM_SERIES_DIR>");

		// first argument is for DICOM series directory
		_gDICOM_Dir = argv[1];

		// check whether argv[1] is directory or not
		auto isDirectory = std::experimental::filesystem::is_directory(_gDICOM_Dir);

		// read dicom series from directory
		if(!isDirectory)
			throw itk::ExceptionObject("DICOM directory does not exists");

		// read dicom series from given folder		
		fs::directory_iterator end_itr;
		std::vector<std::string> fileNames;
		for (fs::directory_iterator itr(argv[1]); itr != end_itr; ++itr)
			fileNames.push_back((*itr).path().string().c_str());

		if(fileNames.size() <= 0)
			throw itk::ExceptionObject("DICOM files does not exists");

		// check if DICOM data is JPEG compressed or not
		auto isCompressed = false;
		auto isJPEGCompressed = false;
		DcmFileFormat fileformat;
		auto status = fileformat.loadFile(fileNames[0].c_str());
		if (status.good())
		{
			OFString compress;
			DcmTagKey compression_tag(0x0008, 0x2111);
			if (fileformat.getDataset()->findAndGetOFStringArray(compression_tag, compress).good())
			{
				// compression tag is available
				isCompressed = true;

				// check if it's JPEG compressed
				auto tmp = strstr(const_cast<char*>(compress.c_str()), "JPEG");
				if (tmp != nullptr) isJPEGCompressed = true;
			}
		}

		if(isCompressed && !isJPEGCompressed)
			throw itk::ExceptionObject("Only supports JPEG compressed data");

		// clear dicom file name vector
		fileNames.clear();

		// DICOM series directory
		char uncompressedDICOMSeriesDir[1024];
		sprintf_s(uncompressedDICOMSeriesDir, "%s", _gDICOM_Dir);

		char tempDir[1024];
		if (isCompressed && isJPEGCompressed)
		{	
			sprintf_s(tempDir, "%s\\temp", _gDICOM_Dir);

			// uncompress DICOM slices
			DICOMDecoder decoder;
			decoder.Decode(uncompressedDICOMSeriesDir, tempDir);
			
			// uncompressed DICOM data directory
			sprintf_s(uncompressedDICOMSeriesDir, "%s", tempDir);
		}

		auto reader = new DICOMReader();
		auto vtkOutput = reader->Read(uncompressedDICOMSeriesDir);

		if(vtkOutput == nullptr)
			throw itk::ExceptionObject("Error reading DICOM series");

		auto viewer = new DICOMViewer(vtkOutput);
		viewer->RenderImage();

		delete viewer;
		delete reader;

		CleanUp(_gDICOM_Dir);

		return EXIT_SUCCESS;

	}
	catch(itk::ExceptionObject & e)
	{
		cerr << e << endl;
		CleanUp(_gDICOM_Dir);
		return EXIT_FAILURE;
	}
}