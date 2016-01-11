#include <memory>
#include <string>

#include "event_system/Dispatcher.h"
#include "event_system/Subscriber.h"

class BaseLoader {
    class BaseLoaderStaticInit {
        BaseLoaderStaticInit() {
            Subscriber* init_subscriber = new Subscriber(this);
            init_subscriber->method = std::bind(&BaseLoaderStaticInit::Init, this, std::placeholders::_1);
            Dispatcher::GetInstance()->AddEventSubscriber(init_subscriber, "EVENT_APP_INIT_SUCCESS");

            std::cout << "IN BASE_LOADER" << std::endl;
        }

        void Init(std::shared_ptr<void> event_data) {
            //========================================
            // Load Level
            //========================================
            App3D* app = (App3D*)event_data.get();
            app->Reset();
            std::string level_config_file = "foobar.xml";
            if (!app->LoadLevel(level_config_file)) {
                printf("Game could not load level %s: ",
                       level_config_file.c_str());
                exit(1);  // this case will leak a lot of memory...
                          // should properly do destructor calls and proper shutdown
            }

            Dispatcher::GetInstance()->DispatchImmediate("EVENT_LOADER_INIT_SUCCESS", std::make_shared<void>(app));
        }

    };

    friend class BaseLoaderStaticInit;
 private:
    static BaseLoader::BaseLoaderStaticInit init;
};

BaseLoader BASE_LOADER_NORMAL_NAME;
