#pragma once
#include "stdafx.h"
#include <iostream>
#include <cmath>

template<typename T>
class rangeType
{
    public:
        rangeType(void);
        ~rangeType(void);
        
        vector<T> range;
        char type;

        T getRandomValue();

        friend ostream& operator<<(ostream& os, rangeType<T>& rt)
        {
            os << rt.type << " " << rt.range.size();
            for(unsigned int i = 0; i < rt.range.size(); i ++)
            {
                os << " " << rt.range[i];
            }
            os << " ";

            return os;
        }

};

template<typename T>
rangeType<T>::rangeType(void)
{
}

template<typename T>
rangeType<T>::~rangeType(void)
{
}

    template<typename T>
T rangeType<T>::getRandomValue()
{
    switch(this->type)
    {
        case 'S':
            {
                int index = rand() % this->range.size();
                return this->range[index];
            }
        case 'R':
            {
                T interval = this->range[1] - this->range[0];
                // FIX: if T is integer, than we need to use %
                T randNum = this->range[0] + fmod((T)rand(), interval);
                return randNum;
            }
        default:
            break;
    }

    return 0;
}



