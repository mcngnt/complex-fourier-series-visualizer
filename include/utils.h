#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define SCREEN_W 1344
#define SCREEN_H 756

#define PI 3.14

float rand_float(bool);
float clamp(float, float, float);
float sign(float);
float neg_range(float);

#endif