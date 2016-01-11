// Copyright 2015 Casey Megginson and Blaise Koch

#include <ctime>
#include <iostream>
#include <string>

// Media Libraries
#include "SDL.h"

// Physics

// Threaded Events
#include "event_system/Dispatcher.h"

// Project Libraries
#include "util/definitions.h"
#include "core/app_3d.h"
#include "core/input_device.h"

// Plugable Renderers
#include "render/renderer.h"
#include "render/opengl/opengl_renderer.h"

int main(int argc, char* argv[]) {
    UNUSED(argc); UNUSED(argv);
    SDL_Init(0);

    Dispatcher* dispatch = Dispatcher::GetInstance();
    dispatch->DispatchImmediate("EVENT_RENDERER_INIT", std::shared_ptr<void>(nullptr));
    //========================================
    // Initialize the random number generator
    //========================================
    srand((unsigned int)time(NULL));

    //========================================
    // Construct Input Device
    //========================================
    InputDevice* input_device = new InputDevice();
    if (!input_device->Initialize()) {
        printf("Input Device could not initialize!");
        exit(1);
    }

    SDL_InitSubSystem(SDL_INIT_EVENTS);
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // Update the Input Device with the Event
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
              dispatch->DispatchEvent("EVENT_INPUT_NEW", std::make_shared<SDL_Event>(event));
            }
        }
        dispatch->DispatchEvent("EVENT_APP_RUN", std::make_shared<SDL_Event>(event));
    }

    //========================================
    // Clean-up
    //========================================
    Dispatcher::GetInstance()->Terminate();

    if (input_device != nullptr) {
        delete input_device;
        input_device = nullptr;
    }

    SDL_Quit();

    return 0;
}
