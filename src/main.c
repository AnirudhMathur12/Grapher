#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

int mouse_screen_drag = 0;

int running = 1;

void process_input();
void graph_function(SDL_Renderer* rend, float scale, double (*fn)(double));

int offsetX, offsetY;
int init_mousePosX, init_mousePosY;
int curr_mousePosX, curr_mousePosY;

int curr_XaxisPos, curr_YaxisPos;

typedef struct
{
    float posX;
    float posY;
    float scale;
} Camera;

typedef struct
{
    float x;
    float y;
} PointF;

double f(double x)
{
    return tan(x);
}

double f2(double x)
{ return 2*cos(x)*cos(x); }

float scale = 0.01;

int main()
{
    curr_XaxisPos = WINDOW_HEIGHT/2;
    curr_YaxisPos = WINDOW_WIDTH/2;
    Camera cam = {0, 0, 1};
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *win = SDL_CreateWindow("Grapher", 500, 500, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, 0);

    SDL_Event e;
    
    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                running = 0;
                break;
                case SDL_MOUSEBUTTONDOWN:
                mouse_screen_drag = 1;
                SDL_GetMouseState(&init_mousePosX, &init_mousePosY); 
                break;
                case SDL_MOUSEBUTTONUP:
                mouse_screen_drag = 0;
                break;
                case SDL_MOUSEWHEEL:
                scale -= (abs(e.wheel.y)/e.wheel.y)*0.0005;

            }
        }

        if(mouse_screen_drag)
        {
            SDL_GetMouseState(&curr_mousePosX, &curr_mousePosY);
            offsetX = curr_mousePosX - init_mousePosX;
            offsetY = curr_mousePosY - init_mousePosY;
            init_mousePosX = curr_mousePosX;
            init_mousePosY = curr_mousePosY;
        } 
        
        curr_XaxisPos += offsetY;
        curr_YaxisPos += offsetX;

        SDL_Rect x_axis = {0, curr_XaxisPos, WINDOW_WIDTH, 1};
        SDL_Rect y_axis = {curr_YaxisPos, 0, 1, WINDOW_HEIGHT};
        SDL_SetRenderDrawColor( rend, 0, 0, 0, 0 ); // line of code in question
        SDL_RenderClear( rend );

        SDL_SetRenderDrawColor( rend, 0xFF, 0x00, 0x00, 0x00 ); // 2nd line of code in question
        SDL_RenderFillRect( rend, &x_axis );
        SDL_RenderFillRect( rend, &y_axis );

        graph_function(rend, scale, &f);
        //graph_function(rend, 0.01, &f2);

        SDL_RenderPresent( rend );
    }
    
    return 0; 
}

void process_input()
{
    SDL_PumpEvents();
    //int* keyboard = SDL_GetKeyboardState(NULL);
}

void graph_function(SDL_Renderer *rend, float scale, double (*fn)(double))
{
    float resolution = 0.001;
    float min = (-WINDOW_WIDTH)*scale/2;
    float max = (WINDOW_WIDTH)*scale/2;
    int iter = 0;

    PointF points[(int)((max-min)/resolution) + 1];



    for(float i = min; i < max; i+=resolution)
    {
        float x = i - (curr_YaxisPos - WINDOW_WIDTH/2)*scale;
        points[iter++] = (PointF){x, fn(x)};
    }

    SDL_SetRenderDrawColor(rend, 0x00, 0xFF, 0x00, 0x00);
    
    for(int i = 0; i < iter - 1; i++)
    {
        SDL_RenderDrawLineF(rend, points[i].x/scale + curr_YaxisPos, curr_XaxisPos - points[i].y/scale, points[i+1].x/scale + curr_YaxisPos, curr_XaxisPos - points[i+1].y/scale);
    } 
}
