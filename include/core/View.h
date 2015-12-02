#pragma once

#include "core/InputDevice.h"
#include "util/definitions.h"

class View {
protected:
    InputDevice* input_device_;
    Vector2 position_;
    uint32 velocity_;

public:
    View();
    ~View();
    bool Initialize(InputDevice*, float32, float32);
    void Update(float32);
    Vector2 position();
};
