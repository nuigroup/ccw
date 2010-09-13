#ifndef _COMMUNITYCOREWEB
#define _COMMUNITYCOREWEB


#include "ofMain.h"
#include "ofxWebServer.h"
#include "ofxWebSimpleGuiToo.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
#include "ofxGuiTypes.h"
#include "ccwConfigResponse.h"	
#include "ccwPlugin.h"	
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

// ========================= Used if config file can't be read
#define CCW_FULL
#define DEFAULT_HTTP_PORT 8080
#define DEFAULT_HTTP_DIRECTORY "www"
#define DEFAULT_DIRECTORY_LISTING "true" // Note: string
#define DEFAULT_INDICES "index.html, index.htm, index.php"
#define DEFAULT_MAX_THREADS 5
#define DEFAULT_IDLE_TIME 10
#define DEFAULT_ALOG "log/access.log"
#define DEFAULT_ELOG "log/error.log"
#define PATH_TO_CONFIG "config/ccw-config.xml"

// ===================== Holds server options
struct ofxWebServerSettings 
{
	int			port; 
	string		directory; 
	bool		dirListEnabled; 
	string		validIndices; 
	string		sslCert; 
	bool		autoRun; 
	int			maxThreads; 
	string		accessLog; 
	string		errorLog; 
	bool		logEnabled;
};

class ccwServer : public ofSimpleApp, public ofxGuiListener {

	public:
	friend class ccwConfigResponse;
	friend class ccwPlugin;
	
	// ===================== Singleton
		static ccwServer* instance();
	
	// ===================== ofx
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
	
	// ===================== Server responses	
		static void		show404(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data);
		static void		show403(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data);
		ccwConfigResponse configResponse;
	
	// ===================== Server behaviour
		vector<ccwPlugin*> activePlugins;
		ofxWebServerSettings httpSettings;

	private:
	// ===================== Singleton handling
		ccwServer(){};
		ccwServer(const ccwServer&);
		ccwServer& operator= (const ccwServer&);
		static ccwServer* pInstance;
	
	
		ofxWebServer	httpServer;
		ofxXmlSettings	setupFile;
		
	// ===================== CCW UI stuff
		void			drawGUI();
		ofxGui *		ccwGui;
		ofxGuiPanel *	p_startStop;
		ofxGuiPanel *	p_settings;
		ofxGuiPanel *	p_ports;
	
		ofxGuiButton *	b_http;
		ofxGuiButton *	b_mySQL;
		ofxGuiButton *  b_webGui;
	
		ofxGuiButton *  b_autoRun;
		ofxGuiButton *	b_log;
		ofxGuiButton *  b_dirList;
		
		ofxGuiSlider *  s_httpPort;
		ofxGuiSlider *  s_webGuiPort;
		
		ofxGuiButton *	b_http80;
		ofxGuiButton *	b_http888;
		ofxGuiButton *	b_http8080;
		
	// ===================== State flags
		bool			httpIsRunning, httpIsLogging, httpPortChanged, startFlag, mySQLDisabled;
		void			resetStateFlags();
	
	// ===================== Main server methods
		void			setupHttpServer();
		void			startHttpServer();
		void			stopHttpServer();
		void			setHttpPort(unsigned int);
		void			handleGui(int parameterId, int task, void* data, int length);
		int				testForPlugins();
	
	// ============== These members will likely be contained in .so/.dll plugins
		void			startMySQLServer();
		void			stopMySQLServer();
		void			startWebGuiServer();
		void			stopWebGuiServer();
		void			populateWebGui();
		bool			mySQLIsRunning;
		ofxWebSimpleGuiToo	webGui;
		bool			webGuiIsRunning;
		u_short			webGuiPort;
	
		map<string,int>	intMap;
		map<string,bool> boolMap;
	
	// ===================== Utility
		bool			resolveBool(string toTest);
		string			resolveBool(bool toTest);
	
	
	// ==== Parses ofxWebServerSettings into format for mongoose
		string			parseServerSettings(ofxWebServerSettings&);
	
		enum
		{
			uiPanel1,
			uiPanel2,
			uiPanel3,
			uiStartStop,
			uiMySQLStartStop,
			uiWebGuiStartStop,
			uiAutoRun,
			uiLog,
			uiDirList,
			uiPort,
			uiPort80,
			uiPort888,
			uiPort8080,
			uiWebGuiPort
		};
};

#endif
