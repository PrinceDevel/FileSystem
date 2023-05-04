#pragma once
#include <iostream>

namespace FileSystem
{

struct Vector2
{
    union {
        struct { float x, y; };
        float value[2];
    };
    Vector2 () : x(0), y(0) {}
    Vector2 (float _x, float _y) : x(_x), y(_y) {}
    Vector2 (float* values) : x(values[0]), y(values[1]) {}
    friend std::ostream& operator<<(std::ostream&, const Vector2&);
};

struct Vector3
{
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
        float value[3];
    };
    Vector3 () : x(0), y(0), z(0) {}
    Vector3 (float _x, float _y, float _z) : x(_x), y(_y), z(_y) {}
    Vector3 (float* values) : x(values[0]), y(values[1]), z(values[2]) {}
    friend std::ostream& operator<<(std::ostream&, const Vector3&);
};

struct Vector4
{
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float value[4];
    };
    Vector4 () : x(0), y(0), z(0), w(0) {}
    Vector4 (float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_y), w(_w) {}
    Vector4 (float* values) : x(values[0]), y(values[1]), z(values[2]), w(values[3]) {}
    friend std::ostream& operator<<(std::ostream&, const Vector4&);
};


typedef Vector4 Quaternion;
typedef Vector3 ColorRGB;   // 96 bits
typedef Vector4 ColorRGBA;  // 128 bits


struct Color32
{
    union {
        struct { uint8_t red, green, blue, alpha; };
        uint32_t integer;
        float    real;
        uint8_t  values[4];
    };
    Color32 () : integer(0) {}
    Color32 (uint32_t value) : integer(value) {}
    Color32 (float value)    : real(value) {}
    Color32 (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = UINT8_MAX) : red(red), green(green), blue(blue), alpha(alpha) {}
    friend std::ostream& operator<<(std::ostream&, const Color32&);
};
struct Color64
{
    union {
        struct { uint16_t red, green, blue, alpha; };
        uint64_t integer;
        double   real;
        uint16_t values[4];
    };
    Color64 () : integer(0) {}
    Color64 (uint64_t value) : integer(value) {}
    Color64 (double   value) : real(value) {}
    Color64 (uint16_t red, uint16_t green, uint16_t blue, uint16_t alpha = UINT16_MAX) : red(red), green(green), blue(blue), alpha(alpha) {}
    friend std::ostream& operator<<(std::ostream&, const Color64&);
};

}