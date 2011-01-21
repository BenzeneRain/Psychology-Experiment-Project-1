#pragma once
#include "conditions.h"

class groupBasedConditions :
    public Conditions
{
    public:
        explicit groupBasedConditions(ifstream& rFin, int numConditions,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);
        explicit groupBasedConditions(ifstream& rFin,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);
        virtual ~groupBasedConditions(void);

        virtual BOOL initConditions();

        virtual BOOL generateConditions();

        virtual BOOL clearConditions();

        virtual void shuffleConditions(int times);
        
        // get all conditions
        virtual const vector<cond_t *>& getAllConditions();

        // get the specific condition using its index
        virtual cond_t& operator[](int &rhs);

    private:
        int numGroups;

        vector< vector<int> * > constraintGroups;
        vector< vector<int> * > conditionGroups;

        vector<cond_t *> shuffledConditions;

        void updateShuffledConditions();
};
