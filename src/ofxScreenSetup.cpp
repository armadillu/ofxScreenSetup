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
void ofxScreenSetup::setup(float baseWidth, float baseHeight, ScreenMode mode, int verticalOff){
	verticalOffset = verticalOff;
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


string ofxScreenSetup::stringForMode(ScreenMode m){

	switch (m) {
		AUTO_CASE_CREATE(FULL_ALL_MONITORS);
		AUTO_CASE_CREATE(FULL_ONE_MONITOR);
		AUTO_CASE_CREATE(BORDERLESS_ONE_MONITOR_W);
		AUTO_CASE_CREATE(BORDERLESS_ONE_MONITOR_H);
		AUTO_CASE_CREATE(WINDOWED);
		default: return "ERROR!";
	}
}


ofVec2f ofxScreenSetup::getMainScreenOrigin(){

	ofAppGLFWWindow *windowP = (ofAppGLFWWindow *)ofGetWindowPtr();
	GLFWwindow * windowPtr = windowP->getGLFWWindow();
	int numberOfMonitors;
	GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);

	int xW;	int yW;
	glfwGetWindowPos(windowPtr, &xW, &yW);

	for (int iC = 0; iC < numberOfMonitors; iC++){
		int xM; int yM;
		glfwGetMonitorPos(monitors[iC], &xM, &yM);
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[iC]);
		ofRectangle monitorRect(xM, yM, desktopMode->width, desktopMode->height);
		if (monitorRect.inside(xW, yW)){
			return ofVec2f(xM, yM);
			break;
		}
	}
	return ofVec2f();
}


ofVec2f ofxScreenSetup::getLeftmostMonitorCoord(){

	ofAppGLFWWindow *windowP = (ofAppGLFWWindow *)ofGetWindowPtr();
	GLFWwindow * windowPtr = windowP->getGLFWWindow();
	int numberOfMonitors;
	GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);

	float xMin = FLT_MAX;
	float yMin = FLT_MAX;
	int xW;	int yW;
	glfwGetWindowPos(windowPtr, &xW, &yW);

	for (int iC = 0; iC < numberOfMonitors; iC++){
		int xM; int yM;
		glfwGetMonitorPos(monitors[iC], &xM, &yM);
		if (xMin > xM) xMin = xM;
		if (yMin > yM) yMin = yM;
	}
	return ofVec2f(xMin, yMin);
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
			w = ofGetScreenWidth();
			arg.newWidth = w;
			arg.newHeight = w / ar;
			break;

		case BORDERLESS_ONE_MONITOR_H:
			window->setMultiDisplayFullscreen(true);
			ofSetFullscreen(true);
			h = ofGetScreenHeight();
			arg.newWidth = (h * ar);
			arg.newHeight = h;
			break;

		case WINDOWED:{
			ofSetFullscreen(false);
			float ratioW = ofGetScreenWidth() / float(baseW);
			float ratioH = ofGetScreenHeight() / float(baseH);
			float ratio = MIN(ratioH, ratioW);
			arg.newWidth = baseW * 0.8 * ratio;
			arg.newHeight = baseH * 0.8  * ratio;
			}break;

		default:
			break;
	}
	ofVec2f mainScreenOffset = getMainScreenOrigin();

	ofSetWindowShape(arg.newWidth, arg.newHeight);
	if(m == WINDOWED){
		ofSetWindowPosition(mainScreenOffset.x + 40, mainScreenOffset.y + 40);
	}else{
		if(m == FULL_ALL_MONITORS){
			ofVec2f leftmostCoord = getLeftmostMonitorCoord();
			ofSetWindowPosition(leftmostCoord.x,leftmostCoord.y);
		}else{
			ofSetWindowPosition(mainScreenOffset.x , mainScreenOffset.y);
		}
	}

	if(inited){
		ofNotifyEvent( setupChanged, arg, this);
	}
	inited = true;
}