#pragma once
#include "Game.h"
#define	checkImageWidth 64
#define	checkImageHeight 64


static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void);
void cleanupCallback();
void renderCallback();
void keysDownCallback(int key, int x, int y);
void keysUpCallback(int key, int x, int y);
void moveCallback();
void mouseCallback(int button, int state, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
