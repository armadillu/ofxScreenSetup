//
//  ofxScreenSetup.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 03/07/14.
//
//

#include "ofxScreenSetup.h"

#ifndef TARGET_OF_IOS

#define AUTO_CASE_CREATE(a) case a: return #a

#include "GLFW/glfw3.h"
#include "ofAppGLFWWindow.h"

ofxScreenSetup::WindowEdgePaddingConfig ofxScreenSetup::windows7ClassicThemePad = WindowEdgePaddingConfig(4, 23);
ofxScreenSetup::WindowEdgePaddingConfig ofxScreenSetup::windows7NormalThemePad = WindowEdgePaddingConfig(8, 30);
ofxScreenSetup::WindowEdgePaddingConfig ofxScreenSetup::windows10Pad = WindowEdgePaddingConfig(8, 31);

ofxScreenSetup::ofxScreenSetup(){
	inited = false;
}

void ofxScreenSetup::setup(float baseWidth, float baseHeight, ScreenMode mode, WindowEdgePaddingConfig padding, bool allowFullscreenDebug){
	baseW = baseWidth;
	baseH = baseHeight;
	paddingConfig = padding;
	fullscreenDebugMode = allowFullscreenDebug;
	setScreenMode(mode);
}

std::vector<std::string> ofxScreenSetup::getModeNames(){
	std::vector<std::string> screenModeNames;
	screenModeNames.push_back(stringForMode(FULL_ALL_MONITORS));
	screenModeNames.push_back(stringForMode(FULL_ONE_MONITOR));
	screenModeNames.push_back(stringForMode(BORDERLESS_ONE_MONITOR_W));
	screenModeNames.push_back(stringForMode(BORDERLESS_ONE_MONITOR_H));
	screenModeNames.push_back(stringForMode(BORDERLESS_ONE_MONITOR_HALF_H));
	screenModeNames.push_back(stringForMode(BORDERLESS_ALL_MONITORS_FIT_TO_W));
	screenModeNames.push_back(stringForMode(BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT));
	screenModeNames.push_back(stringForMode(BORDERLESS_NATIVE_SIZE));
	screenModeNames.push_back(stringForMode(WINDOWED));
	screenModeNames.push_back(stringForMode(MONITOR_2));
	screenModeNames.push_back(stringForMode(MONITOR_3));
	screenModeNames.push_back(stringForMode(MONITOR_4));
	return screenModeNames;
}


void ofxScreenSetup::cycleToNextScreenMode(){
	ScreenMode tmp = currentMode;
	tmp = ScreenMode( (tmp + 1)%NUM_SCREEN_MODES );
	setScreenMode(tmp);
}


std::string ofxScreenSetup::stringForMode(ScreenMode m){

	switch (m) {
		AUTO_CASE_CREATE(FULL_ALL_MONITORS);
		AUTO_CASE_CREATE(FULL_ONE_MONITOR);
		AUTO_CASE_CREATE(BORDERLESS_ONE_MONITOR_W);
		AUTO_CASE_CREATE(BORDERLESS_ONE_MONITOR_H);
		AUTO_CASE_CREATE(BORDERLESS_ONE_MONITOR_HALF_H);
		AUTO_CASE_CREATE(BORDERLESS_ALL_MONITORS_FIT_TO_W);
		AUTO_CASE_CREATE(BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT);
		AUTO_CASE_CREATE(BORDERLESS_NATIVE_SIZE);
		AUTO_CASE_CREATE(WINDOWED);
		AUTO_CASE_CREATE(MONITOR_2);
		AUTO_CASE_CREATE(MONITOR_3);
		AUTO_CASE_CREATE(MONITOR_4);
		default: return "ERROR!";
	}
}

void ofxScreenSetup::drawDebug() {

	ofPushStyle();
	ofClear(0, 0, 0, 255);
	ofSetLineWidth(5);
	ofSetColor(255);
	ofDrawLine(0, 0, ofGetWidth(), ofGetHeight());
	ofDrawLine(ofGetWidth(), 0, 0, ofGetHeight());
	ofDrawLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
	ofDrawLine(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight()/2);
	ofNoFill();
	ofSetCircleResolution(128);
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, MIN(ofGetWidth(), ofGetHeight()) / 2);
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, MAX(ofGetWidth(), ofGetHeight()) / 2);
	ofSetLineWidth(1);
	ofDrawRectangle(ofGetNativeViewport());
	ofFill();
	ofSetColor(255, 0, 0);
	float s = 100;
	ofDrawRectangle(0, 0, s, s);
	ofDrawRectangle(ofGetWidth() - s, 0, s, s);
	ofDrawRectangle(0, ofGetHeight() - s, s, s);
	ofDrawRectangle(ofGetWidth() - s, ofGetHeight() - s, s, s);
	ofSetColor(255);
	ofNoFill();
	ofDrawRectangle(0, 0, s, s);
	ofDrawRectangle(ofGetWidth() - s, 0, s, s);
	ofDrawRectangle(0, ofGetHeight() - s, s, s);
	ofDrawRectangle(ofGetWidth() - s, ofGetHeight() - s, s, s);
	ofFill();
	std::string msg = stringForMode(currentMode) + " : " + ofToString(currentW) + " x " + ofToString(currentH);
	float scaleUp = 4;
	int xoff = scaleUp * (msg.size() / 2 * 8); //centering it on screen
	int yoff = 12 * 0.5;
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2 - xoff, ofGetHeight() / 2 + yoff);
	ofScale(scaleUp, scaleUp);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapStringHighlight(msg, 0, 0, ofColor::limeGreen, ofColor::black);
	ofPopMatrix();
	ofPopStyle();

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

vector<ofRectangle> ofxScreenSetup::getAllMonitors(bool fixOrigin){

	vector<ofRectangle> retMonitors;
	ofAppBaseWindow * win = ofGetWindowPtr();
	ofRectangle totalRect;
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		for(int i = 0; i< count; i++){
			const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
			int x, y;
			glfwGetMonitorPos(monitors[i], &x, &y);
			ofRectangle monitorRect = ofRectangle( x, y, desktopMode->width, desktopMode->height );
			retMonitors.push_back(monitorRect);
			totalRect = totalRect.getUnion(monitorRect);
		}
	}

	if(fixOrigin){
		for(int i = 0; i < retMonitors.size(); i++){
			retMonitors[i].x -= totalRect.x;
			retMonitors[i].y -= totalRect.y;
		}
	}
	return retMonitors;
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


int ofxScreenSetup::getNumMonitors(){
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	return count;
}


ofRectangle ofxScreenSetup::getRectForMonitor(int monitorID){
	ofRectangle screen;
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if(monitorID >= 0 && monitorID < count){
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[monitorID]);
		int x, y;
		glfwGetMonitorPos(monitors[monitorID], &x, &y);
		screen = ofRectangle(x, y, desktopMode->width, desktopMode->height);
	}
	return screen;
}


ofRectangle ofxScreenSetup::getAllMonitorCommonHeightSpace(){

	ofRectangle allScreensSpace;
	ofAppBaseWindow * win = ofGetWindowPtr();
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){

		ofRectangle commonH;
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		for(int i = 0; i< count; i++){
			const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
			int x, y;
			glfwGetMonitorPos(monitors[i], &x, &y);
			ofRectangle screen = ofRectangle( x, y, desktopMode->width, desktopMode->height );
			ofRectangle temp = ofRectangle(0,screen.y, 100, screen.height);
			if (i == 0) {
				commonH = temp;
			}else {
				commonH = commonH.getIntersection(temp);
			}
			allScreensSpace = allScreensSpace.getUnion(screen);
		}
		allScreensSpace.y = commonH.y;
		allScreensSpace.height = commonH.height;
	}
	return allScreensSpace;
}


void ofxScreenSetup::setScreenMode(ScreenMode m){

	ofRectangle monitorOnly;
	ScreenSetupArg arg;
	arg.oldMode = currentMode;
	arg.newMode = m;
	arg.oldHeight = ofGetHeight();
	arg.oldWidth = ofGetWidth();
	arg.who = this;

	currentMode = m;

	bool cancel = false;
	bool isGLUT = true;
	ofAppBaseWindow * win = ofGetWindowPtr();
	GLFWwindow* glfww = NULL;
	if(dynamic_cast<ofAppGLFWWindow*>(win) != NULL){
		isGLUT = false;
	}

	ofAppGLFWWindow *window = (ofAppGLFWWindow *)ofGetWindowPtr();
	if (!isGLUT) {
		glfww = window->getGLFWWindow();
	}
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
			
		case BORDERLESS_NATIVE_SIZE:
			if(!isGLUT) window->setMultiDisplayFullscreen(true);
			ofSetFullscreen(true);
			arg.newWidth = baseW;
			arg.newHeight = baseH;
			break;

		case WINDOWED:{
			ofSetFullscreen(false);
			arg.newWidth = baseW ;
			arg.newHeight = baseH;
			ofSetWindowShape(baseW, baseH);
			}break;

		case MONITOR_2:
		case MONITOR_3:
		case MONITOR_4:{
			monitorOnly = getRectForMonitor(1 + m - MONITOR_2);
			if(monitorOnly.width > 0.0f && monitorOnly.height > 0.0f){
				ofSetFullscreen(false);
				if(!isGLUT) window->setMultiDisplayFullscreen(false);
				ofSetFullscreen(true);
				arg.newWidth = monitorOnly.width;
				arg.newHeight = monitorOnly.height;
			}else{
				ofLogError("ofxScreenSetup") << "Cant set this mode, no monitor found!";
				return;
			}
		}break;

		default:
			break;
	}

#ifdef TARGET_WIN32
	ofVec2f mainScreenOffset = getMainScreenOrigin();
	//those are magic numbers that I empirically figured out on windows 7 classic theme
	//in windoes 7 basic theme numbers are off!

	int xpad = paddingConfig.xPadding;
	int yoff = paddingConfig.yOffset;
	int bottomYoff = xpad + yoff;

	if (!isGLUT) {
		switch (m) {
			case FULL_ALL_MONITORS: {
				if (!isGLUT) setFullscreenWindowStyle();
				ofVec2f topLeftSpaceCoord = getVirtualTopLeftMonitorCoord();
				glfwSetWindowPos(glfww, topLeftSpaceCoord.x + xpad, topLeftSpaceCoord.y + yoff);
				}break;

			case BORDERLESS_NATIVE_SIZE: 
			case BORDERLESS_ONE_MONITOR_H:
			case BORDERLESS_ONE_MONITOR_W:
			case BORDERLESS_ONE_MONITOR_HALF_H:
				if (!isGLUT) setFullscreenWindowStyle();
				glfwSetWindowPos(glfww, 0 + xpad, 0 + yoff);
				glfwSetWindowSize(glfww, arg.newWidth - 2 * xpad, arg.newHeight - bottomYoff);
				break;

			case BORDERLESS_ALL_MONITORS_FIT_TO_W: {
				if (!isGLUT) setFullscreenWindowStyle();
				ofRectangle allScreensSpace = getAllMonitorSpace();
				glfwSetWindowPos(glfww, allScreensSpace.x + xpad, allScreensSpace.y + yoff);
				glfwSetWindowSize(glfww, arg.newWidth - 2 * xpad, arg.newHeight - bottomYoff);
				}break;

			case BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT: {
				if (!isGLUT) setFullscreenWindowStyle();
				ofRectangle commonHspace = getAllMonitorCommonHeightSpace();
				glfwSetWindowPos(glfww, commonHspace.x + xpad, commonHspace.y + yoff);
				glfwSetWindowSize(glfww, arg.newWidth - 2 * xpad, arg.newHeight - bottomYoff);
				
				}break;

			case WINDOWED: {
				ofSetWindowPosition(mainScreenOffset.x + 40, mainScreenOffset.y + 40);
				}break;

			case MONITOR_2:
			case MONITOR_3:
			case MONITOR_4: {
				if (!isGLUT) setFullscreenWindowStyle();
				glfwSetWindowPos(glfww, monitorOnly.x + xpad, monitorOnly.y + yoff);
				glfwSetWindowSize(glfww, arg.newWidth - 2 * xpad, arg.newHeight - bottomYoff);
				}break;
		}
	}

#endif // TARGET_WIN32

#ifdef TARGET_OSX

	ofSetWindowShape(arg.newWidth, arg.newHeight);

	switch(m){

		case FULL_ALL_MONITORS:{
			ofVec2f topLeftSpaceCoord = getVirtualTopLeftMonitorCoord();
			ofSetWindowPosition(topLeftSpaceCoord.x,topLeftSpaceCoord.y);
			}break;

		case BORDERLESS_NATIVE_SIZE:{
			ofSetWindowPosition(0,0);
			}break;

		case BORDERLESS_ONE_MONITOR_H:
		case BORDERLESS_ALL_MONITORS_FIT_TO_W:{
		case BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT:
			ofVec2f leftmostCoord = getLeftmostMonitorCoord();
			ofSetWindowPosition(leftmostCoord.x,leftmostCoord.y);
			}break;

		case WINDOWED:{
			ofVec2f mainScreenOffset = getMainScreenOrigin();
			ofSetWindowPosition(mainScreenOffset.x + 40, mainScreenOffset.y + 40);
		}break;

		case MONITOR_2:
		case MONITOR_3:
		case MONITOR_4:{
			ofSetWindowPosition(monitorOnly.x, monitorOnly.y);
		}break;

		case BORDERLESS_ONE_MONITOR_W:
		case FULL_ONE_MONITOR:
		case BORDERLESS_ONE_MONITOR_HALF_H:
		default: {
			ofVec2f mainScreenOffset = getMainScreenOrigin();
			ofSetWindowPosition(mainScreenOffset.x , mainScreenOffset.y);
		}break;
	}
#endif
	if(inited){
		ofNotifyEvent( setupChanged, arg, this);
	}
	currentW = arg.newWidth;
	currentH = arg.newHeight;
	inited = true;

	ofLogNotice("ofxScreenSetup") << "changed from " << arg.who->stringForMode(arg.oldMode)
	<< " (" << arg.oldWidth << "x" << arg.oldHeight << ") to " << arg.who->stringForMode(arg.newMode)
	<< " (" << arg.newWidth << "x" << arg.newHeight << ")";

}


void ofxScreenSetup::setFullscreenWindowStyle() {
	#ifdef TARGET_WIN32 //this allows for the debugger to be reachable from a fullscreen breakpoint/crash
	HWND hwnd = ((ofAppGLFWWindow *)ofGetWindowPtr())->getWin32Window();
	SetWindowLong(hwnd, GWL_EXSTYLE, 0);
	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	SetWindowPos(hwnd, fullscreenDebugMode ? HWND_NOTOPMOST : HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	#endif
}

#endif
