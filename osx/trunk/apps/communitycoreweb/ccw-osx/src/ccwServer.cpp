
#include "ccwServer.h"


// ========================= Singleton handling
ccwServer* ccwServer::pInstance = NULL;

ccwServer* ccwServer::instance()
{
	if (!pInstance)
		pInstance = new ccwServer;
	
	return pInstance;
}




// ========================= Setup must be in this order
void ccwServer::setup()
{
	resetStateFlags();
	
#ifdef TARGET_OSX
	char currPath[FILENAME_MAX]; // OSX: Must be extracted to /Applications/CCW for MySQL to work
	if (getcwd(currPath, sizeof(currPath)))
	{
			if (currPath != "/Applications/CCW/CommunityCoreWeb.app/Contents/MacOS" && currPath != "/Applications/CCW/CommunityCoreWebDebug.app/Contents/MacOS")
			{
				cerr << "[ERROR] Please extract CCW to /Applications/CCW. You will likely have problems running MySQL this way." << endl;
				mySQLDisabled = true;
			}
	}
#endif	   
		   
	ccwGui = ofxGui::Instance(this); // Instance handle
	
	testForPlugins();
	setupHttpServer();
	drawGUI();
	populateWebGui();
}




// ========================= All flags are false to begin
void ccwServer::resetStateFlags()
{
	httpIsRunning = httpIsLogging = httpPortChanged = startFlag = mySQLDisabled = mySQLIsRunning = webGuiIsRunning = false;
}




// ========================= Check UI is instantiated before accessing its members
void ccwServer::update()
{	
	if (startFlag) 
	{
		startFlag = 0;
		startHttpServer();
	}
}



// ========================= Read settings from config xml file
void ccwServer::setupHttpServer(){
		
	if( !setupFile.loadFile(PATH_TO_CONFIG)) 
	{	
		cout << "[ERROR] " << ofToDataPath(PATH_TO_CONFIG) << "could not be found OR a parsing error. "<< endl;
		cout << "Please check location and syntax. Proceeding using default values:" << endl; 
	} 
	
	else // if config file was opened
	{ 	
		cout << "[Setup] Parsing XML\n" << endl; 
		setupFile.pushTag("ccwconfig");
		httpSettings.port			=	setupFile.getValue("port", DEFAULT_HTTP_PORT);
		httpSettings.directory		=	setupFile.getValue("dir", DEFAULT_HTTP_DIRECTORY);
		httpSettings.dirListEnabled	=	resolveBool(setupFile.getValue("dirlist", DEFAULT_DIRECTORY_LISTING));
		httpSettings.validIndices	=	setupFile.getValue("index", DEFAULT_INDICES);
		httpSettings.maxThreads		=	setupFile.getValue("maxthreads", DEFAULT_MAX_THREADS);
		httpSettings.sslCert		=	setupFile.getValue("sslcertificate",NULL);
		
		if (resolveBool(setupFile.getValue("logging","false")))
		{
			httpSettings.accessLog		=	setupFile.getValue("accesslog", DEFAULT_ALOG);
			httpSettings.errorLog		=	setupFile.getValue("errorlog", DEFAULT_ELOG);	
		} 
		else 
		{
			//httpSettings.accessLog = NULL;
			//httpSettings.errorLog = NULL;
		}
		
		// If autoRun was specified true, request HTTP server to start immediately
		if (httpSettings.autoRun = resolveBool(setupFile.getValue("autorun","false")))
			startHttpServer();
	}	
	
	cout << "[Setup] HTTP Server setup complete, using:\n-Port: " << httpSettings.port << "\n-Dir: " << httpSettings.directory << "\n-Directory Listing: " << httpSettings.dirListEnabled << "\n-Indices: " << httpSettings.validIndices << "\n-autoRun: " << httpSettings.autoRun << "\n-Max Threads: " << httpSettings.maxThreads << "\n-SSL Certificate: " << httpSettings.sslCert << "\n-Access Log: " << httpSettings.accessLog << "\n-Error Log: " << httpSettings.errorLog << "\n" << endl; 

}

// ======================================= Draw the Display
void ccwServer::drawGUI()
{	
	ofSetWindowTitle("CCW [Stopped]");
	
	// -- Background panel
	p_startStop	= ccwGui->addPanel(uiPanel1, "Community Core Web - HTTP / PHP", OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING, false);
	
	// Start / stop buttons
	b_http		= static_cast<ofxGuiButton*>(p_startStop->addButton(uiStartStop, "Run HTTP Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	
	#ifdef CCW_FULL
	b_mySQL		= static_cast<ofxGuiButton*>(p_startStop->addButton(uiMySQLStartStop, "Run MySQL Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	b_webGui	= static_cast<ofxGuiButton*>(p_startStop->addButton(uiWebGuiStartStop, "Run Web GUI Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	#endif
	
	// -- Settings panel
	p_settings	= ccwGui->addPanel(uiPanel2, "Options", 2*OFXGUI_PANEL_BORDER, p_startStop->mObjHeight+OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING,true);
	
	b_autoRun	= static_cast<ofxGuiButton*>(p_settings->addButton(uiAutoRun, "Autorun", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, httpSettings.autoRun , kofxGui_Button_Trigger));
	b_log		= static_cast<ofxGuiButton*>(p_settings->addButton(uiLog, "Debug Logging", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, httpIsLogging, kofxGui_Button_Trigger)); //TODO implement
	b_dirList	= static_cast<ofxGuiButton*>(p_settings->addButton(uiDirList, "Directory Listing", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, httpSettings.dirListEnabled, kofxGui_Button_Trigger));
	
	// -- Ports panel
	p_ports		= ccwGui->addPanel(uiPanel3, "Ports", 3*OFXGUI_PANEL_BORDER+p_settings->mObjWidth , p_startStop->mObjHeight+OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING,true);
	
	#ifdef TARGET_OSX
	s_httpPort	= static_cast<ofxGuiSlider*>(p_ports->addSlider(uiPort, "Port", 100, OFXGUI_BUTTON_HEIGHT, 1025, 65535, httpSettings.port, kofxGui_Display_Int, 65522)); // TODO Revise width
	#endif
	
	#ifndef TARGET_OSX // because these won't work TODO implement them
	s_httpPort	= static_cast<ofxGuiSlider*>(p_ports->addSlider(uiPort, "HTTP", 100, OFXGUI_BUTTON_HEIGHT, 0, 65535, httpSettings.port, kofxGui_Display_Int, 65536)); // TODO Revise width
	b_http80	= static_cast<ofxGuiButton*>(p_ports->addButton(uiPort80, "80", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger)); // TODO implement
	b_http888	= static_cast<ofxGuiButton*>(p_ports->addButton(uiPort888, "888", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger)); // TODO implement	
	#endif
	
	b_http8080	= static_cast<ofxGuiButton*>(p_ports->addButton(uiPort8080, "8080", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger)); // TODO implement
	
	#ifdef CCW_FULL
		#ifdef TARGET_OSX
		s_webGuiPort = static_cast<ofxGuiSlider*>(p_ports->addSlider(uiWebGuiPort, "Web GUI", 100, OFXGUI_BUTTON_HEIGHT, 1025, 65535, webGuiPort, kofxGui_Display_Int, 65536));
		#endif
		#ifndef TARGET_OSX
		s_webGuiPort = static_cast<ofxGuiSlider*>(p_ports->addSlider(uiWebGuiPort, "Web GUI", 100, OFXGUI_BUTTON_HEIGHT, 0, 65535, webGuiPort, kofxGui_Display_Int, 65536));
		#endif
	#endif
	
	
	// Stretch background panel behind options panel
	if (p_settings->mObjHeight > p_ports->mObjHeight)
		{
			p_startStop->mObjHeight += (p_settings->mObjHeight + OFXGUI_PANEL_BORDER);
		}
	else p_startStop->mObjHeight += (p_ports->mObjHeight + OFXGUI_PANEL_BORDER);
	
	p_startStop->mObjWidth = (p_settings->mObjWidth + (3*OFXGUI_PANEL_BORDER) + p_ports->mObjWidth);
	
	ccwGui->activate(true);
	ccwGui->forceUpdate(true);
	
}




// ======================================= Called when an action is performed on the UI
void ccwServer::handleGui(int parameterId, int task, void* data, int length)
{
	bool httpWasRunning = httpIsRunning;
	
	switch(parameterId)
	{
		case uiStartStop:
			if (httpIsRunning) 
			{
				stopHttpServer();
				ofSetWindowTitle("CCW [Stopped]");
			}
			else 
			{
				startHttpServer();
			}
		break;
			
		case uiMySQLStartStop:
			if (mySQLIsRunning)
			{
				stopMySQLServer();
			} 
			else startMySQLServer();
		break;
			
		case uiWebGuiStartStop:
			if (webGuiIsRunning)
			{
				stopWebGuiServer();
			} 
			else startWebGuiServer();
		break;
			
		case uiPort80:
			setHttpPort(80);
			b_http80->setValue(true);
		break;
			
		case uiPort888:
			setHttpPort(888);
			b_http888->setValue(true);
		break;
			
		case uiPort8080:
			setHttpPort(8080);
			b_http8080->setValue(true);
		break;
			
		case uiAutoRun:
			if (httpSettings.autoRun) 
			{
				httpSettings.autoRun = "false";
				b_autoRun->setValue(false);
				setupFile.setValue("autorun", "false", 0);
			}
			else 
			{
				httpSettings.autoRun = true;
				b_autoRun->setValue(true);
				setupFile.setValue("autorun", "true", 0);
			}
			setupFile.saveFile(PATH_TO_CONFIG);
		break;
			
		case uiLog: //TODO are they written seperately?
			if (httpIsRunning) stopHttpServer();
			
			if (httpSettings.logEnabled) 
			{
				httpSettings.logEnabled = "false";
				b_log->setValue(false);
				setupFile.setValue("logging", "false", 0);
			}
			else 
			{	
				httpSettings.logEnabled = true;
				b_log->setValue(true);
				setupFile.setValue("logging", "true", 0);
			}
			
			if (httpWasRunning) startHttpServer();
			
			setupFile.saveFile(PATH_TO_CONFIG);
		break;
			
		case uiDirList:
			if (httpIsRunning) stopHttpServer();
		
			if (httpSettings.dirListEnabled) 
			{
				httpSettings.dirListEnabled = "false";
				b_dirList->setValue(false);
				setupFile.setValue("dirlist", "false", 0);
			}
			else 
			{
				httpSettings.dirListEnabled = true;
				b_dirList->setValue(true);
				setupFile.setValue("dirlist", "true", 0);
			}
			
			if (httpWasRunning) startHttpServer();
			
			setupFile.saveFile(PATH_TO_CONFIG);
			
		break;
			
		case uiPort:
			httpSettings.port = s_httpPort->mValue;
			httpPortChanged = true;
		break;
	}
}




// ================== Following methods start and stop the servers
string ccwServer::parseServerSettings(ofxWebServerSettings& settings)
{	


	
}




// ================== Following methods start and stop the servers
void ccwServer::startHttpServer()
{
	if (ccwGui->mIsActive) //	GUI Drawn
	{
		string phpCgiPath, docRoot = "";
		
#ifdef _WIN32
		phpCgiPath = "plugins/PHP/php-cgi.exe";
#endif // WIN32
		
#ifdef TARGET_OSX
		// Ugly workaround - osx doesn't like the relative paths
		// Get the absolute location of the executable file in the bundle.
		CFBundleRef appBundle     = CFBundleGetMainBundle();
		CFURLRef   executableURL = CFBundleCopyExecutableURL(appBundle);
		char execFile[4096];
		if (CFURLGetFileSystemRepresentation(executableURL, TRUE, (UInt8 *)execFile, 4096))
		{
			// Strip out the filename to just get the path
			string strExecFile = execFile;
			int found = strExecFile.find_last_of("/");
			string phpCgiPath = docRoot = strExecFile.substr(0, found);
			phpCgiPath.append("/../../../data/plugins/PHP/php-cgi");
	
		}
		else cout << "Couldn't get absolute path. No PHP. \n";
#endif //osx
		
		docRoot.append(httpSettings.directory);
		
		const char* options[] = {
			"document_root", docRoot.c_str(),
			"listening_ports", ofToString(httpSettings.port).c_str(),
			"enable_directory_listing", ofToString(httpSettings.dirListEnabled).c_str(),
			"index_files" , httpSettings.validIndices.c_str(),
			"num_threads" , ofToString(httpSettings.maxThreads).c_str(),
			//"ssl_certificate" , httpSettings.sslCert.c_str(),
			"access_log_file" , httpSettings.accessLog.c_str(),
			"error_log_file" , httpSettings.errorLog.c_str(),
			"cgi_interpreter" , phpCgiPath.c_str(),
			NULL
		};
		const char** optPtr = options;
		
		httpServer.start(optPtr);
		httpIsRunning = true;
		b_http->setValue(true);
		ccwConfigResponse* pConfigResponse = &configResponse;
		httpServer.addHandler(pConfigResponse, "/config/");
		ofSetWindowTitle("CCW [Running]");
		cout << "[HTTP Control] Server started \n\n";
	} 
	else 
	{ 
		startFlag = 1; 
	}
}


// --
void ccwServer::stopHttpServer()
{	
	httpServer.stop();
	httpIsRunning = false;
	b_http->setValue(false);
	ofSetWindowTitle("CCW [Stopped]");
	cout << "[HTTP Control] Server stopped\n\n";
}


// -- MySQL Server calls a shell script on osx to start daemon
void ccwServer::startMySQLServer() 
{
	// This only verifies that the script was called, not if it works
	if (!mySQLDisabled && !system("/Applications/CCW/data/plugins/MySQL/control/start.sh"))
		{ 
			// Returns zero on success osx
			mySQLIsRunning = true;
			cout << "[MySQL Control] MySQL daemon script control/start.sh called" << endl;
		}
	else 
		{
			cout << "[ERROR] Could not start MySQL daemon" << endl;
			mySQLIsRunning = false;
		}

	b_mySQL->setValue(mySQLIsRunning);
}


// --
void ccwServer::stopMySQLServer()
{
	// This only verifies that the script was called, not if it works
	if (!system("/Applications/CCW/data/plugins/MySQL/control/stop.sh")) 
	{
		mySQLIsRunning = false;
		b_mySQL->setValue(false);
		cout << "[MySQL Control] MySQL daemon script control/stop.sh called" << endl;
	}
	else 
	{
		cout << "[ERROR] Could not stop MySQL daemon" << endl;
	}
}


// --
void ccwServer::startWebGuiServer() 
{
	webGui.startServer(webGuiPort);
	webGuiIsRunning = true;
	b_webGui->setValue(true);
	cout << "[WebGUI Control] Server Started" << endl;
}

//--
void ccwServer::stopWebGuiServer() 
{
	webGui.stopServer();
	webGuiIsRunning = false;
	b_webGui->setValue(false);
	cout << "[WebGUI Control] Server Stopped" << endl;
}




// ======================================= Load Web GUI Settings from XML
void ccwServer::populateWebGui() 
{
	
	ofxDirList dir;
	int numFiles = dir.listDir("plugins/WebGUI/gui/");
	cout << "DEBUG: num gui pages = " << numFiles << endl;
	
	webGui.addPage("A test page");
	
	bool toggleValue = true;
	bool buttonValue = false;
	int	 iSldrValue	 = 5;
	float fSldrValue = 5.0;
	
	webGui.page(2).setXMLName("testGUiWIN.xml");
	
	webGui.page(2).addTitle("Test Page Title", 30);
	webGui.page(2).addToggle("TestToggle", toggleValue);
	webGui.page(2).addButton("TestButton", buttonValue);
	webGui.page(2).addSlider("TestIntSlider", iSldrValue, 0, 10);
	webGui.page(2).addSlider("TestFloatSlider", fSldrValue, float(0.0), float(10.0), float(0.1));
	
	webGui.saveToXML();
	
	/*(for (int i=0; i < numFiles; i++)
	{
		string loc = "plugins/WebGUI/gui/";
		string name = dir.getName(i);
		cout << "and the " << i << "th name is " << name << endl;
		webGui.addPage(name);
		loc.append(name);
		cout << "so i am loading " << loc << endl << endl;
		webGui.page(i).setXMLName(loc);
		webGui.page(i).loadFromXML();
	}*/
	
	
	//ofxXmlSettings uiDescription;
	
	//if (!uiDescription.loadFile("/Applications/CCW/data/plugins/WebGUI/gui.xml"))
	//{
	//	cout << "[ERROR] Web GUI not enabled, could not read XML description" << endl;
	//}
	
	//else 
//	{
		
		
		//webGui.addPage("myPage");
		//webGui.page(0).setXMLName("./plugins/WebGUI/gui/myPage.xml");
		//webGui.page(0).addButton("shjkdsf",boolMap["shjkdsf"]);
		//webGui.page(0).saveToXML();
		//webGui.page(1).addButton("btn1",boolMap[0]);
		
		//webGui.saveToXML();
	
		
		
		//webGui.loadFromXML(&uiDescription);
		
		/*cout << "[Plugin Setup] Opened plugins/WebGUI/gui.xml" << endl;
		uiDescription.pushTag("ccw-webgui");
		bool lastPageFlag = false;
		
		for (int i=0; lastPageFlag == false; i++)
		{
			
			if (uiDescription.tagExists("page",i)) 
			{
				webGui.addPage(uiDescription.getAttribute("page","name","untitled page",i));
				
				uiDescription.pushTag("page");
				
				bool lastObjectFlag = false;
				for (int j=0; lastObjectFlag == false; j++)
				{
					if (uiDescription.tagExists("button", j)) 
					{
						string name = uiDescription.getAttribute("button","name","button",j);
						webGui.page(i).addButton(name, boolMap[name]=true);
					} 
					
					
					else if (uiDescription.tagExists("colorpicker", j)) {
						//webGui.page(i).addColorPicker(uiDescription.getAttribute("colorpicker","name","color picker",j), uiDescription.getAttribute("colorpicker","values",0,j));
					} else if (uiDescription.tagExists("combobox", j)) {
						// TODO webGui.page(i).addComboBox(uiDescription.getAttribute("combobox","name","combo box",j),
					} else if (uiDescription.tagExists("content", j)) {
						// create new
					} else if (uiDescription.tagExists("fpscounter", j)) {
						// create new
					} else if (uiDescription.tagExists("movieslider", j)) {
						// create new
					} else if (uiDescription.tagExists("quadwarp", j)) {
						// create new
					} else if (uiDescription.tagExists("slider2d", j)) {
						// create new
					} else if (uiDescription.tagExists("title", j)) {
						// create new
					} else if (uiDescription.tagExists("toggle", j)) {
						// create new
					} else { 
						lastObjectFlag = true;
					}
					
				}
			}
			else { lastPageFlag = true; }
		}*/
		
	
}
		
	


// ======================================= Set the port
void ccwServer::setHttpPort(unsigned int newPort)
{
	bool httpWasRunning = httpIsRunning;
	
	#ifndef TARGET_OSX // we dont have these buttons on macosx version
		btnPort80->setValue(false);
		btnPort888->setValue(false);
	#endif
	
	b_http8080->setValue(false);
	
	if (httpIsRunning) stopHttpServer();
	
	httpSettings.port = newPort;
	s_httpPort->setValue(newPort);
	
	if (httpWasRunning) startHttpServer();
}


//  =================== Currently not used much, just stores text info
int ccwServer::testForPlugins()
{
	ofxDirList dir;
	int numFiles = dir.listDir("plugins/");
	for(int i = 0; i < numFiles; i++)
	{
		string pathToInfo = "plugins/";
		pathToInfo.append(dir.getName(i));
		pathToInfo.append("/ccw-info.xml");
		
		activePlugins.push_back(new ccwPlugin(pathToInfo));
	}
	return numFiles;
}



//===================== Error responses
void ccwServer::show404(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data)
{
	mg_printf(conn, "<html><head><title>404 | Community Core Web</title><style type=\"text/css\">* {margin: 0; padding: 0;}body {font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:11px;background-color: #f9faf9;}#wrap {width: 700px;margin: 4em auto;height: 4em;overflow:hidden;}a {text-decoration: none;}h1 {font-size: 3.27em;line-height:1em;font-weight: 100;color: #3a3a3e;float: left;border-right: 1px solid #5b5b5f;padding-right: 0.5em;margin-right: 0.5em;}h2 {color: #5b5b5f;font-size: 1.45em;}</style></head><body><div id=\"wrap\"><h1>404</h1><h2>The resource you requested could not be found</h2><p>Powered by <a href=\"http://www.nuicode.com/nxml/\">Community Core Web</a></p></div></body></html>");
}

void ccwServer::show403(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data)
{
	mg_printf(conn, "<html><head><title>403 | Community Core Web</title><style type=\"text/css\">* {margin: 0; padding: 0;}body {font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:11px;background-color: #f9faf9;}#wrap {width: 700px;margin: 4em auto;height: 4em;overflow:hidden;}a {text-decoration: none;}h1 {font-size: 3.27em;line-height:1em;font-weight: 100;color: #3a3a3e;float: left;border-right: 1px solid #5b5b5f;padding-right: 0.5em;margin-right: 0.5em;}h2 {color: #5b5b5f;font-size: 1.45em;}</style></head><body><div id=\"wrap\"><h1>403</h1><h2>Directory listings are disabled</h2><p>Powered by <a href=\"http://www.nuicode.com/nxml/\">Community Core Web</a></p></div></body></html>");
}



//===================== Simple bool test
bool ccwServer::resolveBool(string toTest){
	if (toTest == "false") return false;
	else return true;
}

//===================== Reverse
string ccwServer::resolveBool(bool toTest){
	if (toTest == false) return "false";
	else return "true";
}

//--------------------------------------------------------------
void ccwServer::draw(){
	ccwGui->draw();
}

//--------------------------------------------------------------
void ccwServer::keyPressed(int key){
}

//--------------------------------------------------------------
void ccwServer::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ccwServer::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ccwServer::mouseDragged(int x, int y, int button){
	ccwGui->mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ccwServer::mousePressed(int x, int y, int button){
	ccwGui->mousePressed(x, y, button);	
}

//--------------------------------------------------------------
void ccwServer::mouseReleased(int x, int y, int button){
	if (httpPortChanged && httpIsRunning) 
	{ // indicates the port slider updated
		httpPortChanged = false;
		stopHttpServer();
		startHttpServer();
	}
}

//--------------------------------------------------------------
void ccwServer::windowResized(int w, int h){
	
}