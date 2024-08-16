#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Define the color palette as constants
const SDL_Color RED = {255, 0, 0, 255};
const SDL_Color GREEN = {0, 255, 0, 255};
const SDL_Color BLUE = {0, 0, 255, 255};
const SDL_Color YELLOW = {255, 255, 0, 255};
const SDL_Color BLACK = {0, 0, 0, 255};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Davey Paint", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create the texture for the drawing area
    SDL_Texture* drawingTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    if (drawingTexture == NULL) {
        printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set the render target to the drawing texture and clear it to white
    SDL_SetRenderTarget(renderer, drawingTexture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Initially set the drawing color to black
    SDL_Color currentColor = BLACK;

    // Define the color palette rectangles
    SDL_Rect redRect = {10, 10, 50, 50};
    SDL_Rect greenRect = {70, 10, 50, 50};
    SDL_Rect blueRect = {130, 10, 50, 50};
    SDL_Rect yellowRect = {190, 10, 50, 50};
    SDL_Rect blackRect = {250, 10, 50, 50};

    bool quit = false;
    bool drawing = false;
    int lastX, lastY;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Check if the click is within one of the palette rectangles
                if (x >= redRect.x && x <= redRect.x + redRect.w && y >= redRect.y && y <= redRect.y + redRect.h) {
                    currentColor = RED;
                } else if (x >= greenRect.x && x <= greenRect.x + greenRect.w && y >= greenRect.y && y <= greenRect.y + greenRect.h) {
                    currentColor = GREEN;
                } else if (x >= blueRect.x && x <= blueRect.x + blueRect.w && y >= blueRect.y && y <= blueRect.y + blueRect.h) {
                    currentColor = BLUE;
                } else if (x >= yellowRect.x && x <= yellowRect.x + yellowRect.w && y >= yellowRect.y && y <= yellowRect.y + yellowRect.h) {
                    currentColor = YELLOW;
                } else if (x >= blackRect.x && x <= blackRect.x + blackRect.w && y >= blackRect.y && y <= blackRect.y + blackRect.h) {
                    currentColor = BLACK;
                } else {
                    // Start drawing if the click is outside the palette
                    drawing = true;
                    lastX = x;
                    lastY = y;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    drawing = false;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                if (drawing) {
                    int x = event.motion.x;
                    int y = event.motion.y;

                    // Set the drawing color to the currently selected color
                    SDL_SetRenderTarget(renderer, drawingTexture); // Ensure we draw on the texture
                    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
                    SDL_RenderDrawLine(renderer, lastX, lastY, x, y);
                    lastX = x;
                    lastY = y;
                }
            }
        }

        // Render the drawing texture and palette to the window
        SDL_SetRenderTarget(renderer, NULL); // Switch back to rendering on the window
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Clear the window
        SDL_RenderClear(renderer);

        // Copy the drawing texture to the window
        SDL_RenderCopy(renderer, drawingTexture, NULL, NULL);

        // Draw the color palette on top of the drawing texture
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &redRect);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &greenRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blueRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &yellowRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &blackRect);

        // Present the window with the updated content
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(drawingTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

