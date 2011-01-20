#pragma once
#include "stdafx.h"
#include "rangeType.h"

enum DisplayMode {CONTINUOUS_DISPLAY, DISCRETE_DISPLAY};
typedef enum DisplayMode displayMode_t;

struct conditionConstraints
{
    vector<string> objectNames;

    rangeType<GLfloat> radiusRange;
    rangeType<GLfloat> pitchRange;
    rangeType<GLfloat> yawRange;
    rangeType<GLfloat> rollRange;

    rangeType<GLfloat> heightRange;
    rangeType<GLfloat> initZAsptRatioRange;
    rangeType<GLfloat> rotSpeedRange;
    rangeType<GLfloat> maxRotDegRange;

    vector< vector<string> * > textureGroups;

    int weight; // The weight of the constraint

    displayMode_t dispMode;
    // Used only when the display mode is DISCRETE_DISPLAY
    float secDisplay; // seconds for displaying object
    float secBlackScreen; // seconds for displaying black screen
};
typedef struct conditionConstraints condCons_t;
