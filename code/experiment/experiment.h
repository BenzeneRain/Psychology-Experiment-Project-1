#pragma once
#include "stdafx.h"
#include "resource.h"
#include "Screen.h"
#include "TestObject.h"
#include <string>
#include <fstream>
#include <memory>
#include <vector>

using namespace std;

// Experiment should be a singleton as well
class Experiment
{
    public:
        static Experiment *getInstance(HINSTANCE hInstance);
        ~Experiment();

        BOOL startProgram();
        BOOL writeOutputs(string strOutputs);
        BOOL isNewSection(); 
        static void proceedExperiment();

        string subjectID;
        unsigned int maxSecNo;
        unsigned int currSecNo;
        unsigned int experiMode; // 0 -- experiment mode; 1 -- practice mode;
        string outFilename;
        unsigned int trialsInOneSec;
        DEVMODE devMode; // display settings

        vector<Screen *> screens;
        vector<TestObject *> stubObjects;

        //This option is for debugging
        static const int debug;

    private:
        Experiment(HINSTANCE hInstance);
        BOOL initSystem();
        BOOL initOutputFile();
        BOOL closeOutputFile();

        HINSTANCE hInst;
        fstream hFileOut;

        static auto_ptr<Experiment> m_pInstance;


};
