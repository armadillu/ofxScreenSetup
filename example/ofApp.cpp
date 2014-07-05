#include "ofApp.h"

//keep in mind that this addon requies the use of an ofAppGLFWWindow!
//see how in main.cpp!

void ofApp::setup(){

	ofBackground(22);

	//setup a listener for changed window modes
	ofAddListener(ss.setupChanged, this, &ofApp::setupChanged);

	//start the app in windowed mode
	ss.setup(ofGetWidth(), ofGetHeight(), ofxScreenSetup::WINDOWED);
}

void ofApp::setupChanged(ofxScreenSetup::ScreenSetupArg &arg){

	ofLogNotice()	<< "ofxScreenSetup setup changed from " << ss.stringForMode(arg.oldMode)
	<< " (" << arg.oldWidth << "x" << arg.oldHeight << ") "
	<< " to " << ss.stringForMode(arg.newMode)
	<< " (" << arg.newWidth << "x" << arg.newHeight << ")";
}


void ofApp::draw(){

	ofDrawBitmapString("Press 'w' to cycle through window modes", 20, 20);
}


void ofApp::keyPressed(int key){

	//press 'w' to cycle thorugh modes
	if(key == 'w'){
		ss.cycleToNextScreenMode();
	}
}
