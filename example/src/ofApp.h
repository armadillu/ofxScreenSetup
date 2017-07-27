#pragma once

#include "ofMain.h"
#include "ofxScreenSetup.h"


class ofApp : public ofBaseApp{

public:

	void setup();
	void update(){};
	void draw();

	void keyPressed(int key);

	void setupChanged(ofxScreenSetup::ScreenSetupArg &arg);

	ofxScreenSetup ss;
};
