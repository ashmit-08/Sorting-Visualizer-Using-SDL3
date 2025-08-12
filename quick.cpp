// main.cpp
// Complete Quick‑Sort visualization using SDL3 with proper event pumping

#include <SDL3/SDL.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

const int W = 800;
const int H = 600;
const int N = 100;

SDL_Window*   win = nullptr;
SDL_Renderer* ren = nullptr;
std::vector<int> A;

// Draw the array bars, highlighting indices [l..r]
void draw(int l = -1, int r = -1) {
    // Pump events so the window stays responsive
    SDL_PumpEvents();

    // Clear to black
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    // Width of each bar
    float barW = float(W) / N;

    for (int i = 0; i < N; ++i) {
        SDL_FRect bar{
            i * barW,              // x
            float(H) - (A[i] * H / 100.0f), // y (bottom‑aligned)
            barW - 1.0f,           // width
            float(A[i]) * H / 100.0f // height
        };

        if (i >= l && i <= r) {
            // Highlight the active range in red
            SDL_SetRenderDrawColor(ren, 255, 100, 100, 255);
        } else {
            // Normal bars in gray
            SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        }
        SDL_RenderFillRect(ren, &bar);
    }

    // Present what we've drawn
    SDL_RenderPresent(ren);

    // Delay to control animation speed
    SDL_Delay(20);
}

// Partition step for QuickSort
int partition(int lo, int hi) {
    int pivot = A[hi];
    int i = lo;
    for (int j = lo; j < hi; ++j) {
        if (A[j] < pivot) {
            std::swap(A[i], A[j]);
            draw(i, j);
            ++i;
        }
    }
    std::swap(A[i], A[hi]);
    draw(i, hi);
    return i;
}

// Recursive QuickSort
void quickSort(int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(lo, hi);
    quickSort(lo, p - 1);
    quickSort(p + 1, hi);
}

int main() {
    // Seed and fill the array with random values 0–99
    std::srand((unsigned)std::time(nullptr));
    A.resize(N);
    for (int i = 0; i < N; ++i) {
        A[i] = std::rand() % 100;
    }

    // Initialize SDL3
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    win = SDL_CreateWindow("QuickSort SDL3", W, H, 0);
    // Let SDL pick the default renderer:
    ren = SDL_CreateRenderer(win, nullptr);

    if (!win || !ren) {
        SDL_Log("SDL_CreateWindow/Renderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Ensure it's visible and pump once
    SDL_ShowWindow(win);
    SDL_PumpEvents();

    // 1) Draw the initial unsorted array
    draw();

    // 2) Perform QuickSort (calls draw() internally)
    quickSort(0, N - 1);

    // 3) Draw the final, fully sorted array
    draw();

    // 4) Wait here until user closes the window
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            break;
        }
    }

    // Cleanup
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
