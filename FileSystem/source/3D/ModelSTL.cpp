#include "precompiled.hpp"
#include "FileSystem/3D/ModelSTL.hpp"

namespace FileSystem
{

    ModelSTL::ModelSTL() {}
    ModelSTL::~ModelSTL() {}


    //! Decoder For STL Files
    //? [ASCII]
    ModelSTL* ModelSTL::LoadASCII (const char* path)
    {
        std::ifstream stream (path);
        if (!stream.is_open())
        {
            std::cout << "Error: Unable to open file at path " << path << std::endl;
            return nullptr;
        }

        stream >> std::noskipws;
        std::string line;
        ModelSTL* model = new ModelSTL();

        TriangularData data;
        uint32_t index = 0;
        uint8_t dataIndex = 0;
        std::string token;
        bool readingVertices = false;

        while (getline(stream, line))
        {
            std::stringstream lineStream (line);
            lineStream >> token;

            if (token.compare("solid") == 0)
                model->m_Label = line.substr(token.length()+1);

            else if (token.compare("facet") == 0)
            {
                lineStream >> token; // -> normal
                lineStream >> token; data.normal.x = std::stof(token);
                lineStream >> token; data.normal.y = std::stof(token);
                lineStream >> token; data.normal.z = std::stof(token);
                dataIndex++;
            }
            else if (token.compare("endfacet") == 0)
            {
                model->m_Faces.push_back(data);
                index++;
                dataIndex = 0;
                readingVertices = false;
            }

            else if (token.compare("outer") == 0)
                readingVertices = true;
            else if (token.compare("endloop") == 0)
                readingVertices = false;
            
            else if (token.compare("vertex") == 0)
            {
                if (!readingVertices)
                {
                    std::cout << "Error: STL File Is Incorrect !!" << std::endl;
                    return nullptr;
                }
                switch (dataIndex)
                {
                    case 1:
                        lineStream >> token; data.first.x = std::stof(token);
                        lineStream >> token; data.first.y = std::stof(token);
                        lineStream >> token; data.first.z = std::stof(token);
                        break;
                    case 2:
                        lineStream >> token; data.second.x = std::stof(token);
                        lineStream >> token; data.second.y = std::stof(token);
                        lineStream >> token; data.second.z = std::stof(token);
                        break;
                    case 3:
                        lineStream >> token; data.third.x = std::stof(token);
                        lineStream >> token; data.third.y = std::stof(token);
                        lineStream >> token; data.third.z = std::stof(token);
                        break;
                }
                dataIndex++;
            }
        }

        stream.close();
        return model;
    }
    //? [Binary]
    ModelSTL* ModelSTL::LoadBinary (const char* path)
    {
        std::ifstream stream (path, std::ios::binary);
        if (!stream.is_open())
        {
            std::cout << "Error: Unable to open file at path " << path << std::endl;
            return nullptr;
        }

        stream >> std::noskipws;
        uint8_t buffer[4] = {0};
        union {
            uint32_t integer;
            float real;
        } value;

        for (int i = 0; i < 80; i++)
            stream >> buffer[0];

        #define Convert() \
        stream >> buffer[0] >> buffer[1] >> buffer[2] >> buffer[3];\
        value.integer =  buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24)

        Convert(); uint32_t count = value.integer;
        ModelSTL* model = new ModelSTL();
        model->m_Faces.resize(count);

        for (int i = 0; i < count; i++)
        {
            // Normals
            Convert(); model->m_Faces[i].normal.x = value.real;
            Convert(); model->m_Faces[i].normal.y = value.real;
            Convert(); model->m_Faces[i].normal.z = value.real;
            // Vertex 1
            Convert(); model->m_Faces[i].first.x = value.real;
            Convert(); model->m_Faces[i].first.y = value.real;
            Convert(); model->m_Faces[i].first.z = value.real;
            // Vertex 2
            Convert(); model->m_Faces[i].second.x = value.real;
            Convert(); model->m_Faces[i].second.y = value.real;
            Convert(); model->m_Faces[i].second.z = value.real;
            // Vertex 3
            Convert(); model->m_Faces[i].third.x = value.real;
            Convert(); model->m_Faces[i].third.y = value.real;
            Convert(); model->m_Faces[i].third.z = value.real;

            stream >> buffer[0] >> buffer[1];
        }

        #undef Convert
        stream.close();
        return model;
    }


    //! Encoder For STL Files
    //? [ASCII]
    void ModelSTL::SaveASCII (const char* path)
    {
        std::ofstream stream (path);
        if (!stream.is_open())
        {
            std::cout << "Error: Unable to open file at path " << path << std::endl;
            return;
        }

        stream << "solid " << m_Label << '\n';
        for (auto face: m_Faces)
        {
            stream << "facet normal " << face.normal.x << ' ' << face.normal.y << ' ' << face.normal.z << '\n';
            stream << "outer loop\n";
            stream << "vertex " << face.first.x << ' ' << face.first.y << ' ' << face.first.z << '\n';
            stream << "vertex " << face.second.x << ' ' << face.second.y << ' ' << face.second.z << '\n';
            stream << "vertex " << face.third.x << ' ' << face.third.y << ' ' << face.third.z << '\n';
            stream << "endloop\n";
            stream << "endfacet\n";
        }
        stream << "endsolid " << m_Label << '\n';

        stream.close();
    }
    //? [Binary]
    void ModelSTL::SaveBinary (const char* path)
    {
        std::ofstream stream (path, std::ios::binary);
        if (!stream.is_open())
        {
            std::cout << "Error: Unable to open file at path " << path << std::endl;
            return;
        }

        uint8_t null_char = (uint8_t) NULL;
        union {
            uint32_t integer;
            float real;
        } value;

        for (int i = 0; i < 80; i++)
            stream << null_char;

        #define Write(x) \
            stream << (uint8_t) x;\
            stream << (uint8_t) (x >> 8);\
            stream << (uint8_t) (x >> 16);\
            stream << (uint8_t) (x >> 24)

        uint32_t size = m_Faces.size();
        Write(size);

        for (auto faceData: m_Faces)
        {
            value.real = faceData.normal.x; Write(value.integer);
            value.real = faceData.normal.y; Write(value.integer);
            value.real = faceData.normal.z; Write(value.integer);

            value.real = faceData.first.x;  Write(value.integer);
            value.real = faceData.first.y;  Write(value.integer);
            value.real = faceData.first.z;  Write(value.integer);
            
            value.real = faceData.second.x; Write(value.integer);
            value.real = faceData.second.y; Write(value.integer);
            value.real = faceData.second.z; Write(value.integer);
            
            value.real = faceData.third.x;  Write(value.integer);
            value.real = faceData.third.y;  Write(value.integer);
            value.real = faceData.third.z;  Write(value.integer);
            
            stream << null_char << null_char;
        }

        #undef Write
        stream.close();
    }


    std::ostream& operator<<(std::ostream& stream, const ModelSTL& model)
    {
        return stream;
    }
}
