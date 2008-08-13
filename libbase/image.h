// Image.h: image data class for Gnash.
// 
//   Copyright (C) 2006, 2007, 2008 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// Based on the public domain work of Thatcher Ulrich <tu@tulrich.com> 2002

#ifndef GNASH_IMAGE_H
#define GNASH_IMAGE_H

#include "dsodefs.h"
#include "gnash.h" // for image file types
#include <boost/cstdint.hpp>

#include <boost/scoped_array.hpp>
#include <memory> // for auto_ptr
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace gnash {
    class IOChannel;
    class JpegImageInput;
}

/// Handy image utilities for RGB surfaces.
namespace gnash
{
namespace image
{

enum ImageType
{
	GNASH_IMAGE_INVALID,
	GNASH_IMAGE_RGB,
	GNASH_IMAGE_RGBA,
	GNASH_IMAGE_ALPHA
};

	/// Base class for different types of images
	class DSOEXPORT image_base
	{
	public:

		image_base(const image_base& o)
			:
			_type(o._type),
			m_size(o.size()),
			m_width(o.width()),
			m_height(o.height()),
			m_pitch(o.m_pitch),
			m_data(new boost::uint8_t[m_size])
		{
			update(o);
		}
			
		image_base(boost::uint8_t *data, int width, int height, int pitch, ImageType type);

		/// Construct an image_base allocating data for height*pitch bytes
		image_base(int width, int height, int pitch, ImageType type);

        ImageType type() const { return _type; }

		/// Return size of this image buffer, in bytes
		size_t size() const { return m_size; }

		/// Return size in bytes of a row of this image 
		size_t pitch() const { return m_pitch; }

		/// Return size in bytes of a single pixel value
		size_t pixelSize() const
		{
			return m_pitch / m_width;
		}

		/// Return width of image in pixels
		size_t width() const { return m_width; }

		/// Return height of image in pixels
		size_t height() const { return m_height; }

		/// Copy image data from a buffer.
		//
		/// Note that this buffer MUST have the same m_pitch, or unexpected things
		/// will happen. In general, it is only safe to copy from another image_base
		/// (or derivative thereof) or unexpected things will happen. 
		///
		/// @param data buffer to copy data from.
		///
		void update(boost::uint8_t* data);

		/// Copy image data from another image data
		//
		/// Note that this buffer MUST have the same m_pitch and _type
		/// or an assertion will fail.
		///
		/// @param from image to copy data from.
		///
		void update(const image_base& from);
		
		void clear(const boost::uint8_t byteValue = 0);

		/// Return a pointer to the underlying data
		virtual boost::uint8_t* data() { return m_data.get(); }

		/// Return a pointer to first byte of given line
		DSOEXPORT boost::uint8_t* scanline(size_t y);

		virtual ~image_base() {}


	protected:

		const ImageType _type;

		/// Size of image buffer in bytes.
		const size_t m_size;

		/// Width of image, in pixels
		const size_t m_width;

		/// Height of image, in pixels
		const size_t m_height;

		/// Byte offset from one row to the next
		//
		/// This is basically width in bytes of each line.
		/// For example, in an alpha image type this is equal to m_width
		/// while for an RGB this is 3 times the m_width.
		///
		const size_t m_pitch;

		/// Data bytes, geometry defined by members below
		boost::scoped_array<boost::uint8_t> m_data;

	private:

	};

	/// 24-bit RGB image.  Packed data, red byte first (RGBRGB...)
	class DSOEXPORT rgb : public image_base
	{

	public:

		rgb(int width, int height);

		rgb(const rgb& o)
			:
			image_base(o)
		{}

		rgb(boost::uint8_t* data, int width, int height, int stride)
			: image_base(data, width, height, stride, GNASH_IMAGE_RGB)
		{}

		~rgb();

	};

	/// 32-bit RGBA image.  Packed data, red byte first (RGBARGBA...)
	class DSOEXPORT rgba : public image_base
	{

	public:

		rgba(int width, int height);

		rgba(const rgba& o)
			:
			image_base(o)
		{}

		~rgba();

		/// Set pixel value 
		//
		/// TODO: move in base class ?
		///
		void set_pixel(size_t x, size_t y, boost::uint8_t r, boost::uint8_t g, boost::uint8_t b, boost::uint8_t a);

        void mergeAlpha(const boost::uint8_t* alphaData, const size_t bufferLength);

	};

	/// 8-bit alpha image.
	class DSOEXPORT alpha : public image_base
	{
	public:
		alpha(int width, int height);

		alpha(const alpha& o)
			:
			image_base(o)
		{}

		~alpha();

		/// Set pixel value 
		//
		/// TODO: move in base class ?
		///
		void	set_pixel(size_t x, size_t y, boost::uint8_t a);

	};

	/// Write the given image to the given out channel in a specified format.
	//
	/// @param type     The image format to write in (see libcore/gnash.h)
	/// @param out      The IOChannel to write to.
	/// @param image    The image to write.
	/// @param quality  The quality of the image output (not used for all formats)
	DSOEXPORT void writeImageData(FileType type, boost::shared_ptr<gnash::IOChannel> out,
	                              image_base* image, int quality);

	/// \brief
	/// For reading SWF JPEG2-style image data, using pre-loaded
	/// headers stored in the given jpeg::input object.
	DSOEXPORT std::auto_ptr<rgb> readSWFJpeg2WithTables(JpegImageInput& loader);

	/// \brief
	/// For reading SWF JPEG3-style image data, like ordinary JPEG, 
	/// but stores the data in rgba format.
	DSOEXPORT std::auto_ptr<rgba> readSWFJpeg3(boost::shared_ptr<gnash::IOChannel> in);
	
	DSOEXPORT std::auto_ptr<rgb> readImageData(boost::shared_ptr<gnash::IOChannel> in, FileType type);

} // namespace image
} // namespace gnash

#endif // IMAGE_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
