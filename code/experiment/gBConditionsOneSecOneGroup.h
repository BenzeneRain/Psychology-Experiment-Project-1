#pragma once
#include "stdafx.h"
#include "groupbasedconditions.h"
#include <numeric>

class gBConditionsOneSecOneGroup :
    public groupBasedConditions
{
    public:
        explicit gBConditionsOneSecOneGroup(ifstream& rFin, int numConditions,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);
        explicit gBConditionsOneSecOneGroup(ifstream& rFin,
                vector<TestObjectFactory *>& rObjectFactories, Screen& rScr);

        virtual ~gBConditionsOneSecOneGroup(void);

        virtual BOOL initConditions();

        virtual BOOL generateConditions();
        virtual BOOL generateAllConditions();

        virtual void shuffleConditions(int times);
        
        // get all conditions
        virtual const vector<cond_t *>& getAllConditions();

        // get the specific condition using its index
        virtual cond_t& operator[](int &rhs);
        
        virtual BOOL clearConditions();

        inline int getTotalSectionNo() {return accumulate(_groupRepeatTimes.begin(), _groupRepeatTimes.end(), 0);}
        inline void stepSection() {this->currSection ++;}

    private:
        void _updateCurrentGroupID();
        void _updateSectionBounds();
        void _shuffleGroups(int times);

        int currSection;
        vector<int> _groupRepeatTimes;
        vector<int> _sectionBounds;
        int currGroupID;

        vector<cond_t *> _currConditions;
};
