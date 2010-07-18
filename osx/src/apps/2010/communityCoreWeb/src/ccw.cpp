#include "ccw.h"

//--------------------------------------------------------------
void ccw::setup(){
	svrGui = ofxGui::Instance(this);
	//ofxNxmlFile storedFile("config.nxml");
	runServerRequest = serverIsRunning = false;
	drawGUI();
	setupServer();

	if (autoStart)
		runServerRequest = true;
}

//--------------------------------------------------------------
void ccw::update(){
	if (runServerRequest != serverIsRunning){
		if (runServerRequest){
			cout << "Starting server...\n";
			startServer(srvPort);
		} else {
			cout << "Stopping server...\n";
			stopServer();
		}
	}
}

//--------------------------------------------------------------
void ccw::draw(){
	svrGui->draw();
}

//--------------------------------------------------------------
void ccw::keyPressed(int key){

}

//--------------------------------------------------------------
void ccw::keyReleased(int key){

}

//--------------------------------------------------------------
void ccw::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ccw::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ccw::mousePressed(int x, int y, int button){
	svrGui->mousePressed(x, y, button);	
}

//--------------------------------------------------------------
void ccw::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ccw::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ccw::setupServer(){
	server = ofxHTTPServer::getServer();
	ofxXmlSettings setup;
	
	if( !setup.loadFile("config/ccwconfig.xml")) {	
		cout << "---------------------------------------"	<< endl;
		cout << "ERROR: " << ofToDataPath("ccwconfig.xml")  << endl;
		cout << "could not be found OR a parsing error. "	<< endl;
		cout << "Please check location and syntax.      "	<< endl;
		cout << "Proceed using default values:			"	<< endl;
		cout << "---------------------------------------"	<< endl;  
		}
			
	setup.pushTag("ccwconfig");
		srvPort				=	setup.getValue("port",5555);
		string srvDir		=	setup.getValue("servedir","www");
		string uploadDir	=	setup.getValue("uploaddir","uploads");
		string cbExt		=	setup.getValue("callbackextension",".ccw");
		u_short maxClients	=	setup.getValue("maxclients", 5);
		string tmpAutoStart	=	setup.getValue("autostart","false");
		string index		=	setup.getValue("index","index.html");
		string logFileLoc	=	setup.getValue("log","noLog");
	setup.popTag();
	
	// Tidy up a few of the XML settings before applying them
	if (tmpAutoStart == "false")
		autoStart = false;
	else autoStart = true;
	
	if (logFileLoc != "noLog") {
		logFile.open(ofToDataPath(logFileLoc).c_str(), ios::app);
		
		if (logFile.is_open()) { 
			cout.rdbuf(logFile.rdbuf());
			logging = true;
		} else {
			cout << "Can't open logfile\n" << endl;
			logging = false;
		}
	} else logging = false;
	
	server->setServerRoot(srvDir);
	server->setUploadDir(uploadDir);
	server->setCallbackExtension(cbExt);
	server->setMaxNumberClients(maxClients);
	server->setIndex(index);
	svrResetBtn->setValue(false);
	
	ofAddListener(server->getEvent,this,&ccw::getRequest);
	ofAddListener(server->postEvent,this,&ccw::postRequest);
	
	cout<< "Server setup complete, using params:\nPort: " << srvPort << "\nServing from directory: " << ofToDataPath(srvDir) << "\nUpload directory: " << ofToDataPath(uploadDir) << "\nCallback extension: " << cbExt << "\nMax Clients = " << maxClients << "\nAutoStart: " << tmpAutoStart << endl;
	if (logging) cout << "Logging to: " << ofToDataPath(logFileLoc) << endl << endl;
	else cout << "Logging is disabled" << endl << endl;
}

//--------------------------------------------------------------
void ccw::startServer(u_short port){
	serverIsRunning = true;
	server->start(port);
	svrOnOffBtn->setValue(true);
	ofSetWindowTitle("CCW Running");
	cout << "... server running\n";
}

//--------------------------------------------------------------
void ccw::stopServer(){
	serverIsRunning = false;
	server->stop();
	svrOnOffBtn->setValue(false);
	ofSetWindowTitle("CCW Stopped");
	cout << "... server stopped\n";
}

//-------------------------------------------------------------
void ccw::getRequest(ofxHTTPServerResponse & response){
	cout << "GET request made\n";
}

//-------------------------------------------------------------
void ccw::postRequest(ofxHTTPServerResponse & response){
	cout << "POST request made\n";
}
	
//--------------------------------------------------------------
void ccw::drawGUI(){
	ofSetWindowTitle("CCW Stopped");
	panel1 = svrGui->addPanel(ccwPanel1, "Control", 20, 20, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	svrOnOffBtn = static_cast<ofxGuiButton*>(panel1->addButton(ccwStartStop, "Run Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	svrResetBtn = static_cast<ofxGuiButton*>(panel1->addButton(ccwReset,"Restart Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	//svrCtrlHide = static_cast<ofxGuiButton*>(panel1->addButton(ccwHide,"Hide (cmd+h)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	
	svrGui->activate(true);
	svrGui->forceUpdate(true);	
}

//--------------------------------------------------------------
void ccw::handleGui(int parameterId, int task, void* data, int length)
{
	switch(parameterId)
	{
		case ccwStartStop:
			if(task == kofxGui_Set_Bool)
			{
				if(*(bool*)data) {
					if (serverIsRunning) {
						runServerRequest = false;
					}
					else {
						runServerRequest = true;
					}
				}
			}
		break;
		case ccwReset:
			setupServer();
			break;
		case ccwHide:
			  // nah - just cmd+h ?
			break;
	}
}

//--------------------------------------------------------------
ccw::~ccw(){
	if (logging)
		logFile.close();
}