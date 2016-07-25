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
		BORDERLESS_NATIVE_SIZE,
		WINDOWED,
		MONITOR_2,
		MONITOR_3,
		MONITOR_4,
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

	//this only applies to windows, glwf doesnt quite get the window size right
	struct WindowEdgePaddingConfig {
		int xPadding;
		int yOffset;
		WindowEdgePaddingConfig() {
			xPadding = yOffset = 0;
		}

		WindowEdgePaddingConfig(int xp, int ypad) {
			xPadding = xp;
			yOffset = ypad;
		}
	};

	ofxScreenSetup();

	//padding only applies to Windows; GLWF fullscreen somehow gets confused about the Win32 windows edges
	//so we need to compensate with some offsets. Your "Windows theme" also seems to affect this;
	//try passing windows7Pad or windows10Pad
	void setup(	float baseWidth, 
				float baseHeight, 
				ScreenMode mode,
				#ifdef TARGET_WIN32
				WindowEdgePaddingConfig padding = windows10Pad,
				#else
				WindowEdgePaddingConfig padding = WindowEdgePaddingConfig(), //osx && linux get no padding offsets
				#endif
				#ifdef _DEBUG
				bool allowFullscreenDebug = true //if true, you can still atl+tab to VS on windows when a breakpoints hits
				#else							 //but the windows menubar is sometimes on top of the window (so true is only good for debug)														
				bool allowFullscreenDebug = false
				#endif
	);

	void setScreenMode(ScreenMode m);

	vector<string> getModeNames();
	string stringForMode(ScreenMode);

	void cycleToNextScreenMode();
	void drawDebug(); //draws a clear pattern showing all the screen pixels

	ScreenMode & getCurrentScreenMode(){return currentMode;}

	ofEvent<ScreenSetupArg>	setupChanged;

	//gets you all the monitor rects in window space, assuming one large windows that spans across
	//all your monitors (using OF's multiScreenFullscreen)
	static vector<ofRectangle> getAllMonitors();


	ScreenMode currentMode;

	static WindowEdgePaddingConfig windows7ClassicThemePad;
	static WindowEdgePaddingConfig windows7NormalThemePad;
	static WindowEdgePaddingConfig windows10Pad;

private:

	ofVec2f getMainScreenOrigin(); //only in GLFWwindow
	ofVec2f getLeftmostMonitorCoord(); //only in GLFWwindow
	ofVec2f getVirtualTopLeftMonitorCoord();
	ofRectangle getAllMonitorSpace(); //only in GLFWwindow
	ofRectangle getAllMonitorCommonHeightSpace(); //only in GLFWwindow
	int getNumMonitors();
	ofRectangle getRectForMonitor(int monitorID); //only in GLFWwindow

	bool inited;
	int baseW, baseH;
	int currentW, currentH;

	void setFullscreenWindowStyle();

	WindowEdgePaddingConfig paddingConfig;
	bool fullscreenDebugMode; //dictates if borderless fs windows are on top of everything or not

};

#endif /* defined(__BaseApp__ofxScreenSetup__) */
