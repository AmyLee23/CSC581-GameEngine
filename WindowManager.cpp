#include "WindowManager.h"
#include "PhysicsEngine.h"

// constructor
WindowManager::WindowManager() : 
    window(nullptr), renderer(nullptr), windowWidth(800), windowHeight(600), 
    lastWindowWidth(windowWidth), lastWindowHeight(windowHeight), m_windowExists(false) {}

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
            shape->getDrawer().drawShape(game, shape, game.isProportionalMode());
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
    lastWindowWidth = windowWidth;
    lastWindowHeight = windowHeight;
    windowWidth = width;
    windowHeight = height;
    SDL_SetWindowSize(window, windowWidth, windowHeight);
}

void WindowManager::updateWindowSize(int width, int height, Game& game) {
    lastWindowWidth = windowWidth;
    lastWindowHeight = windowHeight;
    windowWidth = width;
    windowHeight = height;

    const auto& shapes = game.getShapes();
    for (const auto& pair : shapes) {
        Shape* shape = pair.second;

        shape->getDrawer().prepareRelativePosition(shape, game);

        if (game.isProportionalMode()) {
            shape->getDrawer().prepareScaling(shape, game);
        }
    }

    SDL_SetWindowSize(window, windowWidth, windowHeight);
}

SDL_Window* WindowManager::getWindow() {
    return window;
}

SDL_Renderer* WindowManager::getRenderer() {
    return renderer;
}

int WindowManager::getLastWindowWidth() const {
    return lastWindowWidth;
}
int WindowManager::getLastWindowHeight() const {
    return lastWindowHeight;
}
