#pragma once

#include <iostream>
#include <vector>
#include "FileSystem/Utils/DataTypes.hpp"

namespace FileSystem
{
    template <typename ColorType>
    class BitmapImage
    {
        bool        m_Transculent;      // Tells whether image has alpha channel
        uint16_t    m_Width;            // width of image in pixels
        uint16_t    m_Height;           // height of image in pixels
        uint32_t    m_Count;            // no. of pixels
        std::vector<ColorType> m_Pixels;    // array to store pixels

    public:
        BitmapImage (uint32_t, uint32_t, bool = false);

        void Save (const char* a_Filepath);
        static BitmapImage* Load (const char* a_Filepath);

        inline bool     IsTransculent() { return m_Transculent; }
        inline uint16_t GetWidth()  { return m_Width; }
        inline uint16_t GetHeight() { return m_Height; }
        inline uint32_t GetCount()  { return m_Count; }

        void SetPixel(uint16_t x, uint16_t y, ColorType pixel)
        {
            if (x < m_Width && y < m_Height)
                m_Pixels[y * m_Width + x] = pixel;
        }
        void SetPixel(uint32_t index, ColorType pixel)
        {
            if (index < m_Count)
                m_Pixels[index] = pixel;
        }

        ColorType GetPixel(uint16_t x, uint16_t y)
        {
            return (x < m_Width && y < m_Height) ? m_Pixels[x * m_Width + y] : ColorType(0);
        }
        ColorType GetPixel(uint32_t index)
        {
            return index < m_Count ? m_Pixels[index] : ColorType(0);
        }
    };

    typedef BitmapImage<Color32> BMP32;
    typedef BitmapImage<Color64> BMP64;
}