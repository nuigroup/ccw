#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "../../../../addons/ofxHttpServer/src/ofxHTTPServer.h"
#include "../../../../addons/ofxXmlSettingsTknMod/src/ofxXmlSettings.h"
#include "../../../../addons/ofxGui/ofxGui.h"
#include "../../../../addons/ofxGui/ofxGuiTypes.h"
#include "../../../../addons/ofxNxml/ofxNxmlFile.h"
#include "../../../../addons/ofxNxml/ofxNxmlObject.h"
#include <fstream>

class ccw : public ofSimpleApp, public ofxGuiListener {

	public:
		~ccw();
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

		void			getRequest(ofxHTTPServerResponse & response);
		void			postRequest(ofxHTTPServerResponse & response);

		ofstream		logFile;

	private:
		ofxHTTPServer *	server;

		void			drawGUI();
		ofxGui *		svrGui;
		ofxGuiPanel *	panel1;
		ofxGuiPanel *	panel2;
		ofxGuiButton *	svrOnOffBtn;
		ofxGuiButton *	svrResetBtn;
		ofxGuiButton *	svrCtrlHide;

		u_short			srvPort;
		bool			runServerRequest;
		bool			serverIsRunning;
		bool			autoStart;
		bool			logging;

		void			setupServer();
		void			startServer(u_short);
		void			stopServer();

		void			handleGui(int parameterId, int task, void* data, int length);

		enum
		{
			ccwPanel1,
			ccwPanel2,
			ccwStartStop,
			ccwReset,
			ccwHide
		};
};

		const string dummy_tags[] =
		{
			"stopped",
			"started"
		};

#endif
