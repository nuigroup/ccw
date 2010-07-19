#include "ccs.h"

//--------------------------------------------------------------
void ccs::setup(){
	svrGui = ofxGui::Instance(this);
	//ofxNxmlFile storedFile("config.nxml");
	setupServer();
	drawGUI();
	serverIsRunning = false;
	runServerRequest = srvAutoStart;
}

//--------------------------------------------------------------
void ccs::update(){
	if (runServerRequest != serverIsRunning){
		if (runServerRequest) startServer();
		else stopServer();		
	}
}

//--------------------------------------------------------------
void ccs::draw(){
	svrGui->draw();
}

//--------------------------------------------------------------
void ccs::keyPressed(int key){

}

//--------------------------------------------------------------
void ccs::keyReleased(int key){

}

//--------------------------------------------------------------
void ccs::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ccs::mouseDragged(int x, int y, int button){
	svrGui->mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ccs::mousePressed(int x, int y, int button){
	svrGui->mousePressed(x, y, button);	
}

//--------------------------------------------------------------
void ccs::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ccs::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ccs::setupServer(){
	
	if( !setupFile.loadFile("config/ccs-config.xml")) {	
		cout << "[ERROR] " << ofToDataPath("config/ccs-config.xml")  << endl;
		cout << "could not be found OR a parsing error. "	<< endl;
		cout << "Please check location and syntax.      "	<< endl;
		cout << "Proceed using default values:			"	<< endl; 
	} else { cout << "[Setup] Parsing XML\n" << endl; }
			
	setupFile.pushTag("ccsconfig");
	srvPort				=	setupFile.getValue("port",80);
	srvDir				=	setupFile.getValue("dir","www");
	string srvTmpDirList=	setupFile.getValue("dirlist","true");
	string srvTmpLogging=	setupFile.getValue("logging","false");
	srvIndexes			=	setupFile.getValue("index","index.html,index.htm");
	string srvTmpAutoStart=	setupFile.getValue("autorun","false");
	srvMaxThreads		=	setupFile.getValue("maxthreads", 5);
	srvIdleTime			=	setupFile.getValue("idletime", 10);
	srvAccessLog		=	setupFile.getValue("accesslog", "log/access.log");
	srvErrorLog			=	setupFile.getValue("errorlog", "log/error.log");
	
	// Tidy up a few of the XML settings before applying them
	srvDirList = resolveBool(srvTmpDirList);
	srvAutoStart = resolveBool(srvTmpAutoStart);
	srvLogging = resolveBool(srvTmpLogging);
	
	cout<< "[Setup] Server setup complete, using:\n-Port: " << srvPort << "\n-Dir: " << srvDir << "\n-Directory Listing: " << srvDirList << "\n-Indexes: " << srvIndexes << "\n-AutoStart: " << srvAutoStart << "\n-Max Threads: " << srvMaxThreads << "\n-Idle Time: " << srvIdleTime << "\n-Access Log: " << srvAccessLog << "\n-Error Log: " << srvErrorLog << "\n" << endl; 
}

//--------------------------------------------------------------
bool ccs::resolveBool(string toTest){
	if (toTest == "false")
		return false;
	
	else return true;
}

//--------------------------------------------------------------
void ccs::startServer(){
	serverIsRunning = true;
	server.start(srvDir, srvPort, srvDirList, srvIndexes, srvMaxThreads, srvIdleTime, srvAccessLog, srvErrorLog);
	cout << "[Control] Server started \n\n";
	btnOnOff->setValue(true);
	ofSetWindowTitle("CCS [Running]");
}

//--------------------------------------------------------------
void ccs::stopServer(){
	if (serverIsRunning) {
		server.stop();
		serverIsRunning = false;
	}
	btnOnOff->setValue(false);
	ofSetWindowTitle("CCS [Stopped]");
	cout << "[Control] Server stopped\n\n";
}

	
//--------------------------------------------------------------
void ccs::drawGUI(){
	ofSetWindowTitle("CCS [Stopped]");
	
	// Background panel
	panel1 = svrGui->addPanel(ccsPanel1, "Community Core Server - HTTP / PHP", OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING,false);
	btnOnOff = static_cast<ofxGuiButton*>(panel1->addButton(ccsStartStop, "Run Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	btnReset = static_cast<ofxGuiButton*>(panel1->addButton(ccsReset,"Restart Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));	
	
	// Options panel
	panel2 = svrGui->addPanel(ccsPanel2, "Options", 2*OFXGUI_PANEL_BORDER, panel1->mObjHeight+OFXGUI_PANEL_BORDER, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING,true);
	btnAutoRun = static_cast<ofxGuiButton*>(panel2->addButton(ccsAutoRun, "Autorun", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,srvAutoStart, kofxGui_Button_Trigger));
	btnLog = static_cast<ofxGuiButton*>(panel2->addButton(ccsLog, "Debug Logging", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, srvLogging, kofxGui_Button_Trigger)); //TODO implement
	btnDirList = static_cast<ofxGuiButton*>(panel2->addButton(ccsDirList, "Directory Listing", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, srvDirList, kofxGui_Button_Trigger));
	sldrPort = static_cast<ofxGuiSlider*>(panel2->addSlider(ccsPort, "Port", 300, OFXGUI_BUTTON_HEIGHT, 0, 65535, srvPort, kofxGui_Display_Int, 65536)); // TODO Revise width and reposition
	btnPort80 = static_cast<ofxGuiButton*>(panel2->addButton(ccsPort80, "80", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger)); // TODO implement
	btnPort888 = static_cast<ofxGuiButton*>(panel2->addButton(ccsPort888, "888", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger)); // TODO implement
	btnPort8080 = static_cast<ofxGuiButton*>(panel2->addButton(ccsPort8080, "8080", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger)); // TODO implement
	//filesSrvDir = static_cast<ofxGuiFiles*>(panel2->addFiles(ccsSrvDir,	"WWW Directory", 300, OFXGUI_BUTTON_HEIGHT, ofToDataPath("WWW"), "subpath", "suffix")); // TODO argh
	
	// Set layout
	
	// Stretch background panel behind options panel
	panel1->mObjHeight += (panel2->mObjHeight + OFXGUI_PANEL_BORDER);	
	if (panel1->mObjWidth < (panel2->mObjWidth + (2*OFXGUI_PANEL_BORDER)))
		panel1->mObjWidth = (panel2->mObjWidth + (2*OFXGUI_PANEL_BORDER));

	svrGui->activate(true);
	svrGui->forceUpdate(true);	
}

//--------------------------------------------------------------
// This is a bit messy for now, quick UI implementation
//--------------------------------------------------------------
void ccs::handleGui(int parameterId, int task, void* data, int length)
{
	bool serverWasRunning = serverIsRunning;
	switch(parameterId)
	{
		case ccsStartStop:
			if(task == kofxGui_Set_Bool)
			{
				if(*(bool*)data) {
					if (serverIsRunning) {
						runServerRequest = false;
						ofSetWindowTitle("CCS [Stopping]");
					}
					else {
						runServerRequest = true;
					}
				}
			}
		break;
		case ccsReset:
			runServerRequest = false;
			setupServer();
			btnReset->setValue(false);
			runServerRequest = true;
		break;
		case ccsPort80:
			runServerRequest = false;
			srvPort = 80;
			btnPort80->setValue(true);
			btnPort888->setValue(false);
			btnPort8080->setValue(false);
			sldrPort->setValue(80);
			if (serverWasRunning)
				runServerRequest = true;
		break;
		case ccsPort888:
			runServerRequest = false;
			srvPort = 888;
			btnPort80->setValue(false);
			btnPort888->setValue(true);
			btnPort8080->setValue(false);
			sldrPort->setValue(888);
			if (serverWasRunning)
				runServerRequest = true;
		break;
		case ccsPort8080:
			runServerRequest = false;
			srvPort = 8080;
			btnPort80->setValue(false);
			btnPort888->setValue(false);
			btnPort8080->setValue(true);
			sldrPort->setValue(8080);
			if (serverWasRunning)
				runServerRequest = true;
		break;
		case ccsAutoRun:
			if (srvAutoStart == true) {
				srvAutoStart = false;
				btnAutoRun->setValue(false);
				setupFile.setValue("autorun", "false", 0);
			}
			else {
				srvAutoStart = true;
				btnAutoRun->setValue(true);
				setupFile.setValue("autorun", "true", 0);
			}
			setupFile.saveFile("config/ccs-config.xml");
		break;
		case ccsLog:
			runServerRequest = false;
			if (srvLogging == true) {
				srvLogging = false;
				btnLog->setValue(false);
				setupFile.setValue("logging", "false", 0);
			}
			else {
				srvLogging = true;
				btnLog->setValue(true);
				setupFile.setValue("logging", "true", 0);
			}
			if (serverWasRunning)
				runServerRequest = true;
			
			setupFile.saveFile("config/ccs-config.xml");
		break;
		case ccsDirList:
			runServerRequest = false;
			if (srvDirList == true) {
				srvDirList = false;
				btnDirList->setValue(false);
				setupFile.setValue("dirlist", "false", 0);
			}
			else {
				srvDirList = true;
				btnDirList->setValue(true);
				setupFile.setValue("dirlist", "true", 0);
			}
			if (serverWasRunning)
				runServerRequest = true;
			setupFile.saveFile("config/ccs-config.xml");
		break;
		case ccsPort:
			runServerRequest = false;
			srvPort = sldrPort->mValue;
			if (serverWasRunning)
				runServerRequest = true;
			//setupFile.setValue("port", ofToString(srvPort), 0);
			setupFile.saveFile("config/ccs-config.xml");
		break;
	}
}