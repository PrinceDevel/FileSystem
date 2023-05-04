#include "precompiled.hpp"
#include "FileSystem/Utils/DataTypes.hpp"
#include "FileSystem/2D/Image.hpp"


namespace FileSystem
{
    
Image* BitmapImage::Load(const char* path)
{
    std::ifstream stream (path, std::ios::in | std::ios::binary);
    if (!stream.is_open())
    {
        std::cout << "[ERROR] Unable to open Image at path: " << path << std::endl;
        return nullptr;
    }

    stream >> std::noskipws;
    uint8_t buffer[4], garbage;
    const uint8_t BM_FILE_HEADER_SIZE = 40, BM_INFO_HEADER_SIZE = 12;

    // VERIFICATIon
    {
        stream >> buffer[0] >> buffer[1];
        if (buffer[0] != 'B' || buffer[1] != 'M')
        {
            std::cout << "BMP Verfication Failed[" << buffer[0] << buffer[1] << "]" << std::endl;
            stream.close();
            exit(0);
        }
    }

    // FILE SIZE [2 - 5]
    stream >> garbage >> garbage >> garbage >> garbage;
    // RESERVED
    stream >> garbage >> garbage >> garbage >> garbage;
    // PIXEL DATA OFFSET
    stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
    const uint16_t offset = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
    // INFO HEADER SIZE
    stream >> garbage >> garbage >> garbage >> garbage;
    // IMAGE WIDTH
    stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
    const uint32_t width = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
    // IMAGE HEIGHT
    stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
    const uint32_t height = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
    // PLANCE COUNT
    stream >> garbage >> garbage;
    // BITS PER PIXEL
    stream >> buffer[0] >> buffer[1];
    const uint16_t bitsPerPixel = buffer[0] + (buffer[1] << 8);
    if (bitsPerPixel != 24 && bitsPerPixel != 32)
    {
        std::cout << "[ERROR]" << std::endl 
        << "Supported Color Depth: 24-bit and 32-bit" << std::endl
        << "Provided: " << bitsPerPixel << "-bit" << std::endl;
        stream.close();
        return nullptr;
    }
    const uint8_t channels = (bitsPerPixel == 32) ? 4 : 3;

    // UNIMPORTANT
    for (int i=0; i<24; i++) stream >> garbage;
    uint32_t index = offset - BM_FILE_HEADER_SIZE - BM_INFO_HEADER_SIZE;
    for (; index > 0; index--) stream >> garbage;


    auto image = new Image (width, height, sizeof(uint8_t)*channels, channels);
    auto count = width * height;
    for (index = 0; index < image->m_Size;)
    {
        buffer[3] = UINT8_MAX;
        stream >> buffer[2] >> buffer[1] >> buffer[0];
        if (channels == 4) stream >> buffer[3];
        image->m_Data[index++] = buffer[0];
        image->m_Data[index++] = buffer[1];
        image->m_Data[index++] = buffer[2];
        image->m_Data[index++] = buffer[3];
        //Color32(buffer[0], buffer[1], buffer[2], buffer[3]);
    }

    stream.close();
    return image;
}

}