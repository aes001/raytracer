/*	NAME:
 *		RaccImage.hpp
 *
 *	DESCRIPTION:
 *		Racc Image Abstract Class
 *		Also this is the first file where we are getting rid of the silly 10
 *		tabs rule for function names. Newer files should follow suit. We
 *		might also switch to spaces instead of tabs. This comment should be
 *		a git commit message instead.
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





namespace RACCPPM
{





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




	class RaccImage
	{
	public:
		using PixelsArray = std::vector<RGBValue>;

		RaccImage(const std::string& fileName,
				  int                width,
				  int                height);

		RaccImage(const std::string& fileName,
				  int                width,
				  int                height,
				  const PixelsArray& pixelsArray);



		// Might overwrite a file with the same name
		virtual bool Save() const;
		virtual bool SaveAs(const std::string& filename,
							const std::string& fileExtension,
							bool overwrite = false) const;


		int GetWidth() const { return mWidth; }
		void SetWidth(int width);

		int GetHeight() const { return mHeight; }
		void SetHeight(int height);

		const std::string& GetFileName() const& { return mFilename; }
		std::string GetFileName() && { return std::move(mFilename); }

		virtual RGBValue GetPixel(int x, int y) const;
		virtual void SetPixel(int x, int y, const RGBValue& value);

		// Direct access to set pixels
		virtual RGBValue& PixelAt(int x, int y) &;
		RGBValue PixelAt(int x, int y) && = delete;

		// Direct access to the pixels array to set them in bulk
		virtual const PixelsArray& Pixels() & { return mPixelValues; }
		// Prevent stupidity
		virtual PixelsArray Pixels() && { return std::move(mPixelValues); }



	protected:
		virtual bool SaveFile(const std::string& filename) const = 0;


		std::string mFilename;
		int mWidth;
		int mHeight;
		PixelsArray mPixelValues;
	};





}





#endif // RACC_IMAGE_HPP





