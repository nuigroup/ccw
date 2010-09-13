#include "ofxNxmlObject.h"

ofxNxmlObject::ofxNxmlObject(string nxId)
{
	name = nxId;
	
	cout << "Generic UI Object created with params:\n alpha = " << alpha << "\n blendMode = " << blendMode << "\n cacheAsBitmap = " << cacheAsBitmap << "\n height = " << height << "\n name = " << name << "\n rotation = " << rotation << "\n scaleX = " << scaleX << "\n scaleY = " << scaleY << "\n visible = " << visible << "\n width = " << width << "\n x = " << x << "\n y = " << y << "\n";
}

ofxNxmlObject::ofxNxmlObject(float nxAlpha, string nxBlend, bool nxBmpCache, int nxHeight, string nxId, float nxRotn, float nxScaleX, float nxScaleY, bool nxVis, int nxWidth, float nxX, float nxY)
{
	alpha = nxAlpha;
	blendMode = nxBlend;
	cacheAsBitmap = nxBmpCache;
	height = nxHeight;
	name = nxId;
	rotation = nxRotn;
	scaleX = nxScaleX;
	scaleY = nxScaleY;
	visible = nxVis;
	width = nxWidth;
	x = nxX;
	y = nxY;
	
	cout << "Generic UI Object created with params:\n alpha = " << alpha << "\n blendMode = " << blendMode << "\n cacheAsBitmap = " << cacheAsBitmap << "\n height = " << height << "\n name = " << name << "\n rotation = " << rotation << "\n scaleX = " << scaleX << "\n scaleY = " << scaleY << "\n visible = " << visible << "\n width = " << width << "\n x = " << x << "\n y = " << y << "\n";
}