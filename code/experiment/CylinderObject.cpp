#include "stdafx.h"
#include "TestObject.h"
#include "CylinderObject.h"
#include <string>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

const UINT CylinderObject::objectID = 1;

CylinderObject::CylinderObject(rangeType<GLfloat>& pitchRange,
        rangeType<GLfloat>& yawRange,
        rangeType<GLfloat>& rollRange,
        rangeType<GLfloat>& heightRange,
        rangeType<GLfloat>& initZAsptRatioRange,
        rangeType<GLfloat>& rotSpeedRange,
        rangeType<GLfloat>& maxRotDegRange,
        vector<texture_t *>& texs,
        rangeType<GLfloat>& radiusRange):
    TestObject(pitchRange, yawRange, rollRange, heightRange,
            initZAsptRatioRange, rotSpeedRange, maxRotDegRange, texs) 
{
    this->radiusRange = radiusRange;    
    this->topTextureID = 0;
    this->bottomTextureID = 1;
    this->sideTextureID = 2;

    this->objName = string("Cylinder");
}

CylinderObject::CylinderObject(CylinderObject &rObj) : TestObject(rObj)
{
    this->objName = string("Cylinder");
    this->radius = rObj.radius;
}

CylinderObject::~CylinderObject(void)
{
}

TestObject *CylinderObject::newObj(TestObject &rObject)
{
    return (new CylinderObject(static_cast< CylinderObject& >(rObject))); 
}

string CylinderObject::getObjName(void)
{
    return this->objName;
}

UINT CylinderObject::getObjID()
{
    return this->objectID;
}

void CylinderObject::setRandPara()
{
    TestObject::setRandPara();
    // FIX: rand() maybe is not good enough

    this->radius = this->radiusRange.getRandomValue();
}

BOOL CylinderObject::adjustAsptRatio(GLfloat delta)
{
    if(adjZAsptRatio + delta <= 0)
    {
        return FALSE;
    }
    else
    {
        adjZAsptRatio += delta;
        return TRUE;
    }
}

string CylinderObject::genObjDescTitle()
{
    ostringstream ossObj;
    ossObj << TestObject::genObjDescTitle();

    // Radius Range
    ossObj << "Radius Range" << " | ";

    ossObj << "Textures(Top Bottom Side)" << " | ";

    return ossObj.str();
}

string CylinderObject::genObjDesc()
{

    ostringstream ossObj;
    ossObj << TestObject::genObjDesc();

    // Radius Range
    ossObj << this->radiusRange << "\t  ";

    ossObj << 3 << " "; // number of textures used
    ossObj << this->textures[topTextureID]->name << " " // texture for top face
        << this->textures[bottomTextureID]->name << " " // texture for bottom face
        << this->textures[sideTextureID]->name << "\t  "; // texture for side

    return ossObj.str();
}

string CylinderObject::genObjParaTitle()
{
    ostringstream ossObj;

    ossObj << TestObject::genObjParaTitle();
    ossObj << "Radius" << "\t| "; // Radius of the Cylinder

    ossObj << "Textures(Top Bottom Side)" << "\t| ";

    return ossObj.str();
}

string CylinderObject::genObjPara()
{
    string strPara;

    strPara = TestObject::genObjPara();

    ostringstream ossObj;

    ossObj << this->radius << "\t\t  "; // Radius of the Cylinder

    ossObj << 3 << " "; // number of textures used
    ossObj << this->textures[topTextureID]->name << " " // texture for top face
        << this->textures[bottomTextureID]->name << " " // texture for bottom face
        << this->textures[sideTextureID]->name << "\t\t\t  "; // texture for side

    strPara += ossObj.str();

    return strPara;
}

void CylinderObject::draw(int drawStyle,
        BOOL enableTexture,
        BOOL enablePYRRotation,
        BOOL enableMotion,
        GLfloat xStretch,
        GLfloat yStretch,
        GLfloat zStretch,
        GLfloat xOffset,
        GLfloat yOffset,
        GLfloat zOffset)
{
    GLUquadricObj *pCylinder;
    GLfloat halfHeight = this->height / 2.0f;
    int slices = 256;

    if(enableTexture == FALSE)
        glDisable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(-xOffset, yOffset, -zOffset);
    // Apply the pitch, yaw and roll (suppose the coordination for us doesn't change
    // by rotation that x is from left to right, y is from down to up, 
    // and z is from far to near)
    if(enablePYRRotation)
    {
        glRotatef(this->pitch, 1.0f, 0.0f, 0.0f); // pitch, rotate refer to x-axis
        glRotatef(this->yaw, 0.0f, cos(this->pitch), sin(this->pitch)); // yaw, rotate refer to y-axis
        glRotatef(this->roll, sin(this->yaw), 
                sin(this->pitch) * cos(this->yaw), 
                cos(this->pitch) * cos(this->yaw)); // roll, rotate refer to z-axis
    }

    // Make the cylinder stand on the x-z plane
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -halfHeight);

    if(enableMotion)
    {
        glRotatef(this->currRotDeg, 0.0f, 0.0f, 1.0f);
    }


    if(enableTexture)
    {
        switch(this->textures[this->sideTextureID]->type)
        {
            case 'T':
                glEnable(GL_TEXTURE_2D);    
                glBindTexture(GL_TEXTURE_2D,
                        this->textures[this->sideTextureID]->textureID);
                break;
            case 'C':
                glDisable(GL_TEXTURE_2D);
                glColor3ub(this->textures[this->sideTextureID]->color[0],
                        this->textures[this->sideTextureID]->color[1],
                        this->textures[this->sideTextureID]->color[2]);
                break;
            default:
                break;
        }
    }

    // The Z-axis for us is actually Y-axis for the object after translation
    glScalef(xStretch, zStretch, yStretch);

    pCylinder = gluNewQuadric();
    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);
    if(enableTexture && this->textures[this->sideTextureID]->type == 'T')
    {
        gluQuadricTexture(pCylinder, GL_TRUE);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glPushMatrix();

        float a = this->radius * zStretch;
        float b = this->radius * xStretch;
        
       // glScalef((GLfloat)ceil(b + (a - b) / (4.0 * atan(1.0))),
       //     1.0f,1.0f);
        glScalef((GLfloat)(b + (a - b) / (4.0 * atan(1.0))),
            1.0f,1.0f);
     }

    // Draw the cylinder
    gluCylinder(pCylinder, this->radius, this->radius, this->height, slices, 32);
    gluDeleteQuadric(pCylinder);

    if(enableTexture)
    {
        if(this->textures[this->sideTextureID]->type == 'T')
        {
            glMatrixMode(GL_TEXTURE);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glDisable(GL_TEXTURE_2D);      
        }
        else
            glColor3ub(255, 255, 255);
    }

    // Draw the bottom face
    if(enableTexture)
    {
        switch(this->textures[this->bottomTextureID]->type)
        {
            case 'T':
                glEnable(GL_TEXTURE_2D);    
                glBindTexture(GL_TEXTURE_2D,
                        this->textures[this->bottomTextureID]->textureID);
                glMatrixMode(GL_TEXTURE);
                glLoadIdentity();
                glScalef(xStretch, zStretch, yStretch);
                break;
            case 'C':
                glDisable(GL_TEXTURE_2D);
                glColor3ub(this->textures[this->bottomTextureID]->color[0],
                        this->textures[this->bottomTextureID]->color[1],
                        this->textures[this->bottomTextureID]->color[2]);
                break;
            default:
                break;
        }
    }

    pCylinder = gluNewQuadric();
    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);
    if(enableTexture && this->textures[this->bottomTextureID]->type == 'T')
        gluQuadricTexture(pCylinder, GL_TRUE);

    gluDisk(pCylinder, 0.0f, this->radius, slices, 1);
    gluDeleteQuadric(pCylinder);

    if(enableTexture)
    {
        if(this->textures[this->bottomTextureID]->type == 'T')
        {
            glMatrixMode(GL_TEXTURE);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glDisable(GL_TEXTURE_2D);    
        }
        else
            glColor3ub(255, 255, 255);
    }

    {
        // Draw the top face
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, this->height);

        if(enableTexture)
        {
            switch(this->textures[this->topTextureID]->type)
            {
                case 'T':
                    glEnable(GL_TEXTURE_2D);    
                    glBindTexture(GL_TEXTURE_2D,
                            this->textures[this->topTextureID]->textureID);
                    glMatrixMode(GL_TEXTURE);
                    glLoadIdentity();
                    glScalef(xStretch, zStretch, yStretch);
                    break;
                case 'C':
                    glDisable(GL_TEXTURE_2D);
                    glColor3ub(this->textures[this->topTextureID]->color[0],
                            this->textures[this->topTextureID]->color[1],
                            this->textures[this->topTextureID]->color[2]);
                    break;
                default:
                    break;
            }
        }

        pCylinder = gluNewQuadric();
        gluQuadricDrawStyle(pCylinder, drawStyle);
        gluQuadricNormals(pCylinder, GLU_SMOOTH);
        if(enableTexture && this->textures[this->topTextureID]->type == 'T')
            gluQuadricTexture(pCylinder, GL_TRUE);

        gluDisk(pCylinder, 0.0f, this->radius, slices, 1);
        gluDeleteQuadric(pCylinder);

        if(enableTexture)
        {
            if(this->textures[this->topTextureID]->type == 'T')
            {
                glMatrixMode(GL_TEXTURE);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glDisable(GL_TEXTURE_2D);    

            }
            else
                glColor3ub(255, 255, 255);
        }

        glPopMatrix();
    }

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void CylinderObject::draw2D(int drawStyle,
        GLfloat xStretch,
        GLfloat yStretch,
        GLfloat zStretch,
        GLfloat xOffset,
        GLfloat yOffset,
        GLfloat zOffset)
{
    GLUquadricObj *pCylinder;
    int slices = 256;

    glPushMatrix();

    glTranslatef(-xOffset, yOffset, -zOffset);

    // The Z-axis for us is actually Y-axis for the screen
    glScalef(xStretch, zStretch, yStretch);

    pCylinder = gluNewQuadric();
    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);

    gluDisk(pCylinder, 0.0f, this->radius, slices, 1);
    gluDeleteQuadric(pCylinder);

    glPopMatrix();
}
