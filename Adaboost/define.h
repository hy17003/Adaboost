#ifndef DEFINE_H
#define DEFINE_H

typedef struct RECT_STRU
{
	int x;
	int y;
	int width;
	int height;
};

typedef struct HAARECT_STRU
{
	RECT_STRU Rects[4];
	double weight[4];
}HAARECT_STRU, *PHAARECT_STRU;


#endif