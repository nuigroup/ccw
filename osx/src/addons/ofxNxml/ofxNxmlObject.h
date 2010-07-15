#ifndef _GENERIC_UI_OBJECT
#define _GENERIC_UI_OBJECT

#include <iostream>
#include <string>
using namespace std;

class ofxNxmlObject {

	public:
		ofxNxmlObject(){}
		~ofxNxmlObject();
		ofxNxmlObject(string);
		ofxNxmlObject(float, string, bool, int, string, float, float, float, bool, int, float, float);
		ofxNxmlObject * stage[];
		ofxNxmlObject * scene[];
		ofxNxmlObject * object[];
	
	protected:
		float alpha;
		string blendMode;
		bool cacheAsBitmap;
		// filters
		int height;
		// mask
		string name;
		// opaquebackground
		float rotation;
		// scale9grid
		float scaleX;
		float scaleY;
		// scrollRect
		// stage
		// transform
		bool visible;
		int width;
		float x;
		float y;
};

#endif