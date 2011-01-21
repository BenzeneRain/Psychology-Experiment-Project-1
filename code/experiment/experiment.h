#pragma once
#include "stdafx.h"
#include "resource.h"
#include "Screen.h"
#include "TestObjectFactory.h"
#include "Conditions.h"
#include "groupBasedConditions.h"
#include <string>
#include <fstream>
#include <memory>
#include <vector>

using namespace std;

enum enumExperiMode {EXPERIMENT = 0, PRACTICE = 1};

// Experiment should be a singleton as well
class Experiment
{
    public:
        static Experiment *getInstance(HINSTANCE hInstance);
        ~Experiment();

        BOOL startProgram();
        BOOL writeOutputs(string& strOutputs);
        BOOL isNewSection(); 

        string subjectID;
        unsigned int maxSecNo;
        unsigned int currSecNo;
        enumExperiMode experiMode; // 0 -- experiment mode; 1 -- practice mode;
        string outFilename;
        unsigned int trialsPerSec;
        unsigned int conditionRepeatTimePerSec;
        DEVMODE devMode; // display settings

        unsigned int currTrialID;

        // Time when starting the experiment.
        // This time is actually not the time exactly starting the 
        // experment, but a little bit earlier when the output filename
        // is automatically updated
        string strDate;
        string strTime;

        Screen  *pScreen;

        Conditions *experimentConditions; 

        //This option is for debugging
        static const int debug;

    private:
        Experiment(HINSTANCE hInstance);
        BOOL initSystem();
        BOOL initOutputFile();
        BOOL closeOutputFile();
        BOOL proceedExperiment();
        BOOL disposeSystem();

        BOOL recordConfigurations();
        BOOL recordConstraints();

        static HINSTANCE hInst;
        fstream hFileOut;

        vector<TestObjectFactory *> objectFactories;

        static auto_ptr<Experiment> m_pInstance;


};
