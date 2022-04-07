#include "bmp.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <memory.h>

template <typename T>
void swap_values(T * ptr1, T * ptr2) {
   T tmp = *ptr1;
   *ptr1 = *ptr2;
   *ptr2 = tmp;
}

template <typename T>
void swap_pointers(T * ptr1, T * ptr2) {
   T * tmp = ptr1;
   ptr1 = ptr2;
   ptr2 = ptr1;
}

template <typename T>
void reverse_bytes(T * data, size_t n_bytes) {

   uint8_t * first = (uint8_t *) data;
   uint8_t * last  = (uint8_t *) (data + n_bytes - 1);

   while (first < last) 
      swap_values(first++, last--);
}

enum StreamState { GOOD, DONE, ERROR };
StreamState check_filestream(const std::ifstream & s) 
{
   if (!s.good()) {
      if (s.fail() || s.bad())
         return StreamState::ERROR;
      else if (s.eof())
         return StreamState::DONE;
      else
         throw std::runtime_error("Stream threw unknown error.");
   } else 
      return StreamState::GOOD;
}

/** Checked reading of filestream. */
void read_bytes(void * addr, size_t n_bytes, std::ifstream & s) 
{
   if (check_filestream(s) != GOOD)
      throw std::runtime_error("Couldn't read bytes from stream.");
   
   s.read((char *) addr, n_bytes);

   if (check_filestream(s) != GOOD)
      throw std::runtime_error("Error after reading bytes from filestream.");
}

class NullException : std::exception {
public:
   NullException(const std::string & message) 
      : msg { message } { };
   NullException() : NullException { "No error message specified." } { };

   virtual const char * what() const noexcept {
      return msg.c_str();
   }
private:
   std::string msg;
};

template <typename T>
T byte_array_to_T(void * arr) 
{
   if (arr == nullptr)
      throw NullException { };

   T result;
   memcpy(&result, arr, sizeof(T));

   return result;
}

bool is_valid_bmp_field(const uint8_t field[2])
{
   if (field[0] != 'B' || field[1] != 'M') 
      return false;
   else
      return true;
}

struct RawBmp {
   /** The bmp header has the following structure. src: https://en.wikipedia.org/wiki/BMP_file_format
    * 2 bytes: Header Field - offset 0
    * 4 bytes: File size - offset 2
    * 4 bytes: Reserved - offset 6
    * 4 bytes: Offset for pixel data - offset 10 */
   static constexpr size_t headerSize = 14;
   u_int fileSize;
   u_int dataOffset;

   uint8_t * data;    // the data of the file, excluding the file header

   RawBmp(std::ifstream & fileStream)
   {
      // load header
      uint8_t headerData[headerSize];
      read_bytes(headerData, headerSize, fileStream);

      // check if header has the right field specified
      uint8_t tmpField[2];
      memcpy(tmpField, &headerData[0], 2);
      if (!is_valid_bmp_field(tmpField)) 
         throw std::runtime_error("BMP file couldn't be loaded: field is invalid.");

      memcpy(&fileSize,   &headerData[2],  sizeof(u_int));
      memcpy(&dataOffset, &headerData[10], sizeof(u_int));
   
      data = new uint8_t[fileSize];

      // load header into data
      memcpy(data, headerData, headerSize);

      // read the rest of the file into the data buffer
      read_bytes(data + headerSize, fileSize - headerSize, fileStream);
   }

   RawBmp(const RawBmp & other)
      : fileSize { other.fileSize }
   {
      data = new uint8_t[fileSize];
      memcpy(data, other.data, fileSize);
   }

   RawBmp(RawBmp && other) 
      : fileSize { other.fileSize }, data { other.data }
   {
      other.data = nullptr;
   }

   ~RawBmp() { delete[] data; };
};

/** Load RawBmp object from given filename. */
RawBmp bmp_loadraw(const std::string & filename) 
{
   std::ifstream fileStream { filename };
   if (!fileStream) 
      throw std::runtime_error("File \"" + filename + "\" could not be loaded.");

   return RawBmp { fileStream };
}

// src: https://en.wikipedia.org/wiki/BMP_file_format
// indexes of enumeration match the wikipedia-page under the Windows BITMAPINFOHEADER table
enum CompressionMethods { BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS, BI_JPEG, BI_PNG, BI_ALPHABITFIELD };

/** Check whether a given compression method read from a BMP DIB header is supported 
 * by the current system. */ 
bool supported_compression_method(u_int cm) {
   if (cm == BI_RGB) return true; // only BI_RGB is supported for now
   else              return false;
}

constexpr int maxImageDim = 10000;
constexpr int supportedBitdepthts[2] = { 24, 32 };
constexpr int maxHeaderSize = 140;

/** Checks parameters from dib header against a set of invariants. */
bool are_valid_dib_params(u_int width, u_int height, u_int bitdepth, u_int headerSize, u_int fileSize) {
   bool supported = false;
      for (size_t i = 0; i < 2; i++)
         if (bitdepth == supportedBitdepthts[i]) { supported = true; break; }
   
        if (width  < 0 || width  > maxImageDim) return false;
   else if (height < 0 || height > maxImageDim) return false;
   else if (width * height > fileSize)          return false;
   else if (headerSize > maxHeaderSize)         return false;
   else if (!supported)                         return false;
   else return true;
}

/* Reads image specified by given bitdepth, width and height, starting from the first index
in the pixelData array. */
fbg::Frame read_bmp_pixel_data(const uint8_t * pixelData, u_short bitdepth, u_int width, u_int height) {
   fbg::Frame resf { int(width), int(height) };

   if (bitdepth == 24) {
      // bitdepth 24: |B8|G8|R8|
      size_t srcWidth = 3 * width;
      size_t srcHeight = height;
      for (size_t y = 0; y < height; y++) {
         for (size_t x = 0; x < height; x++) {
            size_t dataIdx = x * 3 + srcWidth * (srcHeight - 1 - y); // bmp stored in opposite
            size_t pxIdx   = x + width * y;

            resf.pixels[pxIdx].b = pixelData[dataIdx + 0];
            resf.pixels[pxIdx].g = pixelData[dataIdx + 1];
            resf.pixels[pxIdx].r = pixelData[dataIdx + 2];
            resf.pixels[pxIdx].a = 255;
         }
      }
   } else if (bitdepth == 32) {
      // bitdepth 32: |B8|G8|R8|A8|
      size_t srcWidth = 4 * width;
      size_t srcHeight = height;
      for (size_t y = 0; y < height; y++) {
         for (size_t x = 0; x < height; x++) {
            size_t dataIdx = x * 4 + srcWidth * (srcHeight - 1 - y); // bmp stored in opposite
            size_t pxIdx   = x + width * y;

            resf.pixels[pxIdx].b = pixelData[dataIdx + 0];
            resf.pixels[pxIdx].g = pixelData[dataIdx + 1];
            resf.pixels[pxIdx].r = pixelData[dataIdx + 2];
            resf.pixels[pxIdx].a = pixelData[dataIdx + 3];
         }
      }
   } else {
      throw std::runtime_error("Bitdepth of given .bmp image file i unsupported. Bitdepth: " + std::to_string(bitdepth));
   }

   return resf;
}

/** Create graphics frame object from given raw bmp data. */
fbg::Frame create_frame_from_raw_bmp(const RawBmp & bmp) {

   // read DIB header of bmp
   int dibIdx = RawBmp::headerSize; 
   u_int headerSize;
   int width, height;
   u_short bitdepth;

   // read compression method, and check if supported
   u_int compressionMethod;
   memcpy(&compressionMethod, bmp.data + dibIdx + 16, sizeof(u_int));

   if (!supported_compression_method(compressionMethod))
      throw std::runtime_error("Compression method is unsupported.");
   
   // read attrbiutes from DIB header of image
   memcpy(&headerSize, bmp.data + dibIdx + 0,  sizeof(u_int));
   memcpy(&width,      bmp.data + dibIdx + 4,  sizeof(int));
   memcpy(&height,     bmp.data + dibIdx + 8,  sizeof(int));
   memcpy(&bitdepth,   bmp.data + dibIdx + 14, sizeof(u_short));

   // check for validity of odib header parameters
   are_valid_dib_params(width, height, bitdepth, headerSize, bmp.fileSize);

   // read pixel data based on given attributes
   fbg::Frame resultFrame = read_bmp_pixel_data(bmp.data + dibIdx + headerSize, bitdepth, width, height);

   return resultFrame;
}

/** Create frame from given filename of bmp-file. */
fbg::Frame fbg::create_frame(const std::string & filename) {
   return create_frame_from_raw_bmp(bmp_loadraw(filename));
}