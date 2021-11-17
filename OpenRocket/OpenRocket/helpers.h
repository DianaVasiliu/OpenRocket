#pragma once
#include "Game.h"
#define	checkImageWidth 64
#define	checkImageHeight 64

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void);
void cleanupCallback();
void renderCallback();
void keysDownCallback(int, int, int);
void keysUpCallback(int, int, int);
void moveCallback();
void mouseCallback(int, int, int, int);
void processNormalKeys(unsigned char, int, int);
