//
// This file is part of the "DirectX12" project
// See "LICENSE" for license information.
//

#include "utility.h"

#include <fmt/format.h>
#include <fstream>

//----------------------------------------------------------------------------------------------------------------------

std::string ReadFile(const std::filesystem::path &path) {
    std::basic_ifstream<char> fin(path, std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        throw std::runtime_error(fmt::format("Fail to open {}.", path.string()));
    }
    return std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
}

//----------------------------------------------------------------------------------------------------------------------

id<MTLFunction> CompileShader(id<MTLDevice> device, const std::filesystem::path &file_path,
                              const std::string &entrypoint) {
    auto source = ReadFile(file_path);

    // Create a library.
    NSError* error;
    auto library = [device newLibraryWithSource:@(source.data()) options:nullptr error:&error];
    if (error) {
        throw std::runtime_error(fmt::format("Fail to create a library: {}.", error.description.UTF8String));
    }

    // Create a function.
    auto function = [library newFunctionWithName:@(entrypoint.data())];
    if (!function) {
        throw std::runtime_error(fmt::format("Fail to create a {} function.", entrypoint));
    }

    return function;
}

//----------------------------------------------------------------------------------------------------------------------
