#pragma once
#include "stdafx.h"

struct conditionConstraints
{
    vector<string> objectNames;

    vector<GLfloat> radiusRange;
    char radiusRangeType;

    vector<GLfloat> slantRange;
    char slantRangeType;

    vector<GLfloat> heightRange;
    char heightRangeType;

    vector<GLfloat> tiltRange;
    char tiltRangeType;

    vector<GLfloat> initZAsptRatioRange;
    char initZAsptRatioRangeType;

    vector<GLfloat> rotSpeedRange;
    char rotSpeedRangeType;

    vector<GLfloat> maxRotDegRange;
    char maxRotDegRangeType;

    vector< vector<string> * > textureGroups;

    int weight; // The weight of the number of this kind of conditions in all conditions
};
typedef struct conditionConstraints condCons_t;
