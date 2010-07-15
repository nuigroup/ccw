#ifndef _OFXNXMLFILE
#define _OFXNXMLFILE

#include "ofxNxmlObject.h"
#include "ofxXmlSettings.h"
#include <iostream>
#include <string>
using namespace std;

#define MAX_OBJECT_DEPTH 25  // Max object nesting allowed

class ofxNxmlFile {
	public:
		ofxNxmlFile(string location);
	private:	
		ofxXmlSettings XML;
};

#endif