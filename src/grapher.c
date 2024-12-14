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
