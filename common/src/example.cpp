//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#include "example.h"

#include "window.h"

//----------------------------------------------------------------------------------------------------------------------

Example::Example(const std::string &title) :
_title(title) {
    InitDevice();
    InitCommandQueue();
    InitSemaphore();
}

//----------------------------------------------------------------------------------------------------------------------

Example::~Example() {
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
    OnInit();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Term() {
    OnTerm();
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Resize(const Resolution &resolution) {
    OnResize(resolution);
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Update() {
    // Wait until the command buffer has completed its work.
    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    
    // Advance the current frame index.
    _frame_index = ++_frame_index % kMetalLayerDrawableCount;
    
    // Retrieve a current drawable.
    _drawable = [_layer nextDrawable];
}

//----------------------------------------------------------------------------------------------------------------------

void Example::Render() {
    // Render by an example.
    _command_buffer = [_command_queue commandBuffer];
    OnRender(_frame_index);
    
    // Schedule a drawable presentation.
    [_command_buffer presentDrawable:_drawable];
    
    // Signal a semaphore after the command buffer has processed.
    __weak dispatch_semaphore_t semaphore = _semaphore;
    [_command_buffer addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer) {
        dispatch_semaphore_signal(semaphore);
    }];
    
    // Commit a command buffer.
    [_command_buffer commit];
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
