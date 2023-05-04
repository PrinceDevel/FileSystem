#pragma once

#include "FileSystem/Utils/DataTypes.hpp"
#include <vector>

namespace FileSystem
{
    class ModelOBJ
    {
    public:
        std::string m_Name = "Unnamed";
        std::string m_Info = "# Exported using FileSystem by PrinceDevel";

        ModelOBJ();
        ~ModelOBJ();

        std::vector<Vector3> m_Vertexes;
        std::vector<Vector3> m_Normals;
        std::vector<Vector2> m_Textures;
        std::vector<std::vector<uint32_t>> m_Faces;

        static ModelOBJ* Load (const char* path);
        void Save (const char* path) const;


        friend std::ostream& operator<<(std::ostream&, const ModelOBJ&);
    };

}
