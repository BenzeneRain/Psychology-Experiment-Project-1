#pragma once
#include "stdafx.h"
#include "StereoViewingFrameV2.h"


const StereoViewingFrame & StereoViewingFrame::operator=(const StereoViewingFrame & vf)
{
	lefteye = vf.lefteye;
	righteye = vf.righteye;
	centereye = vf.centereye;
	screencoords = vf.screencoords;
//	screenleft = vf.screenleft;
//	screenright = vf.screenright;
//	screenbottom = vf.screenbottom;
//	screentop = vf.screentop;
//	farclip = vf.farclip;
//	nearclip = vf.nearclip;
	pixelleft = vf.pixelleft;
	pixelright = vf.pixelright;
	pixelbottom = vf.pixelbottom;
	pixeltop = vf.pixeltop;
	leftscreenshift = vf.leftscreenshift;
	rightscreenshift = vf.rightscreenshift;
	leftscreenrot = vf.leftscreenrot;
	rightscreenrot = vf.rightscreenrot;
	return(*this);
}

void StereoViewingFrame::GetScreenPixels()
{
	pixelleft = glutGet((GLenum) GLUT_WINDOW_X);
	pixelbottom = glutGet((GLenum) GLUT_WINDOW_Y);
	pixelright = pixelleft + glutGet((GLenum) GLUT_WINDOW_WIDTH);
	pixeltop = pixelbottom + glutGet((GLenum) GLUT_WINDOW_HEIGHT);
}



bool StereoViewingFrame::LoadFrame(string filename)
{
	ifstream infile(filename.c_str());
	if(infile.fail()) {
		// do feedback manually outside this
		cout << "couldn't open file for reading" << endl;
		return(false);
	}
	string junk;
	junk.resize(60);
	frame_set = false;
	infile >> junk >> lefteye[0] >> lefteye[1] >> lefteye[2];
	infile >> junk >> righteye[0] >> righteye[1] >> righteye[2];
	centereye = 0.5*(lefteye + righteye);
	//infile >> junk 
	//	>> screenleft >> screenright 
	//	>> screenbottom >> screentop 
	//	>> farclip >> nearclip;
	infile >> junk >> screencoords[0] >> screencoords[1] 
		>> screencoords[2] >> screencoords[3] >> screencoords[4] >> screencoords[5];
	//infile >> junk >> pixelleft >> pixelright >> pixelbottom >> pixeltop;
	infile >> junk >> leftscreenshift[0] >> leftscreenshift[1] >> leftscreenshift[2];
	infile >> junk >> rightscreenshift[0] >> rightscreenshift[1] >> rightscreenshift[2];
	infile >> junk >> leftscreenrot;
	infile >> junk >> rightscreenrot;
	if(infile.good() == false) {
		cout << "error reading calibration parameters" << endl;
		infile.close();
		return(false);
	}
	infile.close();
	
    this->GetScreenPixels();
	MakeProjectionMatrices();

	frame_set = true;
	return(true);
}


bool StereoViewingFrame::SaveFrame(string filename)
{
	cout << "saving calibration info" << endl;
	ofstream outfile;
	outfile.open(filename.c_str());
	if(outfile.fail()) {
		cout << "couldn't open file for writing" << endl;
		return false;
	}
	outfile << "lefteye " << lefteye << endl;
	outfile << "righteye " << righteye << endl;
	outfile << "scr_size " << screencoords << endl;
	//outfile << "scr_size " 
	//	<< screenleft << " " << screenright << " "
	//	<< screenbottom << " " << screentop << " "
	//	<< farclip << " " << nearclip << endl;
	outfile << "scr_pix "
		<< pixelleft << " " << pixelright << " "
		<< pixelbottom << " " << pixeltop << endl;
	outfile << "leftscrshift " << leftscreenshift << endl;
	outfile << "rightscrshift " << leftscreenshift << endl;
	outfile << "leftscrrot " << leftscreenrot << endl;
	outfile << "rightscrrot " << leftscreenrot << endl;
	outfile.close();
	cout << "finished writing calibration info" << endl;

	return(true);
}



void StereoViewingFrame::GenProjectionMatrix(int eyenum)
{
	// eyenum 0: center eye
	// eyenum 1: left eye
	// eyenum 2: right eye

	matrix_4x4 mproj;
	// local copy of eye pos, for switching
	vector_3d eyepos;
	// local copy of screen coords, possibly modified for screen shift
	cml::vector< double, cml::fixed<6> > scrcoord;
	// for adding extra shift
	vector_3d scrshift(0,0,0);
	// for adding rotation of left or right, for dual screen
	double torsion;
	matrix_4x4 torsionmat; 

	switch(eyenum)
	{
	case 1:
		eyepos = lefteye;
		scrshift = leftscreenshift;
		torsion = leftscreenrot; // extra rotation for dual screen
		break;
	case 2:
		eyepos = righteye;
		scrshift = rightscreenshift;
		torsion = rightscreenrot; // extra rotation for dual screen
		break;
	default:
		eyepos = centereye;
		scrshift[0] = 0; scrshift[1] = 0; scrshift[2] = 0;
		torsion = 0.0;
	}

	// get loca copy of screen coords
	// and add shift (which may be zero)
	scrcoord = screencoords;
	// optional extra shift for dual screen
	scrcoord[0] -= scrshift[0];
	scrcoord[1] -= scrshift[0];
	scrcoord[2] -= scrshift[1];
	scrcoord[3] -= scrshift[1];
	scrcoord[4] -= scrshift[2];
	scrcoord[5] -= scrshift[2];


	// this assumes that screen is at depth of 0
	// positive depth pointing out of the screen toward viewer
	// far and near clipping planes defined relative to screen
	// (so far is negative number, near is generally small positive)
	// eye position relative to screen center

	// width and height of frustum specify values at screen distance
	// eye position defined by eyepos
	// near and far clipping are the Near() and Far() of 3d box

	double m00 = 2.0*eyepos[2]/(scrcoord[1] - scrcoord[0]); 
	double m11 = 2.0*eyepos[2]/(scrcoord[3] - scrcoord[2]); 
	// the eye position subtracted is to adjust for when 
	// eye position is not centered relative to clipping box
	// 2.0 because the average of right and left not normalized	
	double A = 
		(scrcoord[1] + scrcoord[0] - 2.0*eyepos[0])/
		(scrcoord[1] - scrcoord[0]);
	double B = 	
		(scrcoord[3] + scrcoord[2] - 2.0*eyepos[1])/
		(scrcoord[3] - scrcoord[2]);

	double C,D;
	// NOTE: The signs of C and D were changed to fix inconsistency
	// with depth testing ( -N and -F get mapped to -1 and 1 respectively,
	// rather than 1 and -1, as I had done previously)
	// This affects only depth, not screen coords of projection
	if(eyepos[2] > scrcoord[5]) {
		// this is the normal case, in which clipping past eye
		C =  -(2.0*eyepos[2] - scrcoord[4] - scrcoord[5])/
			(scrcoord[5] - scrcoord[4]);
		D =  -2.0*(eyepos[2] - scrcoord[4])*(eyepos[2] - scrcoord[5])/
			(scrcoord[5] - scrcoord[4]);
	}
	else {
		// if bad near clipping plane (behind eye)
		// use 80% of distance to eye as near clipping plane
		C =  -(2.0*eyepos[2] - scrcoord[4] - 0.8*eyepos[2])/
			(0.8*eyepos[2] - scrcoord[4]);
		D =  -2.0*(eyepos[2] - scrcoord[4])*(eyepos[2] - 0.8*eyepos[2])/
			(0.8*eyepos[2] - scrcoord[4]);
	}
		

	mproj(0,0) = m00;
	mproj(0,1) = 0;
	mproj(0,2) = 0;
	mproj(0,3) = 0;

	mproj(1,0) = 0;
	mproj(1,1) = m11;
	mproj(1,2) = 0;
	mproj(1,3) = 0;

	mproj(2,0) = A;
	mproj(2,1) = B;
	mproj(2,2) = C;
	mproj(2,3) = -1.0;

	mproj(3,0) = -m00*eyepos[0] - eyepos[2]*A;
	mproj(3,1) = -m11*eyepos[1] - eyepos[2]*B;
	mproj(3,2) = D - C*eyepos[2];
	mproj(3,3) = eyepos[2];

	// matrix is:
	//	m00,0,0,0,
	//	0,m11,0,0,
	//	A,B,C,-1.0,
	//	-m00*eyeposx - eyeposz*A, 
	//	-m11*eyeposy - eyeposz*B, 
	//	D - C*eyeposz, eyeposz


	torsionmat(0,0) = cos(torsion);
	torsionmat(0,1) = sin(torsion);
	torsionmat(0,2) = 0.0;
	torsionmat(0,3) = 0.0;
	torsionmat(1,0) = -sin(torsion);
	torsionmat(1,1) = cos(torsion);
	torsionmat(1,2) = 0.0;
	torsionmat(1,3) = 0.0;
	torsionmat(2,0) = 0.0;
	torsionmat(2,1) = 0.0;
	torsionmat(2,2) = 1.0;
	torsionmat(2,3) = 0.0;
	torsionmat(3,0) = 0.0;
	torsionmat(3,1) = 0.0;
	torsionmat(3,2) = 0.0;
	torsionmat(3,3) = 1.0;

	mproj = torsionmat * mproj;

	switch(eyenum)
	{
	case 1:
		leftprojmatrix = mproj;
		break;
	case 2:
		rightprojmatrix = mproj;
		break;
	default:
		centerprojmatrix = mproj;
	}
}
