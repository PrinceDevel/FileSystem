// #include "precompiled.hpp"
// #include "FileSystem/2D/BitmapImage.hpp"


// #define BM_INFO_HEADER_SIZE 40
// #define BM_FILE_HEADER_SIZE 14

// namespace FileSystem
// {   
//     template<> BitmapImage<Color32>::BitmapImage (uint32_t a_Width, uint32_t a_Height, bool a_Transculent)
//     : m_Width(a_Width), m_Height(a_Height), m_Transculent(a_Transculent)
//     {
//         this->m_Count   = a_Width * a_Height;
//         this->m_Pixels  = std::vector<Color32> (m_Count, Color32(0));
//     }
//     template<> BitmapImage<Color64>::BitmapImage (uint32_t a_Width, uint32_t a_Height, bool a_Transculent)
//     : m_Width(a_Width), m_Height(a_Height), m_Transculent(a_Transculent)
//     {
//         this->m_Count   = a_Width * a_Height;
//         this->m_Pixels  = std::vector<Color64> (m_Count, Color64(0));
//     }

//     template<> void BitmapImage<Color32>::Save (const char* a_FilePath)
//     {
//         std::ofstream stream (a_FilePath, std::ios::binary);
//         if (!stream.is_open())
//         {
//             std::cout << "Unable to write in file " << a_FilePath << std::endl;
//             return;
//         }

//         const uint16_t pixelDataOffset = BM_FILE_HEADER_SIZE + BM_INFO_HEADER_SIZE;
//         const uint32_t pixelsSize = sizeof(uint8_t) * 3 * GetCount();
//         const uint32_t fileSize = pixelDataOffset + pixelsSize;
//         const uint8_t  ZERO = 0;

//         // PUTTING HEADER DATA
//         //? Refrence: http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
//         { 
//             // VERIFICATION [0 - 1]
//             stream << 'B' << 'M';
//             // FILE SIZE [2 - 5]
//             stream << (uint8_t) fileSize << (uint8_t) (fileSize >> 8) << (uint8_t) (fileSize >> 16) << (uint8_t) (fileSize >> 24);
//             // RESERVED [6 - 9]
//             stream << ZERO << ZERO << ZERO << ZERO;
//             // PIXEL DATA OFFSET [10 - 13]
//             stream << (uint8_t) pixelDataOffset << ZERO << ZERO << ZERO;
//             // INFO HEADER SIZE [14 - 17]
//             stream << (uint8_t) BM_INFO_HEADER_SIZE << ZERO << ZERO << ZERO;
//             // IMAGE WIDTH [18 - 21]
//             stream << (uint8_t) GetWidth() << (uint8_t) (GetWidth() >> 8) << ZERO << ZERO;
//             // IMAGE HEIGHT [22 - 25]
//             stream << (uint8_t) GetHeight() << (uint8_t) (GetHeight() >> 8) << ZERO << ZERO;
//             // PLANES COUNT [26 - 27]
//             stream << (uint8_t) 1 << ZERO;
//             // BITS PER PIXEL [28 - 29]
//             stream << (uint8_t) (IsTransculent() ? 32 : 24) << ZERO;

//             // UNIMPORTANT
//             for (int i = 0; i < 24; i++) stream << ZERO;
//         }

//         // Putting Pixel Data
//         for (int pos = 0; pos < GetCount(); pos++)
//         {
//             Color32 pixel = GetPixel(pos);
//             stream << pixel.blue;
//             stream << pixel.green;
//             stream << pixel.red;
//             if (IsTransculent())
//                 stream << pixel.alpha;
//         }
//         stream.close();
//     }
//     template<> void BitmapImage<Color64>::Save (const char* a_FilePath)
//     {
//         std::ofstream stream (a_FilePath, std::ios::binary);
//         if (!stream.is_open())
//         {
//             std::cout << "Unable to write in file " << a_FilePath << std::endl;
//             return;
//         }

//         const uint16_t pixelDataOffset = BM_FILE_HEADER_SIZE + BM_INFO_HEADER_SIZE;
//         const uint32_t pixelsSize = sizeof(uint8_t) * 3 * GetCount();
//         const uint32_t fileSize = pixelDataOffset + pixelsSize;
//         const uint8_t  ZERO = 0;

//         // PUTTING HEADER DATA
//         //? Refrence: http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
//         { 
//             // VERIFICATION [0 - 1]
//             stream << 'B' << 'M';
//             // FILE SIZE [2 - 5]
//             stream << (uint8_t) fileSize << (uint8_t) (fileSize >> 8) << (uint8_t) (fileSize >> 16) << (uint8_t) (fileSize >> 24);
//             // RESERVED [6 - 9]
//             stream << ZERO << ZERO << ZERO << ZERO;
//             // PIXEL DATA OFFSET [10 - 13]
//             stream << (uint8_t) pixelDataOffset << ZERO << ZERO << ZERO;
//             // INFO HEADER SIZE [14 - 17]
//             stream << (uint8_t) BM_INFO_HEADER_SIZE << ZERO << ZERO << ZERO;
//             // IMAGE WIDTH [18 - 21]
//             stream << (uint8_t) GetWidth() << (uint8_t) (GetWidth() >> 8) << ZERO << ZERO;
//             // IMAGE HEIGHT [22 - 25]
//             stream << (uint8_t) GetHeight() << (uint8_t) (GetHeight() >> 8) << ZERO << ZERO;
//             // PLANES COUNT [26 - 27]
//             stream << (uint8_t) 1 << ZERO;
//             // BITS PER PIXEL [28 - 29]
//             stream << (uint8_t) (IsTransculent() ? 64 : 48) << ZERO;

//             // UNIMPORTANT
//             for (int i = 0; i < 24; i++) stream << ZERO;
//         }

//         // Putting Pixel Data
//         for (int pos = 0; pos < GetCount(); pos++)
//         {
//             Color64 pixel = GetPixel(pos);
//             stream << (uint8_t) pixel.blue << (uint8_t) (pixel.blue >> 8);
//             stream << (uint8_t) pixel.green << (uint8_t) (pixel.green >> 8);
//             stream << (uint8_t) pixel.red << (uint8_t) (pixel.red >> 8);
//             if (IsTransculent())
//                 stream << (uint8_t) pixel.alpha << (uint8_t) (pixel.alpha >> 8);
//         }
//         stream.close();
//     }

//     template<> BitmapImage<Color32>* BitmapImage<Color32>::Load (const char* a_Filepath)
//     {
//         std::ifstream stream (a_Filepath, std::ios::in | std::ios::binary);
//         if (!stream.is_open())
//         {
//             std::cout << "[ERROR] Unabled to read file at path: " << a_Filepath << std::endl;
//             return nullptr;
//         }

//         stream >> std::noskipws;
//         uint8_t garbage;
//         uint8_t buffer[4];
//         // VERIFICATIon
//         {
//             unsigned char b, m;
//             stream >> b >> m;
//             if (b != 'B' || m != 'M')
//             {
//                 std::cout << "BMP Verfication Failed[" << b << m << "]" << std::endl;
//                 stream.close();
//                 exit(0);
//             }
//         }

//         // FILE SIZE [2 - 5]
//         stream >> garbage >> garbage >> garbage >> garbage;
//         // RESERVED
//         stream >> garbage >> garbage >> garbage >> garbage;
//         // PIXEL DATA OFFSET
//         stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//         const uint16_t offset = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
//         // INFO HEADER SIZE
//         stream >> garbage >> garbage >> garbage >> garbage;
//         // IMAGE WIDTH
//         stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//         const uint16_t width = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
//         // IMAGE HEIGHT
//         stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//         const uint16_t height = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
//         // PLANCE COUNT
//         stream >> garbage >> garbage;
//         // BITS PER PIXEL
//         stream >> buffer[0] >> buffer[1];
//         const uint16_t bitsPerPixel = buffer[0] + (buffer[1] << 8);
//         if (bitsPerPixel != 24 && bitsPerPixel != 32)
//         {
//             std::cout << "[ERROR]" << std::endl 
//             << "Supported Color Depth: 24-bit and 32-bit" << std::endl
//             << "Provided: " << bitsPerPixel << "-bit" << std::endl;
//             stream.close();
//             return nullptr;
//         }
//         bool isTransculent = bitsPerPixel == 32;

//         // UNIMPORTANT
//         for (int i=0; i<24; i++) stream >> garbage;
//         uint32_t index = offset - BM_FILE_HEADER_SIZE - BM_INFO_HEADER_SIZE;
//         for (; index > 0; index--) stream >> garbage;

//         BitmapImage<Color32>* image = new BitmapImage(width, height, isTransculent);
//         for (; index < image->GetCount(); index++)
//         {
//             uint8_t r, g, b, a = UINT8_MAX;
//             stream >> b >> g >> r;
//             if (isTransculent) stream >> a;
//             image->SetPixel(index, Color32(r, g, b, a));
//         }

//         stream.close();
//         return image;
//     }
//     template<> BitmapImage<Color64>* BitmapImage<Color64>::Load (const char* a_Filepath)
//     {
//         std::ifstream stream (a_Filepath, std::ios::in | std::ios::binary);
//         if (!stream.is_open())
//         {
//             std::cout << "[ERROR] Unabled to read file at path: " << a_Filepath << std::endl;
//             return nullptr;
//         }

//         stream >> std::noskipws;
//         uint8_t garbage;
//         uint8_t buffer[4];
//         // VERIFICATIon
//         {
//             unsigned char b, m;
//             stream >> b >> m;
//             if (b != 'B' || m != 'M')
//             {
//                 std::cout << "BMP Verfication Failed[" << b << m << "]" << std::endl;
//                 stream.close();
//                 exit(0);
//             }
//         }

//         // FILE SIZE [2 - 5]
//         stream >> garbage >> garbage >> garbage >> garbage;
//         // RESERVED
//         stream >> garbage >> garbage >> garbage >> garbage;
//         // PIXEL DATA OFFSET
//         stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//         const uint16_t offset = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
//         // INFO HEADER SIZE
//         stream >> garbage >> garbage >> garbage >> garbage;
//         // IMAGE WIDTH
//         stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//         const uint16_t width = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
//         // IMAGE HEIGHT
//         stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];
//         const uint16_t height = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
//         // PLANCE COUNT
//         stream >> garbage >> garbage;
//         // BITS PER PIXEL
//         stream >> buffer[0] >> buffer[1];
//         const uint16_t bitsPerPixel = buffer[0] + (buffer[1] << 8);
//         if (bitsPerPixel != 24 && bitsPerPixel != 32)
//         {
//             std::cout << "[ERROR]" << std::endl 
//             << "Supported Color Depth: 24-bit and 32-bit" << std::endl
//             << "Provided: " << bitsPerPixel << "-bit" << std::endl;
//             stream.close();
//             return nullptr;
//         }
//         bool isTransculent = bitsPerPixel == 32;

//         // UNIMPORTANT
//         for (int i=0; i<24; i++) stream >> garbage;
//         uint32_t index = offset - BM_FILE_HEADER_SIZE - BM_INFO_HEADER_SIZE;
//         for (; index > 0; index--) stream >> garbage;

//         BitmapImage<Color64>* image = new BitmapImage(width, height, isTransculent);
//         for (; index < image->GetCount(); index++)
//         {
//             stream >> buffer[0] >> buffer[1];
//             uint16_t b = buffer[0] + (buffer[1] << 8);
            
//             stream >> buffer[0] >> buffer[1];
//             uint16_t g = buffer[0] + (buffer[1] << 8);
            
//             stream >> buffer[0] >> buffer[1];
//             uint16_t r = buffer[0] + (buffer[1] << 8);

//             uint16_t a = UINT16_MAX;
//             if (isTransculent)
//             {
//                 stream >> buffer[0] >> buffer[1];
//                 a = buffer[0] + (buffer[1] << 8);
//             }

//             image->SetPixel(index, Color64(r, g, b, a));
//         }

//         stream.close();
//         return image;
//     }
// }