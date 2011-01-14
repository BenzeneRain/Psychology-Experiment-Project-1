#pragma once

#include "stdafx.h"
#include "ConditionConstraints.h"
#include "TestObjectFactory.h"
#include "TestObject.h"
#include <hash_map>

using namespace stdext;

struct conditionStruct
{
    TestObject *pRealObject;
};
typedef struct conditionStruct cond_t;


class Conditions
{
    public:
        explicit Conditions(string& filename, int numConditions,
                vector<TestObjectFactory *>& rObjectFactories);
        ~Conditions(void);

        cond_t& operator[](int &rhs);

        void addConstraint(condCons_t* pConstraint);
        
        void addCondition(int constraintIndex);
        void addCondition(cond_t* pCondition);

        void generateConditions();
        void shuffleConditions();

        const vector<condCons_t *>& getAllConstraints();
        const vector<cond_t *>& getAllConditions();

    private:
        vector<condCons_t *> constraints;
        vector<cond_t *> conditions;
        vector<TestObjectFactory *>& objectFactories;
        hash_map<string, TestObjectFactory *> objectFactoryNameMap;

        template<typename T>
        BOOL readRange(ifstream& fin, vector<T>& vec, char& type);

        BOOL cylinderParameterReadingFunction(ifstream& fin, condCons_t& constraint);

        int numConditions;
        // Forbid the operations below
        Conditions& operator=(Conditions& rhs); 
};

