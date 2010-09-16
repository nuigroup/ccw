/*
 *  ccwConfigResponse.h
 *  communitycoreweb
 *
 *  Created by Jake on 16/08/2010.
 *  Copyright 2010 NUI Group. All rights reserved.
 *
 */

#ifndef __CCWCONFIGRESPONSE__
#define __CCWCONFIGRESPONSE__

#include "ofxWSRequestHandler.h"

class ccwConfigResponse : public ofxWSRequestHandler {
	public:
	void httpGet(string url);
};

#endif