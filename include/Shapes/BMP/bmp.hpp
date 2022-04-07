#ifndef BMP_HPP
#define BMP_HPP
#include "../../frame.hpp"

#include <string>

class RawBmp;

namespace fbg {
   Frame create_frame(const std::string & filename);
};

#endif