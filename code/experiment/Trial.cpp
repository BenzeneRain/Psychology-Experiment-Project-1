#include "stdafx.h"
#include "Trial.h"
#include "Scene.h"
#include "PreTrialScene.h"
#include "Separate2D3DViewScene.h"
#include "Overlapped2DViewScene.h"
#include "TestObject.h"
#include "experiment.h"

Trial::Trial()
{
    this->trialID = 0;
    this->currState = IDLE;
}

Trial::~Trial(void)
{
}

BOOL Trial::startTrial()
{
    return this->proceedNextScene();
}

BOOL Trial::proceedNextScene()
{
    static auto_ptr<Scene> apScene;
    Scene *pScene;
    BOOL ret;
    BOOL trialFinished = FALSE;

    // go through all scenes
    while(trialFinished == FALSE)
    {
        switch(this->currState)
        {
            case IDLE:
                apScene.reset(new PreTrialScene());
                pScene = apScene.get();        
                this->currState = PRE_TRIAL_SCENE;
                ret = pScene->startScene();
                break;
            case PRE_TRIAL_SCENE:
                apScene.reset(new Separate2D3DViewScene());
                pScene = apScene.get();        
                this->currState = MAIN_SCENE;
                ret = pScene->startScene();
                break;
            case MAIN_SCENE:
                {
                    Experiment *pExperi = Experiment::getInstance(NULL);
                    if(pExperi->experiMode == EXPERIMENT)
                    {
                        this->currState = IDLE;
                        this->stepTrial();
                        trialFinished = TRUE;
                    }
                    else
                    {
                        // In practice mode, so show the result comparison
                        
                        TestObject *pObject = ((dynamic_cast<Separate2D3DViewScene *>(pScene))->pObj);
                        TestObject *pNewObj = pObject->newObj(*pObject);

                        apScene.reset(new Overlapped2DViewScene(*pNewObj));
                        pScene = apScene.get();        
                        this->currState = POST_TRIAL_SCENE;
                        ret = pScene->startScene();

                    }
                    //TODO: Finish the trial
                    break;
                }
            case POST_TRIAL_SCENE:
                this->currState = IDLE;
                this->stepTrial();
                trialFinished = TRUE;
                break;
            default:
                trialFinished = TRUE;
                break;
        }
    }
    return TRUE;
}

BOOL Trial::stepTrial()
{
    Experiment *pExperi = Experiment::getInstance(NULL);

    this->trialID ++;

    if(this->trialID >= pExperi->trialsInOneSec)
    {
        this->trialID = 0;
        pExperi->currSecNo ++;
    }

    return TRUE;
}

// Below are the codes for the singalton
auto_ptr<Trial> Trial::m_pInstance;

Trial *Trial::getInstance()
{
    if(m_pInstance.get() == 0)
    {
        m_pInstance.reset(new Trial);
    }

    return m_pInstance.get();
}
