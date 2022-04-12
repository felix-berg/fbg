#ifndef BMP_HPP
#define BMP_HPP
#include "../../frame.hpp"

#include <string>

struct RawBmp;

namespace fbg {
   Frame create_frame(const std::string & filename);
};

#endif