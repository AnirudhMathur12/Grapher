#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

int mouse_screen_drag = 0;
int running = 1;

void graph_function(SDL_Renderer *rend, float scale, double (*fn)(double));

/* SDL3 mouse uses float positions */
float init_mousePosX = 0.0f, init_mousePosY = 0.0f;
float curr_mousePosX = 0.0f, curr_mousePosY = 0.0f;
float offsetX = 0.0f, offsetY = 0.0f;

float curr_XaxisPos, curr_YaxisPos;

typedef struct {
    float x;
    float y;
} PointF;

double f(double x) { return tan(x); }
double f2(double x) { return 2 * cos(x) * cos(x); }
double f3(double x) { return (sin(log(x) + 1)) / x; }

float scale = 0.01f;

int main(void) {
    curr_XaxisPos = WINDOW_HEIGHT / 2.0f;
    curr_YaxisPos = WINDOW_WIDTH / 2.0f;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Grapher", WINDOW_WIDTH, WINDOW_HEIGHT,
                                       SDL_WINDOW_RESIZABLE);

    if (!win) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer *rend = SDL_CreateRenderer(win, NULL);

    if (!rend) {
        SDL_Log("Metal renderer failed, trying fallback: %s", SDL_GetError());
        rend = SDL_CreateRenderer(win, NULL);
    }
    if (!rend) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Event e;

    while (running) {

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_EVENT_QUIT:
                running = 0;
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mouse_screen_drag = 1;
                SDL_GetMouseState(&init_mousePosX, &init_mousePosY);
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouse_screen_drag = 0;
                break;

            case SDL_EVENT_MOUSE_WHEEL:
                if (e.wheel.y > 0)
                    scale -= 0.0005f;
                if (e.wheel.y < 0)
                    scale += 0.0005f;
                if (scale < 1e-6f)
                    scale = 1e-6f;
                break;
            }
        }

        if (mouse_screen_drag) {
            SDL_GetMouseState(&curr_mousePosX, &curr_mousePosY);
            offsetX = curr_mousePosX - init_mousePosX;
            offsetY = curr_mousePosY - init_mousePosY;
            init_mousePosX = curr_mousePosX;
            init_mousePosY = curr_mousePosY;
        } else {
            offsetX = offsetY = 0.0f;
        }

        curr_XaxisPos += offsetY;
        curr_YaxisPos += offsetX;

        SDL_FRect x_axis = {0.0f, curr_XaxisPos, (float)WINDOW_WIDTH, 1.0f};
        SDL_FRect y_axis = {curr_YaxisPos, 0.0f, 1.0f, (float)WINDOW_HEIGHT};

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_RenderFillRect(rend, &x_axis);
        SDL_RenderFillRect(rend, &y_axis);

        graph_function(rend, scale, f3);

        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void graph_function(SDL_Renderer *rend, float scale, double (*fn)(double)) {
    const float resolution = 0.001f;
    const float min = -WINDOW_WIDTH * scale * 0.5f;
    const float max = WINDOW_WIDTH * scale * 0.5f;

    int capacity = (int)((max - min) / resolution) + 3;
    PointF *points = malloc(sizeof(PointF) * capacity);
    if (!points)
        return;

    int count = 0;
    for (float t = min; t < max; t += resolution) {
        float x = t - (curr_YaxisPos - WINDOW_WIDTH / 2.0f) * scale;
        points[count++] = (PointF){x, (float)fn(x)};
    }

    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);

    for (int i = 0; i < count - 1; i++) {
        float x1 = points[i].x / scale + curr_YaxisPos;
        float y1 = curr_XaxisPos - points[i].y / scale;
        float x2 = points[i + 1].x / scale + curr_YaxisPos;
        float y2 = curr_XaxisPos - points[i + 1].y / scale;

        SDL_RenderLine(rend, x1, y1, x2, y2);
    }

    free(points);
}
