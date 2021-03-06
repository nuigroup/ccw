#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxWebServer.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
#include "ofxGuiTypes.h"
#include "ofxNxmlFile.h"
#include "ofxNxmlObject.h"
#include <fstream>

class ccs : public ofSimpleApp, public ofxGuiListener {

	public:
		void			setup();
		void			update();
		void			draw();
		void			keyPressed  (int key);
		void			keyReleased(int key);
		void			mouseMoved(int x, int y );
		void			mouseDragged(int x, int y, int button);
		void			mousePressed(int x, int y, int button);
		void			mouseReleased(int x, int y, int button);
		void			windowResized(int w, int h);
	
	private:
		ofxWebServer	server;
		ofxXmlSettings	setupFile;
		
		void			drawGUI();
		ofxGui *		svrGui;
		ofxGuiPanel *	panel1;
		ofxGuiPanel *	panel2;
		ofxGuiButton *	btnOnOff;
		ofxGuiButton *	btnReset;
		ofxGuiButton *  btnAutoRun;
		ofxGuiButton *	btnLog;
		ofxGuiButton *  btnDirList;
		ofxGuiSlider *  sldrPort;
		ofxGuiButton *	btnPort80;
		ofxGuiButton *	btnPort888;
		ofxGuiButton *	btnPort8080;
		ofxGuiFiles *	filesSrvDir;
		
		unsigned int	srvPort;
		string			srvDir;
		bool			srvDirList;
		string			srvIndexes;
		bool			srvAutoStart;
		int				srvMaxThreads;
		int				srvIdleTime;
		string			srvAccessLog;
		string			srvErrorLog;	
		bool			runServerRequest;
		bool			serverIsRunning;
		bool			srvLogging;
	
		void			setupServer();
		void			startServer();
		void			stopServer();
	
		void			handleGui(int parameterId, int task, void* data, int length);
		
		bool			resolveBool(string toTest);

		enum
		{
			ccsPanel1,
			ccsPanel2,
			ccsStartStop,
			ccsReset,
			ccsAutoRun,
			ccsLog,
			ccsDirList,
			ccsPort,
			ccsPort80,
			ccsPort888,
			ccsPort8080,
			ccsSrvDir
		};
};

		const string dummy_tags[] = 
		{ 
			"stopped",
			"started"
		};

#endif
