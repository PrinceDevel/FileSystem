#include "precompiled.hpp"
#include "FileSystem/3D/ModelOBJ.hpp"

namespace FileSystem
{
    ModelOBJ::ModelOBJ() {}
    ModelOBJ::~ModelOBJ() {}

    ModelOBJ* ModelOBJ::Load(const char* path)
    {
        std::ifstream stream (path);
        if (!stream.is_open())
        {
            std::cout << "Error: Fail To Load File At Path " << path << std::endl;
            return nullptr;
        }

        
        stream >> std::noskipws;
        std::string token;
        std::string line;
        ModelOBJ* model = new ModelOBJ();
        Vector3 vec3;
        Vector2 vec2;
        std::vector<uint32_t> faceIntVector (9);
        while (getline(stream, line))
        {
            std::istringstream lineStream (line);
            lineStream >> token;

            if (token.compare("#") == 0)
            {
                std::ostringstream strBuilder;
                while (lineStream >> token)
                    strBuilder << token << ' ';

                model->m_Info += "\n# " + strBuilder.str();
            }

            else if (token.compare("o") == 0)
            {
                std::ostringstream strBuilder;
                while (lineStream >> token)
                    strBuilder << token << ' ';

                model->m_Name = strBuilder.str();
            }

            else if (token.compare("v") == 0)
            {
                lineStream >> token; vec3.x = stof(token);
                lineStream >> token; vec3.y = stof(token);
                lineStream >> token; vec3.z = stof(token);
                model->m_Vertexes.push_back(vec3);
                // std::cout << "Vertex: " << vec3 << std::endl;
            }
            
            else if (token.compare("vn") == 0)
            {
                lineStream >> token; vec3.x = stof(token);
                lineStream >> token; vec3.y = stof(token);
                lineStream >> token; vec3.z = stof(token);
                model->m_Normals.push_back(vec3);
                // std::cout << "Normal: " << vec3 << std::endl;
            }
            
            else if (token.compare("vt") == 0)
            {
                lineStream >> token; vec2.x = stof(token);
                lineStream >> token; vec2.y = stof(token);
                model->m_Textures.push_back(vec2);
                // std::cout << "Texture: " << vec2 << std::endl;
            }

            else if (token.compare("f") == 0)
            {
                uint32_t findex = 0;
                std::string faceIntToken;
                while (lineStream >> token)
                {
                    std::istringstream faceIntStream (token);
                    while (getline(faceIntStream, faceIntToken, '/'))
                        faceIntVector[findex++] = std::stoi(faceIntToken);
                }
                model->m_Faces.push_back(faceIntVector);
            }
        }

        stream.close();
        return model;
    }
    
    void ModelOBJ::Save(const char* path) const
    {
        std::ofstream stream (path);
        if (!stream.is_open())
        {
            std::cout << "Error: Fail To Load File At Path " << path << std::endl;
            return ;
        }

        stream << m_Info << '\n';
        stream << "o " << m_Name << '\n';

        for (auto vertex: m_Vertexes)
            stream << "v " << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
        
        for (auto normal: m_Normals)
            stream << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';

        for (auto texture: m_Textures)
            stream << "vt " << texture.x << ' ' << texture.y << '\n';

        for (auto face: m_Faces)
            stream << "f " << face[0] << '/' << face[1] << '/' << face[2] << ' ' << face[3] << '/' << face[4] << '/' << face[5] << ' ' << face[6] << '/' << face[7] << '/' << face[8] << '\n';

        stream.close();
    }
    


    std::ostream& operator<<(std::ostream& stream, const ModelOBJ& model)
    {
        stream << model.m_Info << '\n';
        stream << "o " << model.m_Name << '\n';

        for (auto vertex: model.m_Vertexes)
            stream << "v " << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
        
        for (auto normal: model.m_Normals)
            stream << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';

        for (auto texture: model.m_Textures)
            stream << "vt " << texture.x << ' ' << texture.y << '\n';

        for (auto face: model.m_Faces)
            stream << "f " << face[0] << '/' << face[1] << '/' << face[2] << ' ' << face[3] << '/' << face[4] << '/' << face[5] << ' ' << face[6] << '/' << face[7] << '/' << face[8] << '\n';

        return stream;
    }
}