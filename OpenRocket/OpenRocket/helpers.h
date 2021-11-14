#pragma once

#include "Game.h"

void cleanupCallback();
void renderCallback();
void keysDownCallback(int key, int x, int y);
void keysUpCallback(int key, int x, int y);
void moveCallback();
void mouseCallback(int button, int state, int x, int y);
