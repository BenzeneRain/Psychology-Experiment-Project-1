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
        enumRotDirection defaultRotDirection,
        vector<texture_t *>& texs,
        rangeType<GLfloat>& radiusRange):
    TestObject(pitchRange, yawRange, rollRange, heightRange,
            initZAsptRatioRange, rotSpeedRange, maxRotDegRange, 
            defaultRotDirection, texs) 
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
    this->_modelCylinderBody(256);
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

void CylinderObject::buildModel(void)
{
    this->_modelCylinderBody(256);
}

void CylinderObject::_modelCylinderBody(int slices)
{
    vector<GLfloat> points;
    GLfloat pi = acos(-1.0f);
    GLfloat deltaDegree = 2.0f * pi / (GLfloat)slices;
    GLfloat currDegree = 1.5f * pi;

    GLfloat xCurrCoord;
    GLfloat yCurrCoord;

    GLfloat height = this->height;

    this->cylinderBodyCoordIndices.clear();
    this->cylinderBodyCoords.clear();

    GLfloat texRandomShift = (float)((float)(rand() % 100) / 100.0);
    GLfloat texXCurrCoord = texRandomShift;
    GLfloat deltaTexCoord = (this->radius * 0.2f * pi / (GLfloat)slices);

    this->cylinderBodyTexCoords.clear();

    for(int i = 0; i < slices; i ++)
    {

        xCurrCoord = cos(currDegree) * this->radius;
        yCurrCoord = sin(currDegree) * this->radius;

        currDegree += deltaDegree;

        this->cylinderBodyCoords.push_back(xCurrCoord);
        this->cylinderBodyCoords.push_back(yCurrCoord);
        this->cylinderBodyCoords.push_back(height);

        this->cylinderBodyTexCoords.push_back(texXCurrCoord);
        this->cylinderBodyTexCoords.push_back(1.0f);

        this->cylinderBodyCoords.push_back(xCurrCoord);
        this->cylinderBodyCoords.push_back(yCurrCoord);
        this->cylinderBodyCoords.push_back(0.0f);

        this->cylinderBodyTexCoords.push_back(texXCurrCoord);
        this->cylinderBodyTexCoords.push_back(0.0f);

        texXCurrCoord += deltaTexCoord;
    }

    for(int i = 0; i < slices - 1; i ++)
    {
        int j = i << 1;

        // First set of the triangle
        this->cylinderBodyCoordIndices.push_back(j);
        this->cylinderBodyCoordIndices.push_back(j + 2);
        this->cylinderBodyCoordIndices.push_back(j + 3);

        // Second set of the triangle
        this->cylinderBodyCoordIndices.push_back(j);
        this->cylinderBodyCoordIndices.push_back(j + 1);
        this->cylinderBodyCoordIndices.push_back(j + 3);
        // The two triangles form a strip of the side
    }

    // last strip
    int j = (slices - 1) << 1;

    // First set of the triangle
    this->cylinderBodyCoordIndices.push_back(j);
    this->cylinderBodyCoordIndices.push_back(0);
    this->cylinderBodyCoordIndices.push_back(1);


    // Second set of the triangle
    this->cylinderBodyCoordIndices.push_back(j);
    this->cylinderBodyCoordIndices.push_back(j + 1);
    this->cylinderBodyCoordIndices.push_back(1);
}

void CylinderObject::_drawCylinderBody(bool enableTexture)
{
    const GLvoid *data = &this->cylinderBodyCoords[0];
    const GLvoid *indices = &this->cylinderBodyCoordIndices[0];
    glEnableClientState(GL_VERTEX_ARRAY);
    if(enableTexture)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        const GLvoid *texture = &this->cylinderBodyTexCoords[0];
        glTexCoordPointer(2, GL_FLOAT, 0, texture);
    }
    glVertexPointer(3, GL_FLOAT, 0, data);
	glPushMatrix();
    glDrawElements(GL_TRIANGLES, this->cylinderBodyCoordIndices.size(),
        GL_UNSIGNED_INT, indices);
	glPopMatrix();
    if(enableTexture)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
	glDisableClientState(GL_VERTEX_ARRAY);
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
    //glTranslatef(0.0f, 0.0f, -halfHeight);
    glTranslatef(0.0f, 0.0f, -this->height);

    if(enableMotion)
    {
        glRotatef(this->currRotDeg, 0.0f, 0.0f, 1.0f);
    }

    
    //The Z-axis for us is actually Y-axis for the object after translation
    glScalef(xStretch, zStretch, yStretch);

    if(enableTexture)
    {
        switch(this->textures[this->sideTextureID]->type)
        {
            case 'T':
                {
                glEnable(GL_TEXTURE_2D);    
                glBindTexture(GL_TEXTURE_2D,
                        this->textures[this->sideTextureID]->textureID);
                glMatrixMode(GL_TEXTURE);
                glLoadIdentity();
                glPushMatrix();

                float a = this->radius * zStretch;
                float b = this->radius * xStretch;

                if(a < b)
                {
                    float c = a;
                    a = b;
                    b = c;
                }

                GLfloat pi = acos(-1.0f);
                GLfloat double_pi = 2.0f * pi;
                GLfloat factor = (GLfloat)(double_pi * b + 4.0 * (a - b)) / (double_pi * this->radius);
                glScalef(factor, 1.0f, 1.0f);
                
                glMatrixMode(GL_MODELVIEW);
                
                this->_drawCylinderBody(true);

                // Ugly solution to draw the brighter strip on the side of the cylinder
                glMatrixMode(GL_TEXTURE);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glDisable(GL_TEXTURE_2D);
                
                GLfloat deltaDegree = 2.0f * pi / (GLfloat)256.0f;
                GLfloat currDegree = 1.5f * pi;

                GLfloat xCurrCoord = cos(currDegree) * (this->radius + 0.01f);
                GLfloat yCurrCoord = sin(currDegree) * (this->radius + 0.01f);

                GLfloat xNextCoord = cos(currDegree + deltaDegree) * (this->radius + 0.01f);
                GLfloat yNextCoord = sin(currDegree + deltaDegree) * (this->radius + 0.01f);
                
                glColor3ub(255, 255, 255);
                glBegin(GL_QUADS);
                    glVertex3f(xCurrCoord, yCurrCoord, this->height);
                    glVertex3f(xNextCoord, yNextCoord, this->height);
                    glVertex3f(xNextCoord, yNextCoord, 0);
                    glVertex3f(xCurrCoord, yCurrCoord, 0);
                glEnd();
              
                break;
                }
            case 'C':
                glDisable(GL_TEXTURE_2D);
                glColor3ub(this->textures[this->sideTextureID]->color[0],
                        this->textures[this->sideTextureID]->color[1],
                        this->textures[this->sideTextureID]->color[2]);

                this->_drawCylinderBody(false);
                glColor3ub(255, 255, 255);
                break;
            default:
                break;
        }
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
