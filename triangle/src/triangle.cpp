//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#include <fmt/format.h>
#include <common/window.h>
#include <common/example.h>
#include <simd/simd.h>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------

struct Vertex {
    vector_float3 position;
    vector_float3 color;
};

//----------------------------------------------------------------------------------------------------------------------

struct Transforms {
    simd::float4x4 projection;
    simd::float4x4 view;
    simd::float4x4 model;
};

//----------------------------------------------------------------------------------------------------------------------

struct Options {
    bool use_staging_buffer = true;
};

//----------------------------------------------------------------------------------------------------------------------

inline auto BuildFilePath(const std::string &file_name) {
    std::filesystem::path file_path;

    file_path = fmt::format("{}/{}", TRIANGLE_ASSET_DIR, file_name);
    if (file_path.has_filename()) {
        return file_path;
    }

    throw std::runtime_error(fmt::format("File isn't exist: {}.", file_name));
}

//----------------------------------------------------------------------------------------------------------------------

class Triangle : public Example {
public:
    Triangle() :
        Example("Triangle") {
        InitResources();
        InitPipelines();
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
        if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Use staging buffer", &_options.use_staging_buffer)) {
                InitResources();
            }
        }

        // Transformations transforms;
        _transforms.projection = _camera.GetProjection();
        _transforms.view = _camera.GetView();
        _transforms.model = matrix_identity_float4x4;
    }

    void OnRender(uint32_t index) override {
        auto desc = [MTLRenderPassDescriptor new];
        desc.colorAttachments[0].texture = _drawable.texture;
        desc.colorAttachments[0].loadAction = MTLLoadActionClear;
        desc.colorAttachments[0].storeAction = MTLStoreActionStore;
        desc.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.2, 1.0);

        auto encoder = [_command_buffer renderCommandEncoderWithDescriptor:desc];
        [encoder setViewport:_viewport];
        [encoder setScissorRect:_scissor_rect];
        [encoder setVertexBuffer:_vertex_buffer offset:0 atIndex:0];
        [encoder setVertexBytes:&_transforms length:sizeof(Transforms) atIndex:1];
        [encoder setRenderPipelineState:_pipeline_state];
        [encoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:3
                             indexType:MTLIndexTypeUInt16 indexBuffer:_index_buffer
                     indexBufferOffset:0];

        RecordDrawImGuiCommands(desc, encoder);

        [encoder endEncoding];
    }

private:
    void InitResources() {
        // Define vertices.
        Vertex vertices[3] = {{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                              {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                              {{ 0.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

        // Define indices.
        uint16_t indices[3] = {0, 1, 2};

        if (_options.use_staging_buffer) {
            auto command_buffer = [_command_queue commandBuffer];
            auto blit_encoder = [command_buffer blitCommandEncoder];

            auto staging_buffer = [_device newBufferWithBytes:vertices length:sizeof(vertices)
                                                      options:MTLResourceStorageModeShared];
            _vertex_buffer = [_device newBufferWithLength:sizeof(vertices) options:MTLResourceStorageModePrivate];
            [blit_encoder copyFromBuffer:staging_buffer sourceOffset:0
                                toBuffer:_vertex_buffer destinationOffset:0 size:sizeof(vertices)];

            staging_buffer = [_device newBufferWithBytes:indices length:sizeof(indices)
                                                 options:MTLResourceStorageModeShared];
            _index_buffer = [_device newBufferWithLength:sizeof(indices) options:MTLResourceStorageModePrivate];
            [blit_encoder copyFromBuffer:staging_buffer sourceOffset:0
                                toBuffer:_index_buffer destinationOffset:0 size:sizeof(indices)];

            [blit_encoder endEncoding];
            [command_buffer commit];
        } else {
            _vertex_buffer = [_device newBufferWithBytes:vertices length:sizeof(vertices)
                                                 options:MTLResourceStorageModeShared];

            _index_buffer = [_device newBufferWithBytes:indices length:sizeof(indices)
                                                options:MTLResourceStorageModeShared];
        }
    }

    void InitPipelines() {
        auto vertex_descriptor = [MTLVertexDescriptor new];
        vertex_descriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertex_descriptor.attributes[0].offset = 0;
        vertex_descriptor.attributes[0].bufferIndex = 0;
        vertex_descriptor.attributes[1].format = MTLVertexFormatFloat3;
        vertex_descriptor.attributes[1].offset = offsetof(Vertex, color);
        vertex_descriptor.attributes[1].bufferIndex = 0;
        vertex_descriptor.layouts[0].stride = sizeof(Vertex);
        vertex_descriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertex_descriptor.layouts[0].stepRate = 1;

        auto descriptor = [MTLRenderPipelineDescriptor new];
        descriptor.vertexFunction = CompileShader(_device, BuildFilePath("pass_through.metal"), "VSMain");
        descriptor.fragmentFunction = CompileShader(_device, BuildFilePath("pass_through.metal"), "FSMain");
        descriptor.vertexDescriptor = vertex_descriptor;
        descriptor.rasterSampleCount = 1;
        descriptor.colorAttachments[0].pixelFormat = kMetalLayerPixelFormat;
        descriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassTriangle;

        NSError *error;
        _pipeline_state = [_device newRenderPipelineStateWithDescriptor:descriptor error:&error];

        if (!_pipeline_state) {
            throw std::runtime_error(fmt::format("Fail to create a pipeline state: {}", error.description.UTF8String));
        }
    }

private:
    Options _options;
    id<MTLBuffer> _vertex_buffer;
    id<MTLBuffer> _index_buffer;
    id<MTLRenderPipelineState> _pipeline_state;
    MTLViewport _viewport = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    MTLScissorRect _scissor_rect = {0, 0, 0, 0};
    Transforms _transforms = {};
};

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    @autoreleasepool {
        try {
            auto example = std::make_unique<Triangle>();
            Window::GetInstance()->MainLoop(example.get());
        }
        catch (const std::exception &exception) {
            std::cerr << exception.what() << std::endl;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
