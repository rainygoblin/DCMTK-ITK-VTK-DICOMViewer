#pragma once

class DICOMDecoder
{
	static bool Uncompress(const char* inputDicomPath, const char* outputDicomPath);

	public:
		bool Decode(const char* inputDir, const char* outputDir) const;
};
