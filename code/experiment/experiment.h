#pragma once
#include "stdafx.h"
#include "resource.h"
#include <string>

using namespace std;

class Experiment
{
    public:
        Experiment(HINSTANCE hInstance);
        ~Experiment();
        BOOL startProgram();

        string subjectID;
        unsigned int maxSecNo;
        unsigned int experiMode; // 0 -- experiment mode; 1 -- practice mode;
        string outFilename;
        unsigned int trialsInOneSec;
        DWORD refreshRate;
        DWORD srcHeight;
        DWORD srcWidth;
        DWORD bpp; // bits per pixel

    private:
        BOOL initSystem();
        HINSTANCE hInst;
};
