//
// This file is part of the "Metal" project
// See "LICENSE" for license information.
//

#ifndef WINDOW_H_
#define WINDOW_H_

#include <AppKit/AppKit.h>

#include "utility.h"

//----------------------------------------------------------------------------------------------------------------------

constexpr auto kFHDResolution = Resolution(1280, 720);

//----------------------------------------------------------------------------------------------------------------------

class Example;

//----------------------------------------------------------------------------------------------------------------------

@interface View : NSView<NSWindowDelegate> {
}

- (void)MainLoop:(Example*) example;
@end

//----------------------------------------------------------------------------------------------------------------------

class Window {
public:
    //! Retrieve a window.
    //! \return A window.
    [[nodiscard]]
    static Window* GetInstance();
    
    //! Destructor.
    ~Window();

    //! Start the main loop.
    //! \param example An example will be shown on a window.
    void MainLoop(Example *example);

    //! Retrieve a window resolution.
    //! \return A resolution of window.
    [[nodiscard]]
    Resolution GetResolution() const;

    //! Retrieve a view.
    //! \return A view.
    [[nodiscard]]
    inline auto GetView() const {
        return _view;
    }

private:
    //! Constructor.
    Window();
    
    //! Initialize an application.
    void InitApplication();

    //! Initialize a window.
    //! \param resolution A resolution of window.
    void InitWindow(const Resolution &resolution);

    //! Initialize a view.
    void InitView();

private:
    NSWindow* _window = nil;
    View* _view = nil;
};

//----------------------------------------------------------------------------------------------------------------------

#endif
