#pragma once

#include "FileSystem/Utils/DataTypes.hpp"
#include <vector>

namespace FileSystem
{
    struct TriangularData
    {
        Vector3 normal, first, second, third;

        TriangularData() {}
        friend std::ostream& operator<<(std::ostream&, const TriangularData&);
    };

    class ModelSTL
    {
    public:
        std::string m_Label = "No Label";
        std::vector<TriangularData> m_Faces; // Triangular Faces

        ModelSTL();
        ~ModelSTL();

        void SaveASCII (const char* path);
        void SaveBinary (const char* path);
        
        static ModelSTL* LoadASCII (const char* path);
        static ModelSTL* LoadBinary (const char* path);

        friend std::ostream& operator<<(std::ostream&, const ModelSTL&);
    };

}
