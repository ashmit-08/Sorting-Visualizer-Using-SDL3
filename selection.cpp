#include <SDL3/SDL.h>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>

using namespace std;

const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;
const int arrSize = 130;
const int rectSize = 7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool complete = false;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Sorting Visualizer", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        cout << "Couldn't create window. SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void visualize(int x = -1, int y = -1, int z = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize) {
        SDL_FRect rect = {(float)i, 0.0f, (float)rectSize, (float)arr[j]};
        if (complete) {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 255);
            SDL_RenderRect(renderer, &rect);
        } else if (j == x || j == z) {
           SDL_SetRenderDrawColor(renderer, 100, 180, 100, 255);
            SDL_RenderFillRect(renderer, &rect);
        } else if (j == y) {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 255);
            SDL_RenderFillRect(renderer, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 255);
            SDL_RenderRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void selectionSort() {
    for (int i = 0; i < arrSize - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < arrSize; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
                visualize(i, minIndex);
            }
            SDL_Delay(5);
        }
        swap(arr[i], arr[minIndex]);
    }
}

void loadArr() {
    memcpy(arr, Barr, sizeof(int) * arrSize);
}

void randomizeAndSaveArray() {
    srand((unsigned)time(nullptr));
    for (int i = 0; i < arrSize; i++) {
        Barr[i] = rand() % (SCREEN_HEIGHT - 50);
    }
}

void execute() {
    if (!init()) return;

    randomizeAndSaveArray();
    loadArr();

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
                complete = false;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case 'q':
                        quit = true;
                        complete = false;
                        break;
                    case '0':
                        randomizeAndSaveArray();
                        complete = false;
                        loadArr();
                        break;
                    case '1':
                        loadArr();
                        complete = false;
                        selectionSort();
                        complete = true;
                        break;
                }
            }
        }
        visualize();
    }
    close();
}

int main(int argc, char* argv[]) {
    cout << "Sorting Visualizer in SDL3\n";
    cout << "Press 0 to randomize list, 1 to start selection sort, q to quit\n";
    execute();
    return 0;
}
