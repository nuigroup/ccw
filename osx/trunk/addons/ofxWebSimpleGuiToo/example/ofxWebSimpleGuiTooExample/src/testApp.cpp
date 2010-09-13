#include "testApp.h"

bool fillShape = true;
float sizeX = 100;
float sizeY = 100;
bool circle = false;

int r = 0;
int g = 0;
int b = 0;

//--------------------------------------------------------------
void testApp::setup(){	 
	ofSetVerticalSync(true);
	gui.addToggle("fill", fillShape);
	gui.addSlider("sizeX", sizeX, 50, 800);
	gui.addSlider("sizeY", sizeY, 50, 600);
	gui.addToggle("circle", circle);
	gui.addSlider("r", r, 0, 255);
	gui.addSlider("g", g, 0, 255);
	gui.addSlider("b", b, 0, 255);
	
	gui.startServer();
}

//--------------------------------------------------------------
void testApp::update(){


}

//--------------------------------------------------------------
void testApp::draw(){
	if(fillShape) ofFill();
	else ofNoFill();
	
	ofSetColor(r, g, b);
	if(circle) {
		ofEllipse(ofGetWidth()/2, ofGetHeight()/2, sizeX, sizeY);
	} else {
		ofRect((ofGetWidth()-sizeX)/2, (ofGetHeight()-sizeY)/2, sizeX, sizeY);
	}
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}
