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

   u_char * first = (u_char *) data;
   u_char * last  = (u_char *) (data + n_bytes - 1);

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

/** The bmp header has the following structure. src: https://en.wikipedia.org/wiki/BMP_file_format
 * 2 bytes: Header Field - offset 0
 * 4 bytes: File size - offset 2
 * 4 bytes: Reserved - offset 6
 * 4 bytes: Offset for pixel data - offset 10 */
struct BmpHeader {
   static constexpr size_t size = 14;
   BmpHeader(std::ifstream & fileStream) {
      u_char headerBytes[size];
      read_bytes(headerBytes, size, fileStream);

      memcpy(&field,       &headerBytes[0],  sizeof(u_char) * 2);
      memcpy(&fileSize,    &headerBytes[2],  sizeof(u_int));
      memcpy(&dataOffset,  &headerBytes[10], sizeof(u_int));
   }

   u_char field[2];
   u_int fileSize;
   u_int dataOffset;
};

bool bmp_header_is_valid(const BmpHeader & header) 
{
   if (header.field[0] != 'B' || header.field[1] != 'M') 
      return false;
   else
      return true;
}

struct RawBMP {
   RawBMP(std::ifstream & fileStream, const BmpHeader & h)
      : header { h }, sz { h.fileSize - BmpHeader::size }
   {
      data = new u_char[h.fileSize];
      read_bytes(data, sz, fileStream);
   }

   RawBMP(const RawBMP & other)
      : sz { other.sz }, header { other.header }
   {
      data = new u_char[sz];
      memcpy(data, other.data, sz);
   }

   RawBMP(RawBMP && other) 
      : sz { other.sz }, data { other.data }, header { other.header }
   {
      delete other.data;
   }

   ~RawBMP() { delete data; };

   BmpHeader header;
   u_char * data;
   const size_t sz;
};

std::ostream & operator << (std::ostream & os, const RawBMP & bmp) {
   os << std::hex;
   for (size_t i = 0; i < bmp.sz; i++){
      os << static_cast<int> (bmp.data[i]) << ' ';
      if (i % 8 == 7) os << std::endl;
   }
   os << '\n';

   return os;
}

RawBMP bmp_loadraw(const std::string & filename) 
{
   std::ifstream fileStream { filename };
   if (!fileStream) 
      throw std::runtime_error("File \"" + filename + "\" could not be loaded.");

   BmpHeader header { fileStream };
   
   if (!bmp_header_is_valid(header))
      throw std::runtime_error("BMP header of file \"" + filename + "\" is invalid.");

   std::cout << "Valid BMP header found. Attributes:\n" << std::dec
         << '\t' << "filesize:     " << header.fileSize   << " bytes\n"
         << '\t' << "data address: " << header.dataOffset << '\n';

   return RawBMP { fileStream, header };
}