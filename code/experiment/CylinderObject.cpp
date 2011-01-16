#include "stdafx.h"
#include "TestObject.h"
#include "CylinderObject.h"
#include <string>
#include <sstream>
#include <string>
using namespace std;

const UINT CylinderObject::objectID = 1;

CylinderObject::CylinderObject(vector<GLfloat>& slantRange,
                            vector<GLfloat>& heightRange,
                            vector<GLfloat>& tiltRange,
                            vector<GLfloat>& initZAsptRatioRange,
                            vector<GLfloat>& rotSpeedRange,
                            vector<GLfloat>& maxRotDegRange,
                            vector<texture_t *>& texs,
                            vector<GLfloat>& radiusRange):
    TestObject(slantRange, heightRange, tiltRange, initZAsptRatioRange,
            rotSpeedRange, maxRotDegRange, texs) 
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

    int randIndex;

    // FIX: rand() maybe is not good enough

    randIndex = rand() % this->radiusRange.size();
    this->radius = this->radiusRange[randIndex];
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

string CylinderObject::genObjDesc()
{

    ostringstream ossObj;
    ossObj << TestObject::genObjDesc();

    // Radius Range
    ossObj << this->radiusRange.size();
    for(unsigned int i = 0; i < this->radiusRange.size(); i ++)
    {
        ossObj << " " << this->radiusRange[i];
    }
    ossObj << " ";

    ossObj << 3 << " "; // number of textures used
    ossObj << this->textures[topTextureID]->name << " " // texture for top face
        << this->textures[bottomTextureID]->name << " " // texture for bottom face
        << this->textures[sideTextureID]->name << " "; // texture for side

    return ossObj.str();
}

string CylinderObject::genObjPara()
{
    string strPara;

    strPara = TestObject::genObjPara();

    ostringstream ossObj;

    ossObj << this->radius << " "; // Radius of the Cylinder

    ossObj << 3 << " "; // number of textures used
    ossObj << this->textures[topTextureID]->name << " " // texture for top face
        << this->textures[bottomTextureID]->name << " " // texture for bottom face
        << this->textures[sideTextureID]->name << " "; // texture for side

    strPara += ossObj.str();

    return strPara;
}

void CylinderObject::draw(int drawStyle, BOOL enableTexture)
{
    GLUquadricObj *pCylinder;

    if(enableTexture == FALSE)
        glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glRotatef(90, -1.0f, 0.0f, 0.0f);

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

    pCylinder = gluNewQuadric();
    gluQuadricDrawStyle(pCylinder, drawStyle);
    gluQuadricNormals(pCylinder, GLU_SMOOTH);
    gluQuadricTexture(pCylinder, GL_TRUE);

    // Draw the cylinder
    gluCylinder(pCylinder, this->radius, this->radius, this->height, 32, 32);

    gluDeleteQuadric(pCylinder);

    if(enableTexture)
    {
        if(this->textures[this->sideTextureID]->type == 'T')
            glDisable(GL_TEXTURE_2D);    
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
    gluQuadricTexture(pCylinder, GL_TRUE);

    gluDisk(pCylinder, 0.0f, this->radius, 32, 1);
    gluDeleteQuadric(pCylinder);

    if(enableTexture)
    {
        if(this->textures[this->bottomTextureID]->type == 'T')
            glDisable(GL_TEXTURE_2D);    
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
        gluQuadricTexture(pCylinder, GL_TRUE);

        gluDisk(pCylinder, 0.0f, this->radius, 32, 1);
        gluDeleteQuadric(pCylinder);

        if(enableTexture)
        {
            if(this->textures[this->topTextureID]->type == 'T')
                glDisable(GL_TEXTURE_2D);    
            else
                glColor3ub(255, 255, 255);
        }

        glPopMatrix();
    }

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}
