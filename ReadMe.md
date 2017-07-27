# ofxScreenSetup

[![Build Status](https://travis-ci.org/armadillu/ofxScreenSetup.svg?branch=master)](https://travis-ci.org/armadillu/ofxScreenSetup)

OpenFrameworks addon to handle different window setups; easily switch between this modes for your window:

```
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

```
