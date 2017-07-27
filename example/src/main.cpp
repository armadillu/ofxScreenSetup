#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

	ofAppGLFWWindow win;
	win.setNumSamples(8);
	//win.setMultiDisplayFullscreen(false);
	//win.setDepthBits(24);

	ofSetupOpenGL(&win, 1024,500, OF_WINDOW);	// <-------- setup the GL context

	ofRunApp( new ofApp());

}
