#include "precompiled.hpp"
#include "FileSystem/2D/Image.hpp"

namespace FileSystem
{

Image::Image (const uint32_t width, const uint32_t height, const uint8_t pixelSize, const uint32_t channels, const uint32_t colorSpace)
: m_Width(width), m_Height(height), m_Size(width * height * pixelSize), m_Channels(channels), m_ColorSpace(colorSpace)
{
    m_Data.resize(m_Size, 0);
}

Image::~Image() {}
}