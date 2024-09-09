/**
 * @file
 * @brief A file that demonstrates a simple solar system simulation using SDL2.
 *
 * This file creates a graphical representation of a solar system with planets orbiting the sun.
 * The solar system is rendered using SDL2 and textures are loaded from BMP files.
 */

#include <iostream>
#include <SDL.h>
#include <cmath>
#include <memory>
#include <array>

constexpr int SCREEN_WIDTH = 1920; ///< The width of the window.
constexpr int SCREEN_HEIGHT = 1080; ///< The height of the window.

/**
 * @struct Planet
 * @brief Represents a planet in the solar system.
 *
 * The Planet struct holds information about a planet's position, orbit, and texture.
 */
struct Planet {
    SDL_Rect rect; ///< The rectangle defining the position and size of the planet.
    double orbitRadius; ///< The radius of the planet's orbit around the sun.
    double orbitSpeed; ///< The speed at which the planet orbits the sun.
    double angle; ///< The current angle of the planet in its orbit.
    std::shared_ptr<SDL_Texture> texture; ///< The texture representing the planet.
    int halfW; ///< Half of the planet's width, used for positioning.
    int halfH; ///< Half of the planet's height, used for positioning.

    /**
     * @brief Constructs a Planet object.
     * @param w Width of the planet's texture.
     * @param h Height of the planet's texture.
     * @param orbitRadius The radius of the planet's orbit.
     * @param orbitSpeed The speed of the planet's orbit.
     * @param angle The initial angle of the planet.
     * @param texture The texture of the planet.
     */
    Planet(int w, int h, double orbitRadius, double orbitSpeed, double angle, std::shared_ptr<SDL_Texture> texture)
        : rect{ 0, 0, w, h }, orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), angle(angle), texture(texture), halfW(w / 2), halfH(h / 2) {}

    /**
     * @brief Updates the position of the planet based on its orbit.
     * @param sunX The x-coordinate of the sun's center.
     * @param sunY The y-coordinate of the sun's center.
     */
    void updatePosition(int sunX, int sunY) {
        rect.x = sunX + static_cast<int>(orbitRadius * cos(angle)) - halfW;
        rect.y = sunY + static_cast<int>(orbitRadius * sin(angle)) - halfH;
        angle += orbitSpeed;
    }

    /**
     * @brief Renders the planet to the screen.
     * @param renderer The SDL_Renderer used for rendering.
     */
    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture.get(), NULL, &rect);
    }
};

/**
 * @brief Main function for the solar system simulation.
 *
 * Initializes SDL, loads textures, and enters the main loop where the solar system is rendered and updated.
 * The loop continues until the user closes the window or presses the escape key.
 * @return Exit status.
 */
int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not be initialized: " << SDL_GetError() << std::endl;
        return -1;
    }
    else {
        std::cout << "SDL video system initialized\n";
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); ///< Set rendering scale quality to linear for better texture quality.
    window = SDL_CreateWindow("C++ SDL2 Window", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    auto textureStars = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/stars.bmp")), SDL_DestroyTexture);
    auto textureSun = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/sun.bmp")), SDL_DestroyTexture);
    auto textureMercury = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/mercury.bmp")), SDL_DestroyTexture);
    auto textureVenus = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/venus.bmp")), SDL_DestroyTexture);
    auto textureEarth = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/earth.bmp")), SDL_DestroyTexture);
    auto textureMars = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/mars.bmp")), SDL_DestroyTexture);
    auto textureJupiter = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/jupiter.bmp")), SDL_DestroyTexture);
    auto textureSaturn = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/saturn.bmp")), SDL_DestroyTexture);
    auto textureUranus = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/uranus.bmp")), SDL_DestroyTexture);
    auto textureNeptune = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/neptune.bmp")), SDL_DestroyTexture);

    if (!textureStars || !textureSun || !textureMercury || !textureVenus || !textureEarth ||
        !textureMars || !textureJupiter || !textureSaturn || !textureUranus || !textureNeptune) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Rect sunRect = { (SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT - 100) / 2, 100, 100 }; ///< The rectangle defining the position and size of the sun.
    SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; ///< The rectangle defining the background.

    const int sunXCoordinate = sunRect.x + sunRect.w / 2; ///< The x-coordinate of the sun's center calcualted outside of the loop.
    const int sunYCoordinate = sunRect.y + sunRect.h / 2; ///< The y-coordinate of the sun's center calcualted outside of the loop.

    Planet mercury = { 30, 30, 80.0, 0.02 * 4.15, 45.0, textureMercury };
    Planet venus = { 40, 40, 125.0, 0.02 * 1.62, 90.0, textureVenus };
    Planet earth = { 50, 50, 175.0, 0.02, 135.0, textureEarth };
    Planet mars = { 40, 40, 225.0, 0.02 * 0.53, 180.0, textureMars };
    Planet jupiter = { 80, 80, 290.0, 0.02 * 0.084, 225.0, textureJupiter };
    Planet saturn = { 70, 70, 380.0, 0.02 * 0.034, 270.0, textureSaturn };
    Planet uranus = { 60, 60, 470.0, 0.02 * 0.012, 315.0, textureUranus };
    Planet neptune = { 60, 60, 550.0, 0.02 * 0.006, 0.0, textureNeptune };

    std::array<Planet, 8> planets = { mercury, venus, earth, mars, jupiter, saturn, uranus, neptune };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); ///< Set the draw color to black.

    bool running = true;
    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, textureStars.get(), NULL, &backgroundRect);
        SDL_RenderCopy(renderer, textureSun.get(), NULL, &sunRect);

        for (Planet& planet : planets) {
            planet.updatePosition(sunXCoordinate, sunYCoordinate);
            planet.render(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); ///< Delay to achieve a frame rate of approximately 60 FPS.
    }

    SDL_DestroyWindow(window); ///< Clean up resources and close the window.
    SDL_Quit(); ///< Quit SDL.
    return 0;
}
