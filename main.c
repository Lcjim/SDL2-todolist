#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>



SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Renderer* renderer);

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible d'initialiser SDL! Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    const int windowWidth = 900;
    const int windowHeight = 600;
    SDL_Window* window = SDL_CreateWindow("To-Do List", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible de créer la fenêtre! Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible de créer le rendu! Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation de la bibliothèque SDL_ttf
    if (TTF_Init() == -1) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible d'initialiser SDL_ttf! Erreur SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }

    // Création d'une police avec la police Arial et une taille spécifique
    TTF_Font* buttonFont = TTF_OpenFont("arial.ttf", 16);
    if (!buttonFont) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible de charger la police! Erreur SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }

    // Boucle principale
    SDL_Event event;
    bool quit = false;
    bool displayText = false;  // Indique si le texte doit être affiché

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Vérifie si le clic est sur le bouton supérieur
                if (mouseX >= topButton.x && mouseX <= topButton.x + topButton.w &&
                    mouseY >= topButton.y && mouseY <= topButton.y + topButton.h) {
                    displayText = true;
                }

                // Vérifie si le clic est sur le bouton supérieur supplémentaire
                if (mouseX >= anotherTopButton.x && mouseX <= anotherTopButton.x + anotherTopButton.w &&
                    mouseY >= anotherTopButton.y && mouseY <= anotherTopButton.y + anotherTopButton.h) {
                    displayText = true;
                }
            }
        }

        // Efface l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw columns using SDL_RenderDrawRect
        SDL_Rect todoColumn = {0, 0, windowWidth/3, windowHeight};
        SDL_Rect inProgressColumn = {windowWidth/3, 0, windowWidth/3, windowHeight};
        SDL_Rect doneColumn = {(windowWidth/3)*2, 0, windowWidth/3, windowHeight};

        // Change the color of the columns
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        SDL_RenderFillRect(renderer, &todoColumn);

        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Light green
        SDL_RenderFillRect(renderer, &inProgressColumn);

        SDL_SetRenderDrawColor(renderer, 114, 62, 119, 255); // Light purple
        SDL_RenderFillRect(renderer, &doneColumn);

        // Draw buttons
        const int buttonWidth = 40;
        const int buttonHeight = 40;
        const int buttonSpacing = 10;

        SDL_Rect topButton = {doneColumn.x - buttonWidth - buttonSpacing, doneColumn.y + doneColumn.h - buttonHeight - 20, buttonWidth, buttonHeight};  // Top button
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Light green
        SDL_RenderFillRect(renderer, &topButton);

        SDL_Rect middleButton = {doneColumn.x - buttonWidth - buttonSpacing, doneColumn.y + doneColumn.h + buttonSpacing, buttonWidth, buttonHeight};  // Middle button
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderFillRect(renderer, &middleButton);

        SDL_Rect bottomButton = {doneColumn.x - buttonWidth - buttonSpacing, doneColumn.y + doneColumn.h + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight};  // Bottom button
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
        SDL_RenderFillRect(renderer, &bottomButton);

        // Draw another button above the top button
        SDL_Rect anotherTopButton = {topButton.x, topButton.y - buttonHeight - buttonSpacing, buttonWidth, buttonHeight};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        SDL_RenderFillRect(renderer, &anotherTopButton);

        // Affiche le texte si la condition est vraie
        if (displayText) {
            SDL_Texture* buttonText = renderText("Bouton supérieur cliqué!", buttonFont, renderer);
            SDL_Rect textRect = {topButton.x + 10, topButton.y + 10, 0, 0};
            SDL_QueryTexture(buttonText, NULL, NULL, &textRect.w, &textRect.h);
            SDL_RenderCopy(renderer, buttonText, NULL, &textRect);
            SDL_DestroyTexture(buttonText);
        }

        // Met à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // ...

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// Fonction pour rendre le texte sur le bouton
SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Color textColor = {0, 0, 0, 255};  // Couleur du texte (noir)
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible de créer la surface de texte! Erreur SDL_ttf : %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        // Gestion d'erreur
        fprintf(stderr, "Impossible de créer la texture de texte! Erreur SDL : %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return NULL;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}