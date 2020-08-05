#include "pixie.h"
#include "font.h"
#include "imgui.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "Demo.h"
#include <windows.h>

static const char* WindowTitle = "Demo system!";
unsigned char* frameBuffer = new unsigned char[WindowWidth * WindowHeight];


static void draw(int x, int y, uint32_t* pixels)
{
    for (int i = x; i < x+4; i++)
    {
        for (int j = y; j < y+4; j++)
        {
            if (i < WindowWidth && j < WindowHeight)
            {
                int index = (i + (j * WindowWidth));
                pixels[index] = MAKE_RGB(0, 0, 255);
            }
        }
    }
}

int main(int argc, char** argv)
{
    Pixie::Font font;


    Pixie::Window window;
    if (!window.Open(WindowTitle, WindowWidth, WindowHeight, false,true, 4))
        return 0;

    uint32_t* pixels = window.GetPixels();
    screen = frameBuffer;
    Init();
    unsigned long lastTime = timeGetTime();
    while (!window.HasKeyGoneUp(Pixie::Key_Escape))
    {
        unsigned long timeNow = timeGetTime();
        int dt = timeNow - lastTime;
        lastTime = timeNow;
        DrawFrame(dt);

        for (int i = 0; i < WindowWidth * WindowHeight; i++)
        {
            pixels[i] = palette[frameBuffer[i]];
        }

        if (!window.Update())
            break;
    }

    window.Close();

    printf("done\n");
}
