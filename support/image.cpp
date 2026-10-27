// Windows PNG decoder supplied by the course; no student implementation needed.
#define NOMINMAX
#include <windows.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <filesystem>
#include <limits>
#include "course.hpp"

namespace course {
namespace {
void checkImageResult(HRESULT result, const std::string& path) {
    require(SUCCEEDED(result), "Cannot decode PNG: " + path);
}
struct ComScope {
    HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    ~ComScope() { if (SUCCEEDED(result)) CoUninitialize(); }
};
}

Image loadPng(const std::string& path) {
    using Microsoft::WRL::ComPtr;
    ComScope com;
    require(SUCCEEDED(com.result) || com.result == RPC_E_CHANGED_MODE,
            "Cannot initialize Windows image decoder");
    ComPtr<IWICImagingFactory> factory;
    checkImageResult(CoCreateInstance(CLSID_WICImagingFactory, nullptr,
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(factory.GetAddressOf())), path);
    ComPtr<IWICBitmapDecoder> decoder;
    const auto filename = std::filesystem::u8path(path);
    checkImageResult(factory->CreateDecoderFromFilename(filename.c_str(), nullptr,
        GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf()), path);
    ComPtr<IWICBitmapFrameDecode> frame;
    checkImageResult(decoder->GetFrame(0, frame.GetAddressOf()), path);
    UINT width=0, height=0;
    checkImageResult(frame->GetSize(&width, &height), path);
    const auto bytes = static_cast<unsigned long long>(width)*height*3;
    require(width>0 && height>0 && width<=INT_MAX && height<=INT_MAX &&
        bytes<=std::numeric_limits<UINT>::max(), "Invalid image size: " + path);
    ComPtr<IWICFormatConverter> converter;
    checkImageResult(factory->CreateFormatConverter(converter.GetAddressOf()), path);
    // This course uses opaque RGB textures; alpha is not part of A4.
    checkImageResult(converter->Initialize(frame.Get(), GUID_WICPixelFormat24bppRGB,
        WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom), path);
    Image image;
    image.width=static_cast<int>(width);
    image.height=static_cast<int>(height);
    image.rgb.resize(static_cast<std::size_t>(bytes));
    checkImageResult(converter->CopyPixels(nullptr, width*3,
        static_cast<UINT>(bytes), image.rgb.data()), path);
    return image;
}
} // namespace course
