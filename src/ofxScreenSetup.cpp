//
//  ofxScreenSetup.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 03/07/14.
//
//

#include "ofxScreenSetup.h"
#include "ofAppGLFWWindow.h"


ofxScreenSetup::ofxScreenSetup(){
	inited = false;
}
void ofxScreenSetup::setup(float baseWidth, float baseHeight, ScreenMode mode){

	baseW = baseWidth;
	baseH = baseHeight;
	setScreenMode(mode);
}


vector<string> ofxScreenSetup::getModeNames(){
	vector<string> screenModeNames;
	screenModeNames.push_back("FULL_ALL_MONITORS"); screenModeNames.push_back("FULL_ONE_MONITOR");
	screenModeNames.push_back("BORDERLESS_ONE_MONITOR_W"); screenModeNames.push_back("BORDERLESS_ONE_MONITOR_H");
	screenModeNames.push_back("WINDOWED");
	return screenModeNames;
}


void ofxScreenSetup::cycleToNextScreenMode(){
	ScreenMode tmp = currentMode;
	tmp = ScreenMode( (tmp + 1)%NUM_SCREEN_MODES );
	setScreenMode(tmp);
}


void ofxScreenSetup::setScreenMode(ScreenMode m){

	ScreenSetupArg arg;
	arg.oldMode = currentMode;
	arg.newMode = m;
	arg.oldHeight = ofGetHeight();
	arg.oldWidth = ofGetWidth();

	currentMode = m;

	ofAppGLFWWindow *window = (ofAppGLFWWindow *)ofGetWindowPtr();
	float ar = baseW / (float)baseH;
	float w, h;

	switch (m) {
		case FULL_ALL_MONITORS:
			ofSetFullscreen(false);
			window->setMultiDisplayFullscreen(true);
			ofSetFullscreen(true);
			arg.newWidth = ofGetWidth();
			arg.newHeight = ofGetHeight();
			break;

		case FULL_ONE_MONITOR:
			ofSetFullscreen(false);
			window->setMultiDisplayFullscreen(false);
			ofSetFullscreen(true);
			arg.newWidth = ofGetWidth();
			arg.newHeight = ofGetHeight();
			break;

		case BORDERLESS_ONE_MONITOR_W:
			window->setMultiDisplayFullscreen(true);
			ofSetFullscreen(true);
			w = ofGetScreenWidth() * 0.9; //some margin
			arg.newWidth = w;
			arg.newHeight = w / ar;
			break;

		case BORDERLESS_ONE_MONITOR_H:
			window->setMultiDisplayFullscreen(true);
			ofSetFullscreen(true);
			h = ofGetScreenHeight() * 0.9; //some margin
			arg.newWidth = (h * ar);
			arg.newHeight = h;
			break;

		case WINDOWED:
			ofSetFullscreen(false);
			arg.newWidth = baseW;
			arg.newHeight = baseH;
			break;

		default:
			break;
	}

	ofSetWindowShape(arg.newWidth, arg.newHeight);
	ofSetWindowPosition(0, 0);

	if(inited){
		ofNotifyEvent( setupChanged, arg, this);
	}
	inited = true;
}