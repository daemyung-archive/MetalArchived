//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#ifndef UTILITY_H_
#define UTILITY_H_

#include <Metal/Metal.h>
#include <simd/simd.h>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <tuple>
#include <filesystem>

//----------------------------------------------------------------------------------------------------------------------

using Resolution = std::tuple<uint32_t, uint32_t>;

//----------------------------------------------------------------------------------------------------------------------

//! Retrieve a width from a resolution.
//! \param resolution A resolution.
//! \return A width.
inline auto GetWidth(const Resolution &resolution) {
    return std::get<0>(resolution);
}

//----------------------------------------------------------------------------------------------------------------------

//! Retrieve a height from a resolution.
//! \param resolution A resolution.
//! \return A height.
inline auto GetHeight(const Resolution &resolution) {
    return std::get<1>(resolution);
}

//----------------------------------------------------------------------------------------------------------------------

//! Retrieve an aspect ratio of a resolution.
//! \param resolution A resolution.
//! \return An aspect ratio.
inline auto GetAspectRatio(const Resolution &resolution) {
    return GetWidth(resolution) / static_cast<float>(GetHeight(resolution));
}

//----------------------------------------------------------------------------------------------------------------------

//! Read a file.
//! \param path A file path.
//! \return The content of a file.
extern std::string ReadFile(const std::filesystem::path &path);

//----------------------------------------------------------------------------------------------------------------------

//! Compile a shader.
//! \param A Metal device.
//! \param file_path The file path that contains the shader code.
//! \param entrypoint The name of shader entrypoint function where shader execution begin.
//! \return A handle to MTLFunction.
extern id<MTLFunction> CompileShader(id<MTLDevice> device, const std::filesystem::path &file_path,
                                     const std::string &entrypoint);

//----------------------------------------------------------------------------------------------------------------------

template<typename T>
constexpr auto ConvertToRadians(T degree) {
    return (degree / 180.0) * M_PI;
}

//----------------------------------------------------------------------------------------------------------------------

template<typename T>
constexpr bool NearEqual(T lhs, T rhs, T epsilon) {
    return abs(lhs - rhs) < epsilon;
}

//----------------------------------------------------------------------------------------------------------------------

#endif
