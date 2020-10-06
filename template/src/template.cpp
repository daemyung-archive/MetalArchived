//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#include <common/window.h>
#include <iostream>

#include <thread>

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    @autoreleasepool {
        try {
            Window::GetInstance()->MainLoop(nullptr);
        }
        catch (const std::exception &exception) {
            std::cerr << exception.what() << std::endl;
        }
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
