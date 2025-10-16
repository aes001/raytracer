/*	NAME:
 *		ppm.cpp
 *
 *	DESCRIPTION:
 *		Implementations for ppm.hpp
 *
 *	CONTRIBUTORS:
 *		RACC 09-AUG-25
*/





// =============================================================================
//		Includes
// -----------------------------------------------------------------------------
#include "ppm.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>





using namespace RACCPPM;





// =============================================================================
//		PPMImage : Constructor
// -----------------------------------------------------------------------------
PPMImage::PPMImage(const std::string& filename, int width, int height)
	: mFilename(filename)
	, mWidth(width)
	, mHeight(height)
	, mPixelValues(std::vector<RGBValue>(width * height))
{
}





// =============================================================================
//		PPMImage : Constructor
// -----------------------------------------------------------------------------
PPMImage::PPMImage(const std::string& filename, int width, int height, const PixelsArray& pixels)
	: mFilename(filename)
	, mWidth(width)
	, mHeight(height)
	, mPixelValues(pixels)
{
}





// =============================================================================
//		PPMImage : Save
// -----------------------------------------------------------------------------
bool PPMImage::Save() const
{
	return SaveFile(mFilename + ".ppm");
}





// =============================================================================
//		PPMImage : Save As
// -----------------------------------------------------------------------------
bool PPMImage::SaveAs(const std::string& filename, const std::string& fileExtension, bool overwrite) const
{
	namespace fs = std::filesystem;

	std::string savename = filename;
	int filenamePostfix = 1;
	while (!overwrite && fs::exists(savename + fileExtension))
	{
		savename = filename + "-" + std::to_string(filenamePostfix);
		filenamePostfix++;
	}

	savename = savename + fileExtension;

	return SaveFile(savename);
}





// =============================================================================
//		PPMImage : Set Width
// -----------------------------------------------------------------------------
void PPMImage::SetWidth(int width)
{
	// To do: Add bounds checking / handle narrowing?
	mWidth = width;
}





// =============================================================================
//		PPMImage : Set Height
// -----------------------------------------------------------------------------
void PPMImage::SetHeight(int height)
{
	// To do: Add bounds checking / handle narrowing?
	mHeight = height;
}





// =============================================================================
//		PPMImage : Get Pixel
// -----------------------------------------------------------------------------
RGBValue PPMImage::GetPixel(int x, int y) const
{
	// To do: Add bounds checking
	return mPixelValues[(y * mWidth) + x];
}





// =============================================================================
//		PPMImage : Set Pixel
// -----------------------------------------------------------------------------
void PPMImage::SetPixel(int x, int y, const RGBValue& value)
{
	// To do: Add bounds checking
	mPixelValues[(y * mWidth) + x] = value;
}





// =============================================================================
//		PPMImage : Pixel At
// -----------------------------------------------------------------------------
RGBValue& PPMImage::PixelAt(int x, int y) &
{
	return mPixelValues[(y * mWidth) + x];
}




// =============================================================================
//		PPMImage : Save File
// -----------------------------------------------------------------------------
bool PPMImage::SaveFile(const std::string& filename) const
{
	bool isSaved = false;

	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::binary);

	if (file.is_open())
	{
		file << "P3\n";
		file << mWidth << " " << mHeight << "\n";
		file << "255\n";

		for (auto& data : mPixelValues)
		{
			file << data.Red() << " " << data.Green() << " " << data.Blue() << "\n";
		}

		isSaved = true;
	}

	return isSaved;
}





// =============================================================================
//		PPMMaker : New PPM Image
// -----------------------------------------------------------------------------
//		Create a ppm image from width and height
// -----------------------------------------------------------------------------
PPMImage PPMMaker::NewPPMImage(const std::string& filename, int width, int height)
{
	return PPMImage(filename, width, height);
}





// =============================================================================
//		PPMMaker : New PPM Image
// -----------------------------------------------------------------------------
//		Create a ppm image from width and aspectRatio
// -----------------------------------------------------------------------------
PPMImage PPMMaker::NewPPMImage(const std::string& filename, int width, double aspectRatio)
{
	int imageHeight = width/aspectRatio;
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	return PPMImage(filename, width, imageHeight);
}





// =============================================================================
//		PPMMaker : Open PPM Image
// -----------------------------------------------------------------------------
std::optional<PPMImage> PPMMaker::OpenPPMImage(const std::string& filename)
{
	// To do
	return std::nullopt;
}





