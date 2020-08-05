#pragma once
#ifndef DEMO_H

typedef unsigned int uint;
typedef unsigned char uchar;

static const int WindowWidth = 320;
static const int WindowHeight = 240;
extern uchar* screen;
extern uint* palette;

void Init();
void DrawFrame(int dt);
void ClearScreen(uchar c);
#endif // DEMO_H