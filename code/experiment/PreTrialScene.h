#pragma once
#include "purewordscene.h"
#include "Screen.h"

class PreTrialScene :
    public PureWordScene
{
    public:
        PreTrialScene();
        virtual ~PreTrialScene(void);

        virtual BOOL startScene();

    private:
        virtual BOOL renderScene(); 
        virtual BOOL reshape(int w, int h);
};
