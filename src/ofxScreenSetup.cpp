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
	screenModeNames.push_back("BORDERLESS_ONE_MONITOR_HALF_H"); screenModeNames.push_back("BORDERLESS_ALL_MONITORS_FIT_TO_W");
	screenModeNames.push_back("BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT");
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
		AUTO_CASE_CREATE(BORDERLESS_ONE_MONITOR_HALF_H);
		AUTO_CASE_CREATE(BORDERLESS_ALL_MONITORS_FIT_TO_W);
		AUTO_CASE_CREATE(BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT);
		AUTO_CASE_CREATE(WINDOWED);
		default: return "ERROR!";
	}
}


ofVec2f ofxScreenSetup::getMainScreenOrigin(){

	ofAppBaseWindow * win = ofGetWindowPtr();

	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

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
	}
	return ofVec2f();
}

ofVec2f ofxScreenSetup::getVirtualTopLeftMonitorCoord(){

	ofAppBaseWindow * win = ofGetWindowPtr();
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

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
	return ofVec2f();
}


ofVec2f ofxScreenSetup::getLeftmostMonitorCoord(){

	ofAppBaseWindow * win = ofGetWindowPtr();
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

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
			if (xMin > xM){
				xMin = xM; yMin = yM;
			}
		}
		return ofVec2f(xMin, yMin);
	}
	return ofVec2f();
}

ofRectangle ofxScreenSetup::getAllMonitorSpace(){

	ofRectangle allScreensSpace;
	ofAppBaseWindow * win = ofGetWindowPtr();
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		for(int i = 0; i< count; i++){
			const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
			int x, y;
			glfwGetMonitorPos(monitors[i], &x, &y);
			ofRectangle screen = ofRectangle( x, y, desktopMode->width, desktopMode->height );
			allScreensSpace = allScreensSpace.getUnion(screen);
		}
	}
	return allScreensSpace;
}

ofRectangle ofxScreenSetup::getAllMonitorCommonHeightSpace(){

	ofRectangle allScreensSpace;
	ofAppBaseWindow * win = ofGetWindowPtr();
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

		vector<float> tops;
		vector<float> bottoms;

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		for(int i = 0; i< count; i++){
			const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
			int x, y;
			glfwGetMonitorPos(monitors[i], &x, &y);
			ofRectangle screen = ofRectangle( x, y, desktopMode->width, desktopMode->height );
			tops.push_back(y + desktopMode->height);
			bottoms.push_back(y);
			allScreensSpace = allScreensSpace.getUnion(screen);
		}

		int maxYIndex = -1;
		float maxY = FLT_MIN;
		int minHeightIndex = -1;
		float minHeight = FLT_MAX;
		for(int i = 0; i < bottoms.size(); i++){
			if (bottoms[i] > maxY){
				maxYIndex = i;
				maxY = bottoms[i];
			}
			if (tops[i] < minHeight){
				minHeight = i;
				minHeight = tops[i];
			}
		}
		cout << "done" << endl;
		allScreensSpace.y = maxY;
		allScreensSpace.height = minHeight;
	}
	return allScreensSpace;
}


void ofxScreenSetup::setScreenMode(ScreenMode m){

	ScreenSetupArg arg;
	arg.oldMode = currentMode;
	arg.newMode = m;
	arg.oldHeight = ofGetHeight();
	arg.oldWidth = ofGetWidth();

	currentMode = m;

	bool isGLUT = true;
	ofAppBaseWindow * win = ofGetWindowPtr();
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){
		isGLUT = false;
	}

	ofAppGLFWWindow *window = (ofAppGLFWWindow *)ofGetWindowPtr();
	float ar = baseW / (float)baseH;
	float w, h;

	switch (m) {
		case FULL_ALL_MONITORS:
			ofSetFullscreen(false);
			if(!isGLUT) window->setMultiDisplayFullscreen(true);
			ofSetFullscreen(true);
			arg.newWidth = ofGetWidth();
			arg.newHeight = ofGetHeight();
			break;

		case FULL_ONE_MONITOR:
			ofSetFullscreen(false);
			if(!isGLUT) window->setMultiDisplayFullscreen(false);
			ofSetFullscreen(true);
			arg.newWidth = ofGetWidth();
			arg.newHeight = ofGetHeight();
			break;

		case BORDERLESS_ONE_MONITOR_W:
			if(!isGLUT) window->setMultiDisplayFullscreen(false);
			w = ofGetScreenWidth();
			arg.newWidth = w;
			arg.newHeight = w / ar;
			ofSetFullscreen(true);
			break;

		case BORDERLESS_ONE_MONITOR_H:
			if(!isGLUT) window->setMultiDisplayFullscreen(false);
			ofSetFullscreen(true);
			h = ofGetScreenHeight();
			arg.newWidth = (h * ar);
			arg.newHeight = h;
			break;

		case BORDERLESS_ONE_MONITOR_HALF_H:
			if(!isGLUT) window->setMultiDisplayFullscreen(false);
			ofSetFullscreen(true);
			h = ofGetScreenHeight() * 0.5;
			arg.newWidth = (h * ar);
			arg.newHeight = h;
			break;

		case BORDERLESS_ALL_MONITORS_FIT_TO_W:{

			if(!isGLUT){ //this mode does nothing in GLUT windows
				window->setMultiDisplayFullscreen(false);
				ofRectangle allScreensSpace = getAllMonitorSpace();
				ofSetFullscreen(true);
				w = allScreensSpace.width;
				arg.newWidth = w;
				arg.newHeight = w / ar;
			}
			}break;

		case BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT:{
			if(!isGLUT){ //this mode does nothing in GLUT windows
				window->setMultiDisplayFullscreen(false);
				ofRectangle allScreensSpace = getAllMonitorCommonHeightSpace();
				ofSetFullscreen(true);
				arg.newWidth = allScreensSpace.width;
				arg.newHeight = allScreensSpace.height;
			}
		}break;

		case WINDOWED:{
			ofSetFullscreen(false);
			arg.newWidth = baseW ;
			arg.newHeight = baseH;
			}break;

		default:
			break;
	}

#ifdef TARGET_WIN32
	ofVec2f mainScreenOffset = getMainScreenOrigin();
	ofSetWindowShape(arg.newWidth, arg.newHeight);

	if(m == WINDOWED){
		ofSetWindowPosition(mainScreenOffset.x + 40, mainScreenOffset.y + 40);
	}else{
		if (m == BORDERLESS_ONE_MONITOR_H || m == BORDERLESS_ONE_MONITOR_W){
			ofSetWindowPosition(0, verticalOffset);
		}else{
			ofSetFullscreen(false);
			ofSetWindowPosition(0, verticalOffset);
			ofSetFullscreen(true);
		}
	}
#endif // TARGET_WIN32

#ifdef TARGET_OSX

	ofVec2f mainScreenOffset = getMainScreenOrigin();
	ofSetWindowShape(arg.newWidth, arg.newHeight);

	switch(m){

		case FULL_ALL_MONITORS:{
			ofVec2f topLeftSpaceCoord = getVirtualTopLeftMonitorCoord();
			ofSetWindowPosition(topLeftSpaceCoord.x,topLeftSpaceCoord.y);
			}break;

		case BORDERLESS_ONE_MONITOR_H:
		case BORDERLESS_ALL_MONITORS_FIT_TO_W:{
		case BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT:
			ofVec2f leftmostCoord = getLeftmostMonitorCoord();
			ofSetWindowPosition(leftmostCoord.x,leftmostCoord.y);
			}break;

		case WINDOWED:
			ofSetWindowPosition(mainScreenOffset.x + 40, mainScreenOffset.y + 40);
			break;

		case BORDERLESS_ONE_MONITOR_W:
		case FULL_ONE_MONITOR:
		case BORDERLESS_ONE_MONITOR_HALF_H:
		default: ofSetWindowPosition(mainScreenOffset.x , mainScreenOffset.y);
			break;
	}
#endif
	if(inited){
		ofNotifyEvent( setupChanged, arg, this);
	}
	inited = true;
}