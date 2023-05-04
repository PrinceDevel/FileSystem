#include "precompiled.hpp"
#include "FileSystem/2D/Image.hpp"

#define COLOR_TEMP template <typename Color>


namespace FileSystem
{
    
Image* ImageQOI::Load(const char* path)
{
    const uint8_t 
        OP_RGB    = 0b11111110,
        OP_RGBA   = 0b11111111,
        OP_INDEX  = 0b00,
        OP_DIFF   = 0b01,
        OP_LUMA   = 0b10,
        OP_RUN    = 0b11
    ;
    uint8_t buffer[4], channels, colorSpace;
    uint32_t storage[64], height, width;
    uint8_t prevR, prevG, prevB, prevA = UINT8_MAX;

    struct { union { 
        struct { uint8_t r, g, b, a; }; 
        uint32_t rgba;
    };} convertor;


    // Opening QOI File
    std::ifstream stream (path, std::ios::binary | std::ios::in);
    if (!stream.is_open())
    {
        std::cout << "[ERROR] Unable to open image at path: " << path << std::endl;
        return nullptr;
    }
    
    // FILE FORMAT VERIFICATION
    {
        stream >> std::noskipws;
        stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
        if (buffer[0] != 'q' || buffer[1] != 'o' || buffer[2] != 'i' || buffer[3] != 'f')
        {
            std::cout << "[ERROR] File Format is not QOI" << std::endl;
            return nullptr;
        }
    }

    // QOI HEADER
    {
        stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
        width  = buffer[3] + (buffer[2] << 8) + (buffer[1] << 16) + (buffer[0] << 24);
        stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
        height = buffer[3] + (buffer[2] << 8) + (buffer[1] << 16) + (buffer[0] << 24);
        stream >> buffer[0]; channels   = (uint32_t) buffer[0];
        stream >> buffer[0]; colorSpace = (uint32_t) buffer[0];
    }

    
    Image* image = new Image(width, height, sizeof(uint8_t)*channels, channels, colorSpace);
    uint32_t index = 0;

    // Decompressing Image Data
    while (stream >> buffer[0])
    {
        if (index >= image->m_Size) break;

        if ((buffer[0] >> 6) == OP_RUN  &&  buffer[0] != OP_RGB  &&  buffer[0] != OP_RGBA)
            for (int16_t runLength = buffer[0] & 0b00111111; runLength >= 0; runLength--)
            {
                image->m_Data[index++] = prevR;
                image->m_Data[index++] = prevG;
                image->m_Data[index++] = prevB;
                if (channels == 4) image->m_Data[index++] = prevA;
            }

        else if (buffer[0] == OP_RGB)
        {
            stream >> buffer[0] >> buffer[1] >> buffer[2];
            image->m_Data[index++] = convertor.r = prevR = buffer[0];
            image->m_Data[index++] = convertor.g = prevG = buffer[1];
            image->m_Data[index++] = convertor.b = prevB = buffer[2];
            prevA = convertor.a = UINT8_MAX;

            uint32_t storageIndex = (3*prevR + 5*prevG + 7*prevB + 11*prevA) % 64;
            storage[storageIndex] = convertor.rgba; 
        }

        else if (buffer[0] == OP_RGBA)
        {
            stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
            image->m_Data[index++] = convertor.r = prevR = buffer[0];
            image->m_Data[index++] = convertor.g = prevG = buffer[1];
            image->m_Data[index++] = convertor.b = prevB = buffer[2];
            image->m_Data[index++] = convertor.a = prevA = buffer[3];
            uint32_t storageIndex = (3*prevR + 5*prevG + 7*prevB + 11*prevA) % 64;
            storage[storageIndex] = convertor.rgba;
        }

        else if ((buffer[0] >> 6) == OP_INDEX)
        {
            uint8_t refIndex = buffer[0] & 0b00111111;
            image->m_Data[index++] = prevR = (uint8_t) (storage[refIndex] >> 24);
            image->m_Data[index++] = prevG = (uint8_t) (storage[refIndex] >> 16);
            image->m_Data[index++] = prevB = (uint8_t) (storage[refIndex] >> 8);
            if (channels == 4) image->m_Data[index++] = prevA = (uint8_t) storage[refIndex];
            else prevA = UINT8_MAX;
        }

        else if ((buffer[0] >> 6) == OP_DIFF)
        {
            image->m_Data[index++] = convertor.r = prevR = ((buffer[0] >> 4) & 0b00000011) + prevR;
            image->m_Data[index++] = convertor.g = prevG = ((buffer[0] >> 2) & 0b00000011) + prevG;
            image->m_Data[index++] = convertor.b = prevB = (buffer[0] & 0b00000011)        + prevB;
            
            if (channels == 4) image->m_Data[index++] = convertor.a = prevA; 
            else convertor.a = prevA = UINT8_MAX;

            uint32_t storageIndex = (3*prevR + 5*prevG + 7*prevB + 11*prevA) % 64;
            storage[storageIndex] = convertor.rgba;
        }

        else if ((buffer[0] >> 6) == OP_LUMA)
        {
            stream >> buffer[1];
            uint8_t dg = buffer[0] & 0b00111111;
            uint8_t dr = ((buffer[1] >> 4) & 0b00001111) + dg;
            uint8_t db = (buffer[1] & 0b00001111) + dg;

            if (dr >= 0) dr += 1;
            if (dg >= 0) dg += 1;
            if (db >= 0) db += 1;

            image->m_Data[index++] = convertor.r = prevR = dr + prevR;
            image->m_Data[index++] = convertor.g = prevG = dg + prevG;
            image->m_Data[index++] = convertor.b = prevB = db + prevB;
            
            if (channels == 4) image->m_Data[index++] = convertor.a = prevA; 
            else convertor.a = prevA = UINT8_MAX;

            uint32_t storageIndex = (3*prevR + 5*prevG + 7*prevB + 11*prevA) % 64;
            storage[storageIndex] = convertor.rgba;
        }
    }
    
    stream.close();
    return image;
}

void ImageQOI::Save (const Image* image, const char* path)
{
    const uint8_t 
        OP_RGB    = 0b11111110,
        OP_RGBA   = 0b11111111,
        OP_INDEX  = 0b00000000,
        OP_DIFF   = 0b01000000,
        OP_LUMA   = 0b10000000,
        OP_RUN    = 0b11000000
    ;
    uint32_t storage[64];

    struct { union { 
        struct { uint8_t r, g, b, a; }; 
        uint32_t rgba;
    };} current, previous;
    previous.a = UINT8_MAX;


    // Opening QOI File
    std::ofstream stream (path, std::ios::binary | std::ios::out);
    if (!stream.is_open())
    {
        std::cout << "[ERROR] Unable to Open Image at Path: " << path << std::endl;
        return ;
    }
    stream << std::noskipws;
    // std::ostringstream stream;

    // File Header
    {
        // [0-3] Magical Letters
        stream << 'q' << 'o' << 'i' << 'f';
        // [4-7] Width of image
        stream << (uint8_t) (image->m_Width >> 24) << (uint8_t) (image->m_Width >> 16) << (uint8_t) (image->m_Width >> 8) << (uint8_t) image->m_Width;
        // [8-11] Height of image
        stream << (uint8_t) (image->m_Height >> 24) << (uint8_t) (image->m_Height >> 16) << (uint8_t) (image->m_Height >> 8) << (uint8_t) image->m_Height;
        // [12] Color Channels and [13] Color Space
        stream << image->m_Channels << image->m_ColorSpace;
    }

    // Encoding Pixel Data
    uint32_t run_length = 0;
    for (int index = 0; index < image->m_Size; index += image->m_Channels)
    {
        current.r = image->m_Data[index];
        current.g = image->m_Data[index + 1];
        current.b = image->m_Data[index + 2];
        current.a = (image->m_Channels == 4 ? image->m_Data[index + 3] : UINT8_MAX);

       
        if (previous.rgba == current.rgba)
        {
            run_length++;
            continue;
        }

        uint8_t storageIndex = (3*current.r + 5*current.g + 7*current.b + 11*current.a) % 64;
        if (run_length != 0 )
        {
            uint32_t runCount = run_length / 63;
            runCount += (run_length % 63) > 0;

            stream << (uint8_t)(OP_RUN ^ (run_length-1));
            if (runCount == 1) run_length = 0;
            else run_length -= 63;
            
            for (int i = 1; i < runCount; i++)
            {
                stream << (uint8_t)(OP_INDEX ^ storageIndex);
                stream << (uint8_t)(OP_RUN ^ ((run_length - 63 < 1) ? 62U : run_length-1));
            }
        }


        if (storage[storageIndex] == current.rgba)
        {
            stream << (OP_INDEX & storageIndex);
            previous.rgba = current.rgba;
            continue;
        }

        storage[storageIndex] = current.rgba;
        uint8_t dr = current.r + 2 - previous.r;
        uint8_t dg = current.g + 2 - previous.g;
        uint8_t db = current.b + 2 - previous.b;
        uint8_t da = (image->m_Channels == 4) ? (current.a - previous.a) : 0;
        previous.rgba = current.rgba;

        if (da == 0 && dr < 4U && dg < 4U && db < 4U)
        {
            stream << (uint8_t)(OP_DIFF ^ (dr << 4U) ^ (dg << 2U) ^ db);
            continue;
        }

        uint8_t dr_dg = dr - dg;
        uint8_t db_dg = db - dg;
        dg += 30;
        if (da == 0 && dg < 64U && dr_dg < 16U && db_dg < 16U)
        {
            stream << (uint8_t)(OP_LUMA ^ dg) << (uint8_t)((dr_dg << 4) ^ db_dg);
            continue;
        }

        stream << ((image->m_Channels == 4) ? OP_RGBA : OP_RGB);
        stream << current.r << current.g << current.b;
        if (image->m_Channels == 4) stream << current.a;

    }
    for (int i = 0; i < 7; i++)
        stream << (uint8_t) 0;
    stream << (uint8_t) 1;
    
    // std::string str = stream.str();
    // for (int i = 0; i < str.length(); i++)
    //     std::cout << (int) str.at(i) << ' ';
    // std::cout << std::endl;
    stream.close();
}



}