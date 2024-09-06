#include "WindowManager.h"
#include "PhysicsEngine.h"

// constructor
WindowManager::WindowManager() : window(nullptr), renderer(nullptr), windowWidth(800), windowHeight(600), m_windowExists(false) {}

// destructor
WindowManager::~WindowManager() {
    close();
}

bool WindowManager::init(int screen_width, int screen_height) {
    windowWidth = screen_width;
    windowHeight = screen_height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // address SDL bug where windowed display doesn't fit screen if exceeds or even if exactly matches desktop resolution
    SDL_DisplayMode desktopMode;
    if (SDL_GetDesktopDisplayMode(0, &desktopMode) != 0) {
        std::cerr << "Could not get desktop display mode! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (screen_width > desktopMode.w) {
        screen_width = desktopMode.w;
    }
    if (screen_height > desktopMode.h) {
        screen_height = desktopMode.h - 50;
    }

    window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue background
    SDL_SetWindowSize(window, screen_width, screen_height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    m_windowExists = true;
    return true;
}

void WindowManager::updateWindow(Game& game) {
    PhysicsEngine physics = game.getPhysicsEngine();

    if (renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        const auto& shapes = game.getShapes();

        for (const auto& pair : shapes) {
            Shape* shape = pair.second;
            int renderWidth = shape->getWidth();
            int renderHeight = shape->getHeight();
            if (game.isProportionalMode()) {
                //std::cout << getWindowWidth() << " " << getWindowHeight() << std::endl;
                renderWidth = (float(renderWidth) / 800) * getWindowWidth();
                renderHeight = (float(renderHeight) / 600) * getWindowHeight();
                //std::cout << renderWidth << " " << renderHeight << std::endl;
            }
            SDL_Rect rect = { static_cast<int>(shape->getX()), static_cast<int>(shape->getY()), renderWidth, renderHeight };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red color for shapes
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
    }
}

void WindowManager::close() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
    m_windowExists = false;
}

bool WindowManager::windowExists() const {
    return m_windowExists;
}

int WindowManager::getWindowWidth() const {
    return windowWidth;
}

int WindowManager::getWindowHeight() const {
    return windowHeight;
}

void WindowManager::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    SDL_SetWindowSize(window, windowWidth, windowHeight);
}
