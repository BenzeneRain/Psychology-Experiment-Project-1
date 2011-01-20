#pragma once
#include "stdafx.h"

enum DisplayMode {CONTINUOUS_DISPLAY, DISCRETE_DISPLAY};
typedef enum DisplayMode displayMode_t;

struct conditionConstraints
{
    vector<string> objectNames;

    vector<GLfloat> radiusRange;
    char radiusRangeType;

    vector<GLfloat> pitchRange;
    char pitchRangeType;

    vector<GLfloat> yawRange;
    char yawRangeType;

    vector<GLfloat> rollRange;
    char rollRangeType;

    vector<GLfloat> heightRange;
    char heightRangeType;

    vector<GLfloat> initZAsptRatioRange;
    char initZAsptRatioRangeType;

    vector<GLfloat> rotSpeedRange;
    char rotSpeedRangeType;

    vector<GLfloat> maxRotDegRange;
    char maxRotDegRangeType;

    vector< vector<string> * > textureGroups;

    int weight; // The weight of the constraint

    displayMode_t dispMode;
    // Used only when the display mode is DISCRETE_DISPLAY
    float secDisplay; // seconds for displaying object
    float secBlackScreen; // seconds for displaying black screen
};
typedef struct conditionConstraints condCons_t;
