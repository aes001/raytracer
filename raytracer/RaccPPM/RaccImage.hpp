/*	NAME:
 *		RaccImage.hpp
 *
 *	DESCRIPTION:
 *		Raw pixel buffer class that supports exporting to image files
 *
 *	CONTRIBUTORS:
 *		RACC 10-OCT-25
*/





#ifndef RACC_IMAGE_HPP
#define RACC_IMAGE_HPP





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <array>





namespace RaccPixels
{





	enum ImageType
	{
		kPNG,
		kBMP,
		kTGA,
		kJPG,
		kHDR
	};





	enum Channels
	{
		kY = 1, // Grayscale image
		kYA,    // Grayscale image with alpha
		kRGB,
		kRGBA
	};





	struct RGBValue
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;

		std::string Red()   const { return std::to_string(R); }
		std::string Green() const { return std::to_string(G); }
		std::string Blue()  const { return std::to_string(B); }
		std::string Alpha() const { return std::to_string(A); }
	};





	template <Channels channels>
	struct PixelValue
	{
		double mPixels[channels];
		static constexpr int kChannelsCount = channels;
	};





	template <Channels channels>
	class PixelsBuffer
	{
	public:
		PixelsBuffer(int width, int height);

		~PixelsBuffer();


		PixelsBuffer(const PixelsBuffer&);
		PixelsBuffer& operator=(const PixelsBuffer&);


		PixelsBuffer(PixelsBuffer &&) noexcept;
		PixelsBuffer& operator=(PixelsBuffer &&) noexcept;


		bool Save(const std::string& filename,
				  ImageType          imageType,
				  bool               overwrite = false) const;


		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }


		const PixelValue<channels>& GetPixel(int x, int y) const;

		void SetPixel(int x, int y, PixelValue<channels> value);

		// Direct access to set pixels
		PixelValue<channels>& PixelAt(int x, int y);

		// Direct access to the pixels array to set them in bulk
		PixelValue<channels>* Pixels() { return mPixelValues; }


	private:
		bool SaveFile(const std::string& filename) const;

		// These should not be immutable
		// We are setting them as non const to maintain copy-ablity
		int mWidth;
		int mHeight;

	};





}





#endif // RACC_IMAGE_HPP





