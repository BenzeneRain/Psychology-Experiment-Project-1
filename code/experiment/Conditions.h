#pragma once

#include "stdafx.h"
#include "ConditionConstraints.h"
#include "TestObjectFactory.h"
#include "TestObject.h"
#include "Screen.h"
#include "texture.h"
#include <hash_map>

using namespace stdext;

struct conditionStruct
{
    TestObject *pRealObject;

    vector<texture_t *> textures;
};
typedef struct conditionStruct cond_t;

class Conditions
{
    public:
        explicit Conditions(string& rFilename, int numConditions,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);
        ~Conditions(void);

        BOOL initConditions();

        void addConstraint(condCons_t* pConstraint);
        
        void addCondition(int constraintIndex);
        void addCondition(cond_t* pCondition);

        BOOL generateConditions();
        void shuffleConditions();

        cond_t& operator[](int &rhs);
        const vector<condCons_t *>& getAllConstraints();
        const vector<cond_t *>& getAllConditions();

    private:
        vector<condCons_t *> constraints;
        vector<cond_t *> conditions;
        vector<TestObjectFactory *>& objectFactories;
        hash_map<string, TestObjectFactory *> objectFactoryNameMap;

        string filename;
        vector<rTexture_t *> textures;
        hash_map<string, int> textureMap;

        template<typename T>
        BOOL readRange(ifstream& fin, vector<T>& vec, char& type);
        void printReadRangeError(string name, int constraintID);
        BOOL readConstraints(ifstream& fin);
        BOOL readTextures(ifstream& fin);

        BOOL cylinderParameterReadingFunction(ifstream& fin, condCons_t& constraint, int constraintID);

        Screen& rScreen;

        int numConditions;
        // Forbid the operations below
        Conditions& operator=(Conditions& rhs); 
};

