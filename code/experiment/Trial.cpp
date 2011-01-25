#include "stdafx.h"
#include "Trial.h"
#include "Scene.h"
#include "PreTrialScene.h"
#include "Separate2D3DViewScene.h"
#include "Overlapped2DViewScene.h"
#include "TestObject.h"
#include "Conditions.h"
#include "experiment.h"

#include <vector>
#include <sstream>
#include <string>

using namespace std;

Trial::Trial(int trialID, cond_t& cond):
    condition(cond)
{
    this->trialID = trialID;
    this->currState = IDLE;

    this->_results.clear();
}

Trial::~Trial(void)
{
    for(unsigned int i = 0; i < this->_results.size(); i ++)
    {
        trialResult_t *pResult = this->_results.back();
        this->_results.pop_back();
        delete pResult;
    }
}

BOOL Trial::startTrial()
{
    BOOL ret = TRUE;

    //Proceed to the next scene if
    //the trial is not finished and 
    //each scene is executed correctly
    while(this->currState != FINISHED &&
            ret == TRUE)
    {
        ret = this->proceedNextScene();
    }

    return ret;
}

BOOL Trial::proceedNextScene()
{
    Scene *pScene;
    BOOL ret = TRUE;
    Experiment *pExperi = Experiment::getInstance(NULL);

    switch(this->currState)
    {
        case IDLE:
            {
                this->currState = PRE_TRIAL_SCENE;

                break;
            }
        case PRE_TRIAL_SCENE:
            {
                //pScene = new PreTrialScene();
                //ret = pScene->startScene();
                //delete pScene;

                this->currState = MAIN_SCENE;
                break;
            }
        case MAIN_SCENE:
            {
                pScene = new Separate2D3DViewScene(this->condition);
                ret = pScene->startScene();
                
                trialResult_t *pResult = new trialResult_t(
                        pScene->getFps(), pScene->getDuration(), string("Separate 2D 3D View Scene"));
                this->_results.push_back(pResult);
                delete pScene;

                this->currState = POST_TRIAL_SCENE;
                break;
            }
        case POST_TRIAL_SCENE:
            {
                // Write the trial result to the output file
                // if it is experiment mode
                if(pExperi->experiMode == EXPERIMENT)
                    this->recordTrialInfo();
                else
                {
                    pScene = new Overlapped2DViewScene(this->condition);
                    ret = pScene->startScene();
                    delete pScene;
                }

                this->currState = FINISHED;
                break;
            }
        case FINISHED:
            {
                break;
            }
        default:
            return FALSE;
            break;
    }
    return ret;
}

BOOL Trial::recordTrialInfo()
{
    Experiment *pExperi = Experiment::getInstance(NULL);

    TestObject& rObject = *this->condition.pRealObject;
    ostringstream ossTI; // TI = Trial Information  

    if(pExperi->currSecNo == 0 && this->trialID == 0)
    {
        ossTI << "Section No\t| Trial ID\t| Condition Group ID\t| Condition ID\t| "
            << "Motion\t| Object Display Duration\t| Object Disappear Duration\t| "
            << rObject.genObjParaTitle() << " FPS & Reaction Time\t|" << endl;
    }

    ossTI << pExperi->currSecNo + 1 << "\t\t  "; // Section Number
    ossTI << this->trialID + 1 << "\t\t  "; // Trial ID
    ossTI << this->condition.constraintGroupID << "\t\t\t  ";
    ossTI << this->condition.constraintID << "\t\t  ";

    if(this->condition.dispMode == CONTINUOUS_DISPLAY)
        ossTI << "1";
    else
        ossTI << "0";
    ossTI << "\t\t  ";

    streamsize oldPrec = ossTI.precision(2);
    ossTI << this->condition.secDisplay << "\t\t\t\t  ";
    ossTI << this->condition.secBlackScreen << "\t\t\t\t  ";

    ossTI << rObject.genObjPara();
    for(unsigned int i = 0; i < this->_results.size(); i ++)
    {
        trialResult_t *pResult = this->_results[i];

        ossTI.precision(2);
        ossTI << fixed << " " << pResult->fps;

        ossTI.precision(6);
        ossTI << fixed << "   " << pResult->duration << "   ";
    }
    ossTI << "\t  " << endl;

    ossTI.precision(oldPrec);
    pExperi->writeOutputs(ossTI.str());

    return TRUE;
}

