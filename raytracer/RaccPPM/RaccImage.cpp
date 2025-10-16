/*	NAME:
 *		RaccImage.cpp
 *
 *	DESCRIPTION:
 *		Default implementations for Racc Image class
 *
 *	CONTRIBUTORS:
 *		RACC 10-OCT-25
*/





// =============================================================================
//		Includes
// -----------------------------------------------------------------------------
#include "RaccImage.hpp"
#include <filesystem>





using namespace RACCPPM;





// =============================================================================
//		RaccImage : Save
// -----------------------------------------------------------------------------
bool RaccImage::Save() const
{
	return SaveFile(mFilename);
}





// =============================================================================
//		RaccImage : Save As
// -----------------------------------------------------------------------------
bool RaccImage::SaveAs(const std::string& filename, const std::string& fileExtension, bool overwrite) const
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
//		RaccImage : Set Width
// -----------------------------------------------------------------------------
void RaccImage::SetWidth(int width)
{
	// To do: Add bounds checking / handle narrowing?
	mWidth = width;
}





// =============================================================================
//		RaccImage : Set Height
// -----------------------------------------------------------------------------
void RaccImage::SetHeight(int height)
{
	// To do: Add bounds checking / handle narrowing?
	mHeight = height;
}






// =============================================================================
//		RaccImage : Get Pixel
// -----------------------------------------------------------------------------
RGBValue RaccImage::GetPixel(int x, int y) const
{
	// To do: Add bounds checking
	return mPixelValues[(y * mWidth) + x];
}





// =============================================================================
//		RaccImage : Set Pixel
// -----------------------------------------------------------------------------
void RaccImage::SetPixel(int x, int y, const RGBValue& value)
{
	// To do: Add bounds checking
	mPixelValues[(y * mWidth) + x] = value;
}





// =============================================================================
//		RaccImage : Pixel At
// -----------------------------------------------------------------------------
RGBValue& RaccImage::PixelAt(int x, int y) &
{
	return mPixelValues[(y * mWidth) + x];
}





