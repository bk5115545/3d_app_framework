#include <memory>

#include "event_system/Dispatcher.h"
#include "event_system/Subscriber.h"

class BaseApp {
    class BaseAppStaticInit {
        App* app;
        BaseAppStaticInit() {
            Subscriber* init_subscriber = new Subscriber(this);
            init_subscriber->method = std::bind(&BaseAppStaticInit::Init, this, std::placeholders::_1);
            Dispatcher::GetInstance()->AddEventSubscriber(init_subscriber, "EVENT_RENDER_INIT_SUCCESS");

            Subscriber* run_subscriber = new Subscriber(this);
            run_subscriber->method = std::bind(&BaseAppStaticInit::Run, this, std::placeholders::_1);
            Dispatcher::GetInstance()->AddEventSubscriber(run_subscriber, "EVENT_APP_RUN");
        }

        void Init(std::shared_ptr<void> event_data) {
            //========================================
            // Construct Game
            //========================================
            app = new App3D((Renderer*)event_data.get());
            if (!app->Initialize()) {
                printf("Game could not Initialize!");
                exit(1);  // this case will leak a lot of memory...
                          // should properly do destructor calls and proper shutdown
            }

            Dispatcher::GetInstance()->DispatchImmediate("EVENT_APP_INIT_SUCCESS", std::make_shared<void>(app));
        }

        void Run(std::shared_ptr<void> unimportant) {
            // UNUSED(unimportant);
            app->Run();
        }

    };

    friend class BaseAppStaticInit;
 private:
    static BaseApp::BaseAppStaticInit init;
};

BaseApp BASE_APP_NORMAL_NAME;
