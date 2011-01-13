#pragma once

#include "stdafx.h"
#include "TestObject.h"

struct conditionConstraints
{
    vector<TestObject *> templateObjects;
    int weight; // The weight of the number of this kind of conditions in all conditions
};
typedef struct conditionConstraints condCons_t;

struct conditionStruct
{
    TestObject *realObject;
};
typedef struct conditionStruct cond_t;

class Conditions
{
    public:
        explicit Conditions(string& filename, int numConditions,
                vector<TestObject *>templateObjects);
        ~Conditions(void);

        cond_t& operator[](int &rhs);

        void addConstraint(condCons_t& constraint);
        
        void addCondition(int constraintIndex);
        void addCondition(cond_t& condition);

        void generateConditions();
        void shuffleConditions();

        vector<condCons_t>& getAllConstraints();
        vector<cond_t>& getAllConditions();
    private:
        vector<condCons_t> constraints;
        vector<cond_t> conditions;

        template<typename T>
        BOOL readRange(ifstream& fin, vector<T>& vec);

        int numConditions;

        // Forbid the operations below
        Conditions& operator=(Conditions& rhs); 
};
