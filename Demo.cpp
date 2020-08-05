#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include "Demo.h"
#include <math.h>
#include "triangle.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define RGB(r,g,b) ((b)|((g)<<8)|((r)<<16))
float time = 0;
uchar* screen;
uint* palette;

//#define USE_SBUF 

#ifdef USE_SBUF
mfloat_t position[120][VEC3_SIZE];
#define  byte unsigned char
typedef struct
{
	short xb, xe, zb, ze; // x_begin, x_end, ...
	byte ub, ue, vb, ve;
	long kz, ku, kv; // kz = (ze-zb)*65536/(xe-xb) etc
	sbuf_t* next;
	sbuf_t* prev;
} sbuf_t;

sbuf_t* sbufList[200]; // 200 lines in mode 320x200,
#endif
void Init()
{
	palette = new uint[256];
	for (int i = 0; i < 256; i++)
	{
		palette[i] = RGB(rand(), rand(), rand()) ;
	}
}

void project3dto2d(float outCoord[VEC2_SIZE], mfloat_t inCoord[VEC3_SIZE] )
{

	outCoord[0] = inCoord[0] * 512 / (inCoord[2]+256) + 160; 
	outCoord[1] = inCoord[1] * 512 / (inCoord[2]+256) + 120;
}

void swap(float& a, float& b)
{
	float temp = a;
	a = b;
	b = temp;
}
void swap(short& a, short& b)
{
	short temp = a;
	a = b;
	b = temp;
}

void putPixel(int x, int y, uchar c )
{
	screen[y * WindowWidth + x] = c;
}

void horizLine(int x0, int x2, int y, uchar color)
{

	x2 = MIN(x2, WindowWidth - 1);
	x2 = MAX(0, x2);
	if (x2 < x0)
	{
		int t = x2;
		x2 = x0;
		x0 = t;
	}
	for (int i = x0; i < x2; i++)
	{
		putPixel(i, y, color);
	}

}


void triFill2(float p[3][VEC2_SIZE], uchar col)

{
	if (p[1][1] < p[0][1])
	{
		swap(p[0][1], p[1][1]);
		swap(p[0][0], p[1][0]);
	}
	if (p[2][1] < p[1][1])
	{
		swap(p[2][1], p[1][1]);
		swap(p[2][0], p[1][0]);
	}
	if (p[1][1] < p[0][1])
	{
		swap(p[0][1], p[1][1]);
		swap(p[0][0], p[1][0]);
	}
	int y = (int)p[0][1];
	float xac = p[0][0];
	float xab = p[0][0];
	float xbc = p[1][0];
	float xaci = 0;
	float xabi = 0;
	float xbci = 0;

	if ((int)p[1][1] != (int)p[0][1])
		xabi = ((p[1][0] - p[0][0])) / (int)(p[1][1] - (int)p[0][1]);
	if ((int)p[2][1] != (int)p[0][1])
		xaci = ((p[2][0] - p[0][0])) / (int)(p[2][1] - (int)p[0][1]);
	if ((int)p[2][1] != (int)p[1][1])
		xbci = ((p[2][0] - p[1][0])) / (int)(p[2][1] - (int)p[1][1]);

	for (; y < p[1][1] && y < WindowHeight; y++)
	{
		if (y >= 0)
			horizLine((int)xab, (int)xac, y, col);
		xab += xabi;
		xac += xaci;
	}
	for (; y < p[2][1] && y < WindowHeight; y++)
	{
		if (y >= 0)
			horizLine((int)xbc, (int)xac, y, col);
		xbc += xbci;
		xac += xaci;
	}
}
void trifill3D(mfloat_t* v0, mfloat_t* v1, mfloat_t* v2, uchar col)
{
	float p[3][2];
	
	project3dto2d(p[0], v0);
	project3dto2d(p[1], v1);
	project3dto2d(p[2], v2);
	
	triFill2(p, col);

}
void ClearScreen(uchar c)
{

	memset(screen, c, WindowWidth * WindowHeight);
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
	array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition(float arrZ[], short arrIndex[], int low, int high)
{
	float pivot = arrZ[high];    // pivot 
	int i = (low - 1);  // Index of smaller element 

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot 
		if (arrZ[j] < pivot)
		{
			i++;    // increment index of smaller element 
			swap(arrZ[i], arrZ[j]);
			swap(arrIndex[i], arrIndex[j]);
		}
	}
	swap(arrZ[i + 1], arrZ[high]);
	swap(arrIndex[i+1], arrIndex[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(float arrZ[], short arrIndex[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		   at right place */
		int pi = partition(arrZ, arrIndex, low, high);

		// Separately sort elements before 
		// partition and after partition 
		quickSort(arrZ, arrIndex, low, pi - 1);
		quickSort(arrZ, arrIndex, pi + 1, high);
	}
}



void DrawFrame(int dt)
{

	for (int i = 0; i < 200; i++)
	{

	}

	time += dt * 0.001f;
	
	mfloat_t objectMatrix[MAT4_SIZE];

	ClearScreen(0xee);
#define VERTICES 12
	short drawOrder[VERTICES / 3];
	float triZ[VERTICES / 3];



	mfloat_t p[VERTICES][VEC4_SIZE] = {


		{ 0, -30, 0,1 },
		{ -30, 30, 30,1 },
		{ 30, 30, 30,1 },


		{ 0, -30, 0,1 },
		{ 30, 30, -30,1 },
		{ 30, 30, 30,1 },
		

		{ 0, -30, 0,1 },
		{ -30, 30, -30,1 },
		{ 30, 30, -30,1 },

		{ 0, -30, 0,1 },
		{ -30, 30, 30,1 },
		{ -30, 30, -30,1 },


	};


	mat4_identity(objectMatrix);
	mfloat_t translateVector[VEC4_SIZE] = {10+sin(time*0.4f)*30, 0, sin(time * 3.1f) * 30, 1};
	mat4_rotation_y(objectMatrix, time);
//	mat4_rotation_x(objectMatrix, time*0.3);
	//mat4_translation(objectMatrix, objectMatrix, translateVector);



	for (int i = 0; i < VERTICES; i++)
	{
		vec4_multiply_mat4(p[i], p[i], objectMatrix);
	}

	// sort points
	for (int i = 0; i < VERTICES; i += 3)
	{
		// Calc Z
		triZ[i/3] = (p[i + 0][2]+p[i + 1][2]+p[i + 2][2])*0.33333f;
		drawOrder[i / 3] = i / 3;
	}
	quickSort(triZ, drawOrder, 0, VERTICES/3 - 1);

	for (int i = 0; i < VERTICES/3;i++ )
	{
		trifill3D(p[drawOrder[i]*3 + 0], p[drawOrder[i]*3 + 1], p[drawOrder[i]*3 + 2], 0x22+drawOrder[i]);
	}


}
