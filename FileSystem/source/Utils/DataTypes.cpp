#include "precompiled.hpp"
#include "FileSystem/Utils/DataTypes.hpp"

namespace FileSystem
{

std::ostream& operator<<(std::ostream& os, const Vector2& vector)
{
    os << vector.x << ' ' << vector.y;
    return os; 
}

std::ostream& operator<<(std::ostream& os, const Vector3& vector)
{
    os << vector.x << ' ' << vector.y << ' ' << vector.z;
    return os; 
}

std::ostream& operator<<(std::ostream& os, const Vector4& vector)
{
    os << vector.x << ' ' << vector.y << ' ' << vector.z << vector.w;
    return os; 
}


std::ostream& operator<<(std::ostream& os, const Color32& color)
{
    os << "(" << (int) color.red << "," << (int) color.green << "," << (int) color.blue << "," << (int) color.alpha << ") [" << color.integer << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Color64& color)
{
    os << "(" << (int) color.red << "," << (int) color.green << "," << (int) color.blue << "," << (int) color.alpha << ") [" << color.integer << "]";
    return os;
}

}
