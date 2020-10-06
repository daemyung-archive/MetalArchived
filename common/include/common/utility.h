//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#ifndef UTILITY_H_
#define UTILITY_H_

#include <cstdint>
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

#endif
