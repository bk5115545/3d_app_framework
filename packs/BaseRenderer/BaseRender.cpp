#include <memory>

#include "event_system/Subscriber.h"
#include "event_system/Dispatcher.h"

class BaseRender {
    class BaseRenderStaticInit {
        BaseRenderStaticInit() {
            Subscriber* init_subscriber = new Subscriber(this);
            init_subscriber->method = std::bind(&BaseRenderStaticInit::Init, this, std::placeholders:_1);
            Dispatcher::GetInstance()->AddEventSubscriber(init_subscriber, "EVENT_RENDERER_INIT");

            std::cout << "IN BASE_RENDERER" << std::endl;
        }

        void Init(std::shared_ptr<void> event_data) {
            //========================================
            // Base Game Constants
            //========================================
            uint32 screen_width = 800;
            uint32 screen_height = 600;

            //========================================
            // Construct Graphical Device
            //========================================
            Renderer* renderer = nullptr;
            renderer = new OpenGLRenderer(screen_width, screen_height);

            if (!renderer->Initialize()) {
                printf("Graphics Device could not initialize!");
                exit(1);
            }

            Dispatcher::GetInstance()->DispatchImmediate("EVENT_RENDER_INIT_SUCCESS", std::make_shared<Renderer>(renderer));
        }
    };

    friend class BaseRenderStaticInit;
 private:
        static BaseRender::BaseRenderStaticInit init;
};
BaseRender BASE_RENDERER_NORMAL_NAME;
