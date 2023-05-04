#include <iostream>
#include <memory>
#include "FileSystem/2D/Image.hpp"

using namespace FileSystem;

int main()
{
    std::unique_ptr<Image> image (ImageQOI::Load("resource/QOI/Image20x20.qoi"));
    ImageQOI::Save(image.get(), "resource/QOI/Image20.qoi");

    return 0;
}
