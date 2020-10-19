//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <string>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>

#include "utility.h"

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
    
protected:
    std::string _title;
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
