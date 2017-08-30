#include "DICOMDecoder.h"

#include "itkImage.h"
#include "itkExceptionObject.h"

#include<filesystem>
#include <iostream>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmjpeg/djdecode.h>

namespace fs = std::experimental::filesystem;

bool DICOMDecoder::Decode(const char* inputDir, const char* outputDir) const
{
	try
	{
		// check of input directory
		if(!fs::exists(inputDir))
			throw itk::ExceptionObject("Input dicom series directory does not exists.");

		// create output directory if does not exists
		if (!fs::exists(outputDir))
			fs::create_directory(outputDir);

		// list of DICOM files -- input directory
		std::vector<std::string> dicomFileNames;
		fs::directory_iterator end_iter; // default construction yields past-the-end
		for (fs::directory_iterator iter(inputDir); iter != end_iter; ++iter)
			dicomFileNames.push_back((*iter).path().filename().string());

		// decompress DICOM images and store into output directory
		// register JPEG codecs
		DJDecoderRegistration::registerCodecs();
		for (auto filename : dicomFileNames)
		{
			char source[1024], destination[1024];
			sprintf_s(source, "%s\\%s", inputDir, filename.c_str());
			sprintf_s(destination, "%s\\%s", outputDir, filename.c_str());

			Uncompress(source, destination);
		}

		// return true 
		return true;
	}
	catch (itk::ExceptionObject & e)
	{
		std::cerr << e << std::endl;
		return false;
	}
}

bool DICOMDecoder::Uncompress(const char* inputDicomPath, const char* outputDicomPath)
{
	DcmFileFormat fileformat;
	if (fileformat.loadFile(inputDicomPath).good())
	{
		auto *dataset = fileformat.getDataset();

		// decompress data set if compressed
		dataset->chooseRepresentation(EXS_LittleEndianExplicit, nullptr);

		// check if everything went well
		if (dataset->canWriteXfer(EXS_LittleEndianExplicit))
			fileformat.saveFile(outputDicomPath, EXS_LittleEndianExplicit);
		else return false;
	}
	else return false;

	return true;
}