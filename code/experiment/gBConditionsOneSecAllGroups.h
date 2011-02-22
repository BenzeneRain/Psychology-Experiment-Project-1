#pragma once
#include "groupbasedconditions.h"

class gBConditionsOneSecAllGroups :
    public groupBasedConditions
{
    public:
        explicit gBConditionsOneSecAllGroups(ifstream& rFin, int numConditions,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);
        explicit gBConditionsOneSecAllGroups(ifstream& rFin,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);

        virtual ~gBConditionsOneSecAllGroups(void);

        virtual void shuffleConditions(int times);
        
        // get all conditions
        virtual const vector<cond_t *>& getAllConditions();

        // get the specific condition using its index
        virtual cond_t& operator[](int &rhs);
        
        virtual BOOL clearConditions();

    private:
        vector<cond_t *> shuffledConditions;

        void updateShuffledConditions();
};
