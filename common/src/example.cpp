//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#include "example.h"

#include <imgui_impl_osx.h>
#include <imgui_impl_metal.h>

#include "window.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------------------------------------------------

Example::Example(const std::string &title) :
_title(title) {
    InitDevice();
    InitCommandQueue();
    InitSemaphore();
    InitImGui();
}

//----------------------------------------------------------------------------------------------------------------------

Example::~Example() {
    TermImGui();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::BindToWindow(Window *window) {
    _layer = (CAMetalLayer*)[window->GetView() layer];
    
    // 
    _layer.device = _device;
    _layer.maximumDrawableCount = kMetalLayerDrawableCount;
    _layer.pixelFormat = kMetalLayerPixelFormat;
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Init() {
    _timer.Start();
    OnInit();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Term() {
    OnTerm();
    _timer.Stop();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Resize(const Resolution &resolution) {
    @synchronized(_layer) {
        _camera.SetAspectRatio(GetAspectRatio(resolution));

        // Resize a layer drawable size.
        _layer.drawableSize = CGSizeMake(GetWidth(resolution), GetHeight(resolution));

        // Update the display size to ImGui.
        ImGui::GetIO().DisplaySize = ImVec2(GetWidth(resolution), GetHeight(resolution));

        // Resize by an example.
        OnResize(resolution);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Update() {
    _timer.Tick();

    // Calculate FPS.
    auto elapsed_time = _timer.GetElapsedTime();
    if (elapsed_time - _fps_time > 1s) {
        _fps = _cps;
        _fps_time = elapsed_time;
        _cps = 0;
    } else {
        ++_cps;
    }

    // Wait until the command buffer has completed its work.
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    
    // Advance the current frame index.
    _frame_index = ++_frame_index % kMetalLayerDrawableCount;
    
    // Update ImGui by an example.
    BeginImGuiPass();
    OnUpdate(_frame_index);
    EndImGuiPass();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Render() {
    @synchronized(_layer) {
        // Acquire a next drawable.
        _drawable = [_layer nextDrawable];

        // Render by an example.
        _command_buffer = [_command_queue commandBuffer];
        OnRender(_frame_index);

        // Schedule a drawable presentation.
        [_command_buffer presentDrawable:_drawable];
        _drawable = nil;

        // Signal a semaphore after the command buffer has processed.
        __weak dispatch_semaphore_t semaphore = _semaphore;
        [_command_buffer addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer) {
            dispatch_semaphore_signal(semaphore);
        }];

        // Commit a command buffer.
        [_command_buffer commit];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Example::OnMouseButtonDown(NSEvent* event, const NSPoint &point) {
    _mouse_point = point;
}

//----------------------------------------------------------------------------------------------------------------------

void Example::OnMouseButtonUp(NSEvent* event, const NSPoint &point) {
    _mouse_point = point;
}

//----------------------------------------------------------------------------------------------------------------------

void Example::OnMouseMove(NSEvent* event, const NSPoint &point) {
    if (event.type == NSEventTypeLeftMouseDragged) {
        _camera.RotateBy({static_cast<float>(point.x - _mouse_point.x),
                          static_cast<float>(point.y - _mouse_point.y)});
    }
    _mouse_point = point;
}

//----------------------------------------------------------------------------------------------------------------------

void Example::OnMouseWheel(float delta) {
    _camera.ZoomBy(delta);
}

//----------------------------------------------------------------------------------------------------------------------

void Example::RecordDrawImGuiCommands(MTLRenderPassDescriptor *descriptor, id<MTLRenderCommandEncoder> encoder) {
    ImGui_ImplMetal_NewFrame(descriptor);
    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), _command_buffer, encoder);
}

//----------------------------------------------------------------------------------------------------------------------

void Example::InitDevice() {
    _device = MTLCreateSystemDefaultDevice();
    if (!_device) {
        throw std::runtime_error("Fail to create a device.");
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Example::InitCommandQueue() {
    _command_queue = [_device newCommandQueue];
    if (!_command_queue) {
        throw std::runtime_error("Fail to create a command queue.");
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Example::InitSemaphore() {
    _semaphore = dispatch_semaphore_create(kMetalLayerDrawableCount);
    if (!_semaphore) {
        throw std::runtime_error("Fail to create a semaphore");
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Example::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Use the classic theme.
    ImGui::StyleColorsClassic();

    ImGui_ImplMetal_Init(_device);
    ImGui_ImplOSX_Init();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::TermImGui() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::BeginImGuiPass() {
    ImGui_ImplOSX_NewFrame(nullptr);
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::Begin("Metal", nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::TextUnformatted(_title.c_str());
    ImGui::TextUnformatted(_device.name.UTF8String);
    ImGui::Text("%.2f ms/frame(%u FPS)", _timer.GetDeltaTime().count(), _fps);
}

//----------------------------------------------------------------------------------------------------------------------

void Example::EndImGuiPass() {
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::EndFrame();
}

//----------------------------------------------------------------------------------------------------------------------
