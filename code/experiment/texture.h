#pragma once
#include "stdafx.h"

struct raw_textureStruct
{
    char type;
    string name;
    union
    {
        HBITMAP hBitmap;
        UINT color[3];
    };
};
typedef struct raw_textureStruct rTexture_t;

struct textureStruct
{
    char type;
    string name;
    union
    {
        GLuint textureID;
        UINT color[3];
    };
};
typedef struct textureStruct texture_t;

