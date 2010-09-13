#ifndef _CCWPLUGIN
#define _CCWPLUGIN

/*
 *  ccwPlugin.h
 *  communitycoreweb
 *
 *  Created by Jake on 12/08/2010.
 *  Copyright 2010 NUI Group. All rights reserved.
 *
 */

#include "ofxXmlSettings.h"

#define ERROR_MSG "PLUGIN XML ERROR"


class ccwPlugin {

public:
	ccwPlugin(string pathToInfo) { readSettings(pathToInfo); }
	string name, version, author, url, description, type, action;
	
private:
	void readSettings(string pathToInfo);
};

#endif