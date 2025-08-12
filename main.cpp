#include <SDL3/SDL.h>
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;
const int arrSize = 130;
const int rectSize = 7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool complete = false;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window Error: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer Error: " << SDL_GetError() << "\n";
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int i = 0, j = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize, j++) {
        SDL_Rect rect = { i, 0, rectSize, arr[j] };
        if (complete) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else if (j == x || j == z) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else if (j == y) SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        else SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

void randomizeAndSaveArray() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < arrSize; i++)
        Barr[i] = rand() % SCREEN_HEIGHT;
}

void loadArr() {
    memcpy(arr, Barr, sizeof(arr));
}

void bubbleSort() {
    for (int i = 0; i < arrSize - 1; i++) {
        for (int j = 0; j < arrSize - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                visualize(j, j + 1);
                SDL_Delay(5);
            }
        }
    }
}

void selectionSort() {
    for (int i = 0; i < arrSize - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < arrSize; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
            visualize(i, minIdx);
            SDL_Delay(2);
        }
        swap(arr[i], arr[minIdx]);
    }
}

int partition(int a[], int si, int ei) {
    int pivot = a[si], count = 0;
    for (int i = si + 1; i <= ei; i++)
        if (a[i] <= pivot) count++;

    int pivotIdx = si + count;
    swap(a[si], a[pivotIdx]);
    visualize(pivotIdx, si);
    SDL_Delay(30);

    int i = si, j = ei;
    while (i < pivotIdx && j > pivotIdx) {
        while (a[i] <= pivot) i++;
        while (a[j] > pivot) j--;
        if (i < pivotIdx && j > pivotIdx) {
            swap(a[i], a[j]);
            visualize(i, j);
            SDL_Delay(30);
            i++; j--;
        }
    }

    return pivotIdx;
}

void quickSort(int a[], int si, int ei) {
    if (si >= ei) return;

    int p = partition(a, si, ei);
    quickSort(a, si, p - 1);
    quickSort(a, p + 1, ei);
}

void execute() {
    if (!init()) return;

    randomizeAndSaveArray();
    loadArr();

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        quit = true;
                        break;
                    case SDLK_0:
                        randomizeAndSaveArray();
                        loadArr();
                        complete = false;
                        cout << "\nNew list generated.\n";
                        break;
                    case SDLK_1:
                        loadArr();
                        complete = false;
                        cout << "\nSelection Sort started.\n";
                        selectionSort();
                        complete = true;
                        cout << "Selection Sort done.\n";
                        break;
                    case SDLK_2:
                        loadArr();
                        complete = false;
                        cout << "\nBubble Sort started.\n";
                        bubbleSort();
                        complete = true;
                        cout << "Bubble Sort done.\n";
                        break;
                    case SDLK_3:
                        loadArr();
                        complete = false;
                        cout << "\nQuick Sort started.\n";
                        quickSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "Quick Sort done.\n";
                        break;
                }
            }
        }
        visualize();
    }

    close();
}

void intro() {
    cout << "===== Sorting Visualizer with SDL2 =====\n"
         << "Press ENTER to continue...";
    cin.ignore();
}

void controls() {
    cout << "\nControls:\n"
         << "0 - Randomize list\n"
         << "1 - Selection Sort\n"
         << "2 - Bubble Sort\n"
         << "3 - Quick Sort\n"
         << "q - Quit\n"
         << "Press ENTER to start...";
    cin.ignore();
}

int main(int argc, char* args[]) {
    intro();
    controls();
    execute();
    return 0;
}
