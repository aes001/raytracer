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





using namespace RaccPixels;





// =============================================================================
//		RaccImage : Save As
// -----------------------------------------------------------------------------
bool PixelsBuffer::Save(const std::string& filename, ImageType imageType, bool overwrite /*= false*/) const
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
void PixelsBuffer::SetWidth(int width)
{
	// To do: Add bounds checking / handle narrowing?
	mWidth = width;
}





// =============================================================================
//		RaccImage : Set Height
// -----------------------------------------------------------------------------
void PixelsBuffer::SetHeight(int height)
{
	// To do: Add bounds checking / handle narrowing?
	mHeight = height;
}






// =============================================================================
//		RaccImage : Get Pixel
// -----------------------------------------------------------------------------
RGBValue PixelsBuffer::GetPixel(int x, int y) const
{
	// To do: Add bounds checking
	return mPixelValues[(y * mWidth) + x];
}





// =============================================================================
//		RaccImage : Set Pixel
// -----------------------------------------------------------------------------
void PixelsBuffer::SetPixel(int x, int y, const RGBValue& value)
{
	// To do: Add bounds checking
	mPixelValues[(y * mWidth) + x] = value;
}





// =============================================================================
//		RaccImage : Pixel At
// -----------------------------------------------------------------------------
RGBValue& PixelsBuffer::PixelAt(int x, int y) &
{
	return mPixelValues[(y * mWidth) + x];
}





