// Copyright 2015 Casey Megginson and Blaise Koch

#include <ctime>
#include <iostream>
#include <string>

// Media Libraries
#include "SDL.h"

// Threaded Events
#include "event_system/Dispatcher.h"
#include "event_system/Subscriber.h"

// Project Libraries
#include "util/definitions.h"
#include "core/app_3d.h"
#include "core/input_device.h"


class EngineCoreMinimal {
    volatile bool ready_ = false;
    App* app_ = nullptr;

 public:
     EngineCoreMinimal() {
         Subscriber* ready_subscriber = new Subscriber(this,false);
         ready_subscriber->method = std::bind(&EngineCoreMinimal::app_ready, this, std::placeholders::_1);
         Dispatcher::GetInstance()->AddEventSubscriber(ready_subscriber, "EVENT_APP_READY");

         Subscriber* shutdown_subscriber = new Subscriber(this,false);
         shutdown_subscriber->method = std::bind(&EngineCoreMinimal::all_shutdown, this, std::placeholders::_1);
         Dispatcher::GetInstance()->AddEventSubscriber(shutdown_subscriber, "EVENT_ALL_SHUTDOWN");
     }

    void app_ready(std::shared_ptr<void> event_data) {
        app_ = (App3D*)event_data.get();
        ready_ = true;
        std::cout << "APP IS READY" << std::endl;
    }

    bool ready() {
        return ready_ || app_ != nullptr;
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
            //Dispatcher::GetInstance()->DispatchEvent("EVENT_APP_RUN", std::make_shared<SDL_Event>(event));
            std::cout << app_ << std::endl;
            app_->Run();
        }
    }

    void all_shutdown(std::shared_ptr<void> event_data) {
        UNUSED(event_data);
        //========================================
        // Clean-up
        //========================================
        Dispatcher::GetInstance()->Terminate();

        SDL_Quit();
    }
};

int main(int argc, char* argv[]) {
    UNUSED(argc); UNUSED(argv);
    SDL_Init(0);
    EngineCoreMinimal engine = EngineCoreMinimal();
    Dispatcher::GetInstance()->DispatchImmediate("EVENT_RENDERER_INIT", std::shared_ptr<void>(nullptr));

    SDL_InitSubSystem(SDL_INIT_EVENTS);



    //========================================
    // Construct Input Device
    //========================================
    InputDevice* input_device = new InputDevice();
    if (!input_device->Initialize()) {
        printf("Input Device could not initialize!");
        exit(1);
    }

    sleep(1000);

    Dispatcher::GetInstance()->NonSerialProcess();
    std::cout << "Waiting on pack to submit EVENT_APP_READY" << std::endl;



    // Run the engine
    engine.main_loop();

    Dispatcher::GetInstance()->DispatchImmediate("EVENT_ALL_SHUTDOWN", std::shared_ptr<void>(nullptr));
    while(Dispatcher::GetInstance()->QueueSize() > 0) {
        sleep(600);
    }

    return 0;
}
