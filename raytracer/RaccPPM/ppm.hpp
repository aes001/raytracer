/*	NAME:
 *		ppm.hpp
 *
 *	DESCRIPTION:
 *		Class to create and open PPM images
 *
 *	CONTRIBUTORS:
 *		RACC 09-AUG-25
*/





#ifndef PPM_H
#define PPM_H




// To DO: Refactor to inherit from RaccImage




// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <optional>





namespace RACCPPM
{





	struct RGBValue
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;

		std::string Red()   const { return std::to_string(R); }
		std::string Green() const { return std::to_string(G); }
		std::string Blue()  const { return std::to_string(B); }
	};





	class PPMImage
	{
	public:
		friend class PPMMaker;
		using PixelsArray = std::vector<RGBValue>;

		bool								Save() const; // Beware! Will overwrite the file!
		bool								SaveAs(const std::string& filename,
												   const std::string& fileExtension = ".ppm",
												   bool overwrite = false) const;

		int									GetWidth() const { return mWidth; }
		void								SetWidth(int width);

		int									GetHeight() const { return mHeight; }
		void								SetHeight(int height);

		const std::string&					GetFileName() const& { return mFilename; }
		std::string							GetFileName() && { return std::move(mFilename); }


		RGBValue							GetPixel(int x, int y) const;
		void								SetPixel(int x, int y, const RGBValue& value);

		// Direct access to set pixels
		RGBValue&							PixelAt(int x, int y) &;
		RGBValue							PixelAt(int x, int y) && = delete;

		// Direct access to the pixels array to set them in bulk
		PixelsArray&						Pixels() & { return mPixelValues; }
		PixelsArray							Pixels() && { return std::move(mPixelValues); } // Prevent stupidity



	protected:
		explicit							PPMImage(const std::string& filename,
													 int                width,
													 int                height);

		explicit							PPMImage(const std::string& filename,
													 int                width,
													 int                height,
													 const PixelsArray& pixels);

		bool								SaveFile(const std::string& filename) const;


	private:
		std::string							mFilename;
		int									mWidth;
		int									mHeight;
		PixelsArray							mPixelValues;

	};





	class PPMMaker
	{
	public:
		static PPMImage						NewPPMImage(const std::string& filename,
														int                width,
														int                height);

		static PPMImage						NewPPMImage(const std::string& filename,
														int				   width,
														double			   aspectRatio);


		static std::optional<PPMImage>		OpenPPMImage(const std::string& filename);
	};





}





#endif // PPM_H





