#pragma once
#include "stdafx.h"

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
};
typedef struct conditionConstraints condCons_t;
