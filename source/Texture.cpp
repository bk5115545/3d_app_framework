#include <iostream>
#include <SDL_image.h>
#include "Texture.h"
// #include "GameUtils.h"

Texture::Texture() { texture_ = nullptr; }

Texture::~Texture() { SDL_DestroyTexture(texture_); }

bool Texture::Initialize(SDL_Renderer* renderer,
                         View* view,
                         std::string image_path) {
    renderer_ = renderer;
    view_ = view;

    // Load image
    SDL_Surface* surface = nullptr;
    surface = IMG_Load(image_path.c_str());
    if (surface == NULL) {
        std::cerr << "Surface failed: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture_ == NULL) {
        // LogSDLError(std::cerr, "LoadTexture");
        std::cerr << "Texture failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Texture::Render(Vector2 position, float32 angle, SDL_Rect sprite_clip) {
    Vector2 view_position = view_->transform();

    SDL_Rect render_destination;
    render_destination.x = position.x + view_position.x;
    render_destination.y = position.y + view_position.y;
    render_destination.w = sprite_clip.w * 2;
    render_destination.h = sprite_clip.h * 2;

    // SDL_QueryTexture(texture_, NULL, NULL, &render_destination.w, &render_destination.h);

    SDL_RenderCopyEx(renderer_, texture_, &sprite_clip, &render_destination, angle, NULL, SDL_FLIP_NONE);
}

void Texture::GetDimensions(int32* w, int32* h) {
    SDL_QueryTexture(texture_, NULL, NULL, w, h);
    return;
}
