#include "image.hpp"

using namespace fbg;

Image::Image(const V2d<float> & p, const std::string & filename)
   : m_filename { filename }, m_frame { create_frame(filename) },
     m_w { static_cast<float> (m_frame.w) },
     m_h { static_cast<float> (m_frame.h) }
{ 
   add_point(p);
}

void Image::draw_fill(Frame & frame) const
{
   V2d<float> p = pos();
   int tlX = static_cast<int> (p.x - m_w / 2.0f);
   int tlY = static_cast<int> (p.y - m_h / 2.0f);
   frame.add(this->m_frame, tlX, tlY);
}