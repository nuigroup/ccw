#include "ofMain.h"
#include "ccs.h"
#include "ofAppGlutWindow.h"


//========================================================================
int main( ){
	#ifdef TARGET_OSX
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef bundleURL = CFBundleCopyBundleURL(mainBundle);
		CFURLRef containingDir = CFURLCreateCopyDeletingLastPathComponent(NULL,bundleURL);
		char path[1024];
		CFRelease(bundleURL);
		CFRelease(containingDir);
		chdir(path);
	#endif

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 360,320, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ccs());

}
