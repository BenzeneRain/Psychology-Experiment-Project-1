#ifndef _H_SCREEN_
#define _H_SCREEN_
#include "stdafx.h"
#include <string>
using namespace std;

class Screen
{
    public:
        Screen();
        ~Screen();

        BOOL initGlut(DEVMODE devMode,
            UINT displayMode, string title);

        BOOL cancelKMBinds();
        BOOL clear();
        BOOL displayString(string str, float x, float y);

        // the function is only for test purpose 
        static void testRenderScene();
    private:

        UINT displayMode;
        
};

#endif /* _H_SCREEN_ */
