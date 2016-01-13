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

bool ready = false;

InputDevice* input_device = nullptr;

void app_ready(std::shared_ptr<void> event_data) {
    UNUSED(event_data);
    ready = true;
}

void main_loop() {
    //========================================
    // Initialize the random number generator
    //========================================
    srand((unsigned int)time(NULL));

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // Update the Input Device with the Event
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
              Dispatcher::GetInstance()->DispatchEvent("EVENT_INPUT_NEW", std::make_shared<SDL_Event>(event));
            }
        }
        Dispatcher::GetInstance()->DispatchEvent("EVENT_APP_RUN", std::make_shared<SDL_Event>(event));
    }
}

void all_shutdown(std::shared_ptr<void> event_data) {
    UNUSED(event_data);
    //========================================
    // Clean-up
    //========================================
    Dispatcher::GetInstance()->Terminate();

    if (input_device != nullptr) {
        delete input_device;
        input_device = nullptr;
    }

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    UNUSED(argc); UNUSED(argv);
    SDL_Init(0);

    Dispatcher::GetInstance()->DispatchImmediate("EVENT_RENDERER_INIT", std::shared_ptr<void>(nullptr));

    Subscriber* ready_subscriber = new Subscriber(nullptr);
    ready_subscriber->method = std::bind(&app_ready, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(ready_subscriber, "EVENT_APP_READY");

    Subscriber* shutdown_subscriber = new Subscriber(nullptr);
    shutdown_subscriber->method = std::bind(&all_shutdown, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(shutdown_subscriber, "EVENT_ALL_SHUTDOWN");

    SDL_InitSubSystem(SDL_INIT_EVENTS);

    //========================================
    // Construct Input Device
    //========================================
    input_device = new InputDevice();
    if (!input_device->Initialize()) {
        printf("Input Device could not initialize!");
        exit(1);
    }

    while(!ready) {
        sleep(100);
        std::cout << "Waiting on pack to submit EVENT_APP_READY" << std::endl;
    }
    main_loop();

    Dispatcher::GetInstance()->DispatchImmediate("EVENT_ALL_SHUTDOWN", std::shared_ptr<void>(nullptr));
    while(Dispatcher::GetInstance()->QueueSize() > 0) {
        sleep(600);
    }

    return 0;
}
