#pragma once

#include "core/input_device.h"
#include "util/definitions.h"

class Camera {
protected:
    InputDevice* input_device_;

public:
    // virtual Camera() = 0;
    //virtual ~Camera() = 0;
    virtual bool Initialize() = 0;
    virtual void Update(std::shared_ptr<void>) = 0;
};
