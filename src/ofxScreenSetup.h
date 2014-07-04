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
	void setup(float baseWidth, float baseHeight, ScreenMode mode);
	void setScreenMode(ScreenMode m);

	vector<string> getModeNames();
	void cycleToNextScreenMode();

	ScreenMode & getCurrentScreenMode(){return currentMode;}

	ofEvent<ScreenSetupArg>	setupChanged;

	ScreenMode currentMode;

private:


	bool inited;
	int baseW, baseH;

};

#endif /* defined(__BaseApp__ofxScreenSetup__) */
