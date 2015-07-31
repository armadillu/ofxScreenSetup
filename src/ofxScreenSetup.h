//
//  ofxScreenSetup.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 03/07/14.
//
//

#ifndef __BaseApp__ofxScreenSetup__
#define __BaseApp__ofxScreenSetup__

#include "ofMain.h"

#define AUTO_CASE_CREATE(a) case a: return #a

/*
 
 To Use this:
 
 ofxScreenSetup ss;
 ss.setup(400, 300, WINDOWED); //this defines the Aspect ratio of your project

 //implement callback
 void myApp::setupChanged(ofxofxScreenSetupArg &arg){}

 //add screen changed listener
 ofAddListener(ss.setupChanged, this, &myApp::setupChanged);

 //if you have OFX_REMOTE_UI
 ofxScreenSetup::ScreenMode & screenMode = ss.getCurrentScreenMode();
 OFX_REMOTEUI_SERVER_SHARE_ENUM_PARAM(screenMode, ofxScreenSetup::FULL_ALL_MONITORS, ofxScreenSetup::NUM_SCREEN_MODES - 1, ss.getModeNames());

 //if you want to bind to a key
 ss.cycleToNextScreenMode();

 */


class ofxScreenSetup{

public:

	enum ScreenMode{
		FULL_ALL_MONITORS = 0,
		FULL_ONE_MONITOR,
		BORDERLESS_ONE_MONITOR_W,
		BORDERLESS_ONE_MONITOR_H,
		BORDERLESS_ONE_MONITOR_HALF_H,
		BORDERLESS_ALL_MONITORS_FIT_TO_W,
		BORDERLESS_ALL_MONITORS_FILL_COMMON_HEIGHT,
		WINDOWED,
		NUM_SCREEN_MODES //NOT to be used
	};

	struct ScreenSetupArg{
		int newWidth;
		int newHeight;
		int oldHeight;
		int oldWidth;
		ScreenMode newMode;
		ScreenMode oldMode;
	};

	ofxScreenSetup();
	void setup(float baseWidth, float baseHeight, ScreenMode mode, int verticalOffset = 0);
	void setScreenMode(ScreenMode m);

	vector<string> getModeNames();
	string stringForMode(ScreenMode);

	void cycleToNextScreenMode();

	ScreenMode & getCurrentScreenMode(){return currentMode;}

	ofEvent<ScreenSetupArg>	setupChanged;

	ScreenMode currentMode;

private:

	ofVec2f getMainScreenOrigin(); //only in GLFWwindow
	ofVec2f getLeftmostMonitorCoord(); //only in GLFWwindow
	ofVec2f getVirtualTopLeftMonitorCoord();
	ofRectangle getAllMonitorSpace(); //only in GLFWwindow
	ofRectangle getAllMonitorCommonHeightSpace(); //only in GLFWwindow

	bool inited;
	int baseW, baseH;
	int verticalOffset;

};

#endif /* defined(__BaseApp__ofxScreenSetup__) */
