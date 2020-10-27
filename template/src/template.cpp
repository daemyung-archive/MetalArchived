//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#include <common/window.h>
#include <common/example.h>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------

const auto kLightSteelBlue = MTLClearColorMake(0.69, 0.77, 0.87, 1.0);

//----------------------------------------------------------------------------------------------------------------------

class Template : public Example {
public:
    Template() :
        Example("Template") {
    }

protected:
    void OnInit() override {
    }

    void OnTerm() override {
    }

    void OnResize(const Resolution &resolution) override {
        // Update a viewport.
        _viewport.width = static_cast<double>(GetWidth(resolution));
        _viewport.height = static_cast<double>(GetHeight(resolution));

        // Update a scissor rect.
        _scissor_rect.width = GetWidth(resolution);
        _scissor_rect.height = GetHeight(resolution);
    }

    void OnUpdate(uint32_t index) override {
    }

    void OnRender(uint32_t index) override {
        auto desc = [MTLRenderPassDescriptor new];
        desc.colorAttachments[0].texture = _drawable.texture;
        desc.colorAttachments[0].loadAction = MTLLoadActionClear;
        desc.colorAttachments[0].storeAction = MTLStoreActionStore;
        desc.colorAttachments[0].clearColor = kLightSteelBlue;

        auto encoder = [_command_buffer renderCommandEncoderWithDescriptor:desc];
        [encoder setViewport:_viewport];
        [encoder setScissorRect:_scissor_rect];

        RecordDrawImGuiCommands(desc, encoder);

        [encoder endEncoding];
    }

private:
    MTLViewport _viewport = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    MTLScissorRect _scissor_rect = {0, 0, 0, 0};
};

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    @autoreleasepool {
        try {
            auto example = std::make_unique<Template>();
            Window::GetInstance()->MainLoop(example.get());
        }
        catch (const std::exception &exception) {
            std::cerr << exception.what() << std::endl;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
