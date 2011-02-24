#include "stdafx.h"
#include "CylinderFactory.h"

CylinderFactory::CylinderFactory(void)
{
    this->productName = "Cylinder";
}

CylinderFactory::~CylinderFactory(void)
{
}

TestObject* CylinderFactory::createObject(condCons_t& constraint, vector<texture_t *>& textures)
{
    // Check if the number of textures is larger than 3
    if(textures.size() < 3)
    {
        ostringstream ossWarning;
        ossWarning << "The Cylinder requires three textures, but only " 
            << textures.size() << " provided.";
        
        string warningMsg = ossWarning.str();
        MessageBox(NULL, (LPCSTR)(warningMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
        return NULL;
    }

    enumRotDirection defaultRotDirection;
    int randInt = rand() % 2;

    if(randInt == 0)
        defaultRotDirection = CLOCKWISE;
    else
        defaultRotDirection = COUNTERCLOCKWISE;

    CylinderObject *pObj = new CylinderObject(constraint.pitchRange, 
            constraint.yawRange,
            constraint.rollRange,
            constraint.heightRange,
            constraint.initZAsptRatioRange,
            constraint.rotSpeedRange,
            constraint.maxRotDegRange,
            defaultRotDirection,
            textures,
            constraint.radiusRange);

    pObj->setRandPara();
    pObj->buildModel();
    return pObj;
}

vector<TestObject *> CylinderFactory::createAllObjects(condCons_t& constraint, vector<texture_t *>& textures)
{
    vector<TestObject *> pObjs;

    // Check if the number of textures is larger than 3
    if(textures.size() < 3)
    {
        ostringstream ossWarning;
        ossWarning << "The Cylinder requires three textures, but only " 
            << textures.size() << " provided.";
        
        string warningMsg = ossWarning.str();
        MessageBox(NULL, (LPCSTR)(warningMsg.c_str()), NULL, MB_OK | MB_ICONERROR);
        return pObjs;
    }

    vector<int> indexBound;
    rangeType<GLfloat>*  ranges[] = {&constraint.pitchRange, &constraint.yawRange, &constraint.rollRange,
        &constraint.heightRange, &constraint.initZAsptRatioRange, &constraint.rotSpeedRange,
        &constraint.maxRotDegRange, &constraint.radiusRange};

    for(int i = 0; i < 8; i ++)
    {
        if(ranges[i]->type == 'R')
            indexBound.push_back(-1);
        else
            indexBound.push_back(ranges[i]->range.size());
    }

    vector< vector<int> > parameterIndices;
    vector<int> oneParameterIndexSet;

    this->_generateAllParameterCombinations(indexBound, parameterIndices, oneParameterIndexSet, 0);

    for(int i = 0; i < (int)parameterIndices.size(); i ++)
    {

        enumRotDirection defaultRotDirection;
        int randInt = rand() % 2;

        if(randInt == 0)
            defaultRotDirection = CLOCKWISE;
        else
          defaultRotDirection = COUNTERCLOCKWISE;

        CylinderObject *pObj = new CylinderObject(constraint.pitchRange, 
                constraint.yawRange,
                constraint.rollRange,
                constraint.heightRange,
                constraint.initZAsptRatioRange,
                constraint.rotSpeedRange,
                constraint.maxRotDegRange,
                defaultRotDirection,
                textures,
                constraint.radiusRange);

        GLfloat* parameters[] = {&pObj->pitch, &pObj->yaw, &pObj->roll, &pObj->height,
            &pObj->initZAsptRatio, &pObj->rotSpeed, &pObj->maxRotDeg, &pObj->radius};

        for(int j = 0; j < 8; j ++)
        {
            if(parameterIndices[i][j] == -1)
                *parameters[j] = ranges[j]->getRandomValue();
            else
                *parameters[j] = ranges[j]->range[parameterIndices[i][j]];
        }
        pObj->buildModel();
    
        pObjs.push_back(pObj);
    }
    return pObjs;
}

void CylinderFactory::_generateAllParameterCombinations(vector<int>& bound,
                            vector< vector<int> >& combinations, vector<int> currCombination,
                            int paraIndex)
{
    if(paraIndex >= 8)
    {
        combinations.push_back(currCombination);
        return;
    }

    if(bound[paraIndex] == -1)
    {
        currCombination.push_back(-1);
        this->_generateAllParameterCombinations(bound, combinations, currCombination, paraIndex + 1);
        currCombination.pop_back();
    }
    else
    {
        for(int i = 0; i < bound[paraIndex]; i ++)
        {
            currCombination.push_back(i);
            this->_generateAllParameterCombinations(bound, combinations, currCombination, paraIndex + 1);
            currCombination.pop_back();
        }
    }
}

const string CylinderFactory::getProductName()
{
    return this->productName;
}
