#pragma once
#include <stdint.h>
#include <vector>


namespace FileSystem
{

class Image
{
public:
    uint32_t m_Width, m_Height;
    uint64_t m_Size;    // Size in Bytes
    uint8_t m_Channels, m_ColorSpace;

    // std::vector<Color> m_Pixels;
    std::vector<uint8_t> m_Data;
    inline uint32_t ToIndex (const uint32_t row, const uint32_t col) { return row * m_Width + col; }

    Image (const uint32_t width, const uint32_t height, const uint8_t pixelSize = 3, const uint32_t channels = 0, const uint32_t colorSpace = 0);
    ~Image();

};



//! Quite Ok Image (QOI)  En/De-coder
struct ImageQOI
{
    static Image* Load (const char* path);
    static void Save (const Image* image, const char* path);
};


//! Bitmap Image (BMP)   En/De-coder
struct BitmapImage
{
    static Image* Load (const char* path);
    static void Save (const Image* image);
};

}

