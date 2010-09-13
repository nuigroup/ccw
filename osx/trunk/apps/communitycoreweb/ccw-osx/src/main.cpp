#include "ofMain.h"
#include "ccwServer.h"
#include "ofAppGlutWindow.h"


//========================================================================
int main( ){
	
	#ifdef TARGET_OSX    
	// Get the absolute location of the executable file in the bundle.
	CFBundleRef appBundle     = CFBundleGetMainBundle();
	CFURLRef   executableURL = CFBundleCopyExecutableURL(appBundle);
	char execFile[4096];
	if (CFURLGetFileSystemRepresentation(executableURL, TRUE, (UInt8 *)execFile, 4096))
	{
		// Strip out the filename to just get the path
		string strExecFile = execFile;
		int found = strExecFile.find_last_of("/");
		string strPath = strExecFile.substr(0, found);
		
		// Change the working directory to that of the executable
		if(-1 == chdir(strPath.c_str())) {
			ofLog(OF_LOG_ERROR, "Unable to change working directory to executable's directory.");
		}
	}
	else {
		ofLog(OF_LOG_ERROR, "Unable to identify executable's directory.");
	}
	CFRelease(executableURL);
	#endif
	
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 360,320, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( ccwServer::instance());

}
