#ifndef _H_CONFWND_
#define _H_CONFWND_

#include "stdafx.h"
#include <string>
#include <memory>

using namespace std;

// ConfWnd is a singleton
class ConfWnd
{
    public:
        ~ConfWnd();
        static ConfWnd *getInstance();

        INT_PTR displayConfWnd(HINSTANCE ghInstance);
        static INT_PTR CALLBACK confWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);

    private:
        ConfWnd();
        
        static auto_ptr<ConfWnd> m_pInstance;
        string m_secNo;
};


#endif /* _H_CONFWND_ */
