/*
 *  ccwPlugin.cpp
 *  communitycoreweb
 *
 *  Created by Jake on 12/08/2010.
 *  Copyright 2010 NUI Group. All rights reserved.
 *
 */

#include "ccwPlugin.h"
using namespace std;

void ccwPlugin::readSettings(string pathToInfo) 
{

	ofxXmlSettings pluginSettings;
	
	if( !pluginSettings.loadFile(pathToInfo)) {
		cout << "[ERROR] " << ofToDataPath(pathToInfo) << " could not be found. Plugin not activated.\n";		
	}
	else { 
		
		cout << "[Plugin Setup] Opened " << pathToInfo << endl;
		
		pluginSettings.pushTag("ccw-plugininfo");
		
		name		= pluginSettings.getValue("name", ERROR_MSG);
		version		= pluginSettings.getValue("version", ERROR_MSG);
		author		= pluginSettings.getValue("author", ERROR_MSG);
		url			= pluginSettings.getValue("url", ERROR_MSG);
		description	= pluginSettings.getValue("description", ERROR_MSG);
		type		= pluginSettings.getValue("type", ERROR_MSG);
		action		= pluginSettings.getValue("action", ERROR_MSG);
	}
}