#include "ccs.h"

//--------------------------------------------------------------
void ccs::setup(){
	svrGui = ofxGui::Instance(this);
	//ofxNxmlFile storedFile("config.nxml");
	runServerRequest = serverIsRunning = false;
	drawGUI();
	setupServer();

	if (autoStart)
		runServerRequest = true;
}

//--------------------------------------------------------------
void ccs::update(){
	if (runServerRequest != serverIsRunning){
		if (runServerRequest){
			startServer();
		} else {
			stopServer();
		}
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
	ofxXmlSettings setup;
	
	if( !setup.loadFile("config/ccs-config.xml")) {	
		cout << "[ERROR] " << ofToDataPath("config/ccs-config.xml")  << endl;
		cout << "could not be found OR a parsing error. "	<< endl;
		cout << "Please check location and syntax.      "	<< endl;
		cout << "Proceed using default values:			"	<< endl; 
	} else { cout << "[Setup] Parsing XML\n" << endl; }
			
	setup.pushTag("ccsconfig");
	srvPort				=	setup.getValue("port",80);
	srvDir				=	setup.getValue("dir","www");
	string srvTmpDirList=	setup.getValue("dirlist","true");
	srvIndexes			=	setup.getValue("index","index.html,index.htm");
	string srvTmpAutoStart=	setup.getValue("autostart","false");
	srvMaxThreads		=	setup.getValue("maxthreads", 5);
	srvIdleTime			=	setup.getValue("idletime", 10);
	srvAccessLog		=	setup.getValue("accesslog", "log/access.log");
	srvErrorLog			=	setup.getValue("errorlog", "log/error.log");
	setup.popTag();
	
	// Tidy up a few of the XML settings before applying them
	srvDirList = resolveBool(srvTmpDirList);
	srvAutoStart = resolveBool(srvTmpAutoStart);	
	svrResetBtn->setValue(false);
	
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
	svrOnOffBtn->setValue(true);
	ofSetWindowTitle("CCS [Running]");
}

//--------------------------------------------------------------
void ccs::stopServer(){
	serverIsRunning = false;
	server.stop();
	svrOnOffBtn->setValue(false);
	ofSetWindowTitle("CCS [Stopped]");
	cout << "[Control] Server stopped\n\n";
}

	
//--------------------------------------------------------------
void ccs::drawGUI(){
	ofSetWindowTitle("CCS [Stopped]");
	panel1 = svrGui->addPanel(ccsPanel1, "Control", 20, 20, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	svrOnOffBtn = static_cast<ofxGuiButton*>(panel1->addButton(ccsStartStop, "Run Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));
	svrResetBtn = static_cast<ofxGuiButton*>(panel1->addButton(ccsReset,"Restart Server", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, false, kofxGui_Button_Trigger));	
	svrGui->activate(true);
	svrGui->forceUpdate(true);	
}

//--------------------------------------------------------------
void ccs::handleGui(int parameterId, int task, void* data, int length)
{
	switch(parameterId)
	{
		case ccsStartStop:
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
		case ccsReset:
			runServerRequest = false;
			setupServer();
			runServerRequest = true;
			break;
	}
}