
#ifndef JEFF_STEREO_VIEWING_FRAME
#define JEFF_STEREO_VIEWING_FRAME

#include "cml_config.h"         // Must be first (for now)!
#include <cmath>
#include <cml/cml.h>

#include <string>
#include <fstream>
using namespace std;

// make typedef for fixed vector classes to use here
typedef cml::vector< double, cml::fixed<3> > vector_3d;
typedef cml::matrix<double, cml::fixed<4,4>, cml::col_basis> matrix_4x4;

#include <gl/freeglut.h>

class StereoViewingFrame
{
public:
	StereoViewingFrame() : 
		lefteye(-6,0,50), righteye(6,0,50), centereye(0,0,50), 
		//screencoords(-20,20,-15,15,-100,20),
		//screenleft(-20), screenright(20),
		//screenbottom(-15), screentop(15), 
		//farclip(-100), nearclip(20),
		pixelleft(0), pixelright(1679), pixelbottom(0), pixeltop(1049),
		leftscreenshift(0,0,0), rightscreenshift(0,0,0),
		leftscreenrot(0),rightscreenrot(0),
		frame_set(false)
		{ screencoords[0] = -20; screencoords[1] = 20; screencoords[2] = -15; screencoords[3] = 15;
		  screencoords[4] = -100; screencoords[5] = 40; }
	StereoViewingFrame(const StereoViewingFrame & vf) { *this = vf; }
	const StereoViewingFrame & operator=(const StereoViewingFrame & vf);	
	bool LoadFrame(string filename);
	bool SaveFrame(string filename);
	void GetScreenPixels(); // use GL to get screen pixels
	void MakeProjectionMatrices()
		{ GenProjectionMatrix(0); GenProjectionMatrix(1); GenProjectionMatrix(2); }
public: 
	// data for storing coordinates
	vector_3d lefteye;
	vector_3d righteye;
	vector_3d centereye;
    
	matrix_4x4 leftprojmatrix;
	matrix_4x4 centerprojmatrix;
	matrix_4x4 rightprojmatrix;
	unsigned int pixelleft;
	unsigned int pixelright;
	unsigned int pixelbottom;
	unsigned int pixeltop;
	cml::vector< double, cml::fixed<6> > screencoords;
	//double screenleft;
	//double screenright;
	//double screenbottom;
	//double screentop;
	//double nearclip;
	//double farclip;
	vector_3d leftscreenshift;
	vector_3d rightscreenshift;
	double leftscreenrot;
	double rightscreenrot;
	bool frame_set; // flag for checking if calibrated
private:
	// utility function for creating the projection matrix
	void GenProjectionMatrix(int eyenum);
};



#endif



