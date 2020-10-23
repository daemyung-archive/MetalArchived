//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <imgui.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <string>

#include "window.h"
#include "utility.h"
#include "timer.h"

//----------------------------------------------------------------------------------------------------------------------

constexpr auto kMetalLayerDrawableCount = 2;
constexpr auto kMetalLayerPixelFormat = MTLPixelFormatBGRA8Unorm;

//----------------------------------------------------------------------------------------------------------------------

class Window;

//----------------------------------------------------------------------------------------------------------------------

class Example {
public:
    //! Constructor.
    //! \param title The example title.
    explicit Example(const std::string &title);
    
    //! Destructor.
    virtual ~Example();
    
    //! Bind Metal to a window.
    //! \param window A window is bounded by Metal.
    void BindToWindow(Window *window);

    //! Initialize.
    void Init();

    //! Terminate.
    void Term();

    //! Resize.
    //! \param resolution A resolution.
    void Resize(const Resolution &resolution);

    //! Update.
    void Update();

    //! Render.
    void Render();
    
protected:
    //! Record draw commands for ImGui.
    //! \param descriptor A render pass descriptor.
    //! \param encoder A render command encoder.
    void RecordDrawImGuiCommands(MTLRenderPassDescriptor *descriptor, id<MTLRenderCommandEncoder> encoder);

    //! Handle initialize event.
    virtual void OnInit() = 0;

    //! Handle terminate event.
    virtual void OnTerm() = 0;

    //! Handle resize event.
    //! \param resolution A resolution.
    virtual void OnResize(const Resolution &resolution) = 0;

    //! Handle update event.
    //! \param index The current index of swap chain image.
    virtual void OnUpdate(uint32_t index) = 0;

    //! Handle render event.
    //! \param index The current index of swap chain image.
    virtual void OnRender(uint32_t index) = 0;
    
protected:
    //! Initialize a device.
    void InitDevice();
    
    //! Initialize a command queue.
    void InitCommandQueue();
    
    //! Initialize a semaphore.
    void InitSemaphore();
    
    //! Initialize ImGui.
    void InitImGui();

    //! Terminate ImGui.
    void TermImGui();

    //! Begin ImGui pass.
    void BeginImGuiPass();

    //! End ImGui pass.
    void EndImGuiPass();

protected:
    std::string _title;
    Timer _timer;
    uint32_t _cps = 0;
    uint32_t _fps = 0;
    Timer::Duration _fps_time = Timer::Duration::zero();
    uint32_t _frame_index = 0;
    id<MTLDevice> _device;
    id<MTLCommandQueue> _command_queue;
    dispatch_semaphore_t _semaphore = nil;
    id<MTLCommandBuffer> _command_buffer;
    CAMetalLayer *_layer = nil;
    id<CAMetalDrawable> _drawable;
};

//----------------------------------------------------------------------------------------------------------------------

#endif
