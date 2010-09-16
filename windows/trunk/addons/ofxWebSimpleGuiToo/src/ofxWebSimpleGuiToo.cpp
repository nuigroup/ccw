/*
 *  ofxWebSimpleGuiToo.cpp
 *  ofxWebSimpleGuiTooExample
 *
 *  Created by Marek Bereza on 27/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofxWebSimpleGuiToo.h"



ofxWebSimpleGuiToo::ofxWebSimpleGuiToo() {
}

void ofxWebSimpleGuiToo::startServer(int port) {
	// MOD TODO TEMP server.start("ofxWebSimpleGuiToo", port);
	server.start("/Applications/CCW/data/plugins/WebGUI", port);
	server.addHandler(this, "/control");
}

void ofxWebSimpleGuiToo::stopServer(){
	server.stop();
}
void ofxWebSimpleGuiToo::httpGet(string url) {
	
	string key = getRequestParameter("key");
	if(key=="") {
		string guiDesc = "[";
		for(int i = 0; i < pages.size(); i++) {
			guiDesc += describePage(pages[i]);
			if(i+1<pages.size()) {
				guiDesc += ",";
			}
		}
		guiDesc += "]";
		httpResponse(guiDesc);
	} else {

		string value = getRequestParameter("value");
		printf("value received: %s = %s\n", key.c_str(), value.c_str());
		ofxSimpleGuiControl *ctrl = &control(key);
	
		if(ctrl!=NULL) {
			if(ctrl->controlType=="Toggle") {
				ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)ctrl;
				if (value == "") {
					httpResponse(ofToString(*(t->value)));
				} else {
					*(t->value) = (value=="true");
					httpResponse("thanks");
				}
			} else if(ctrl->controlType=="SliderInt") {
				ofxSimpleGuiSliderInt *t = (ofxSimpleGuiSliderInt*)ctrl;
				if (value == "") {
					httpResponse(ofToString(*(t->value)));
				} else {
				*(t->value) = atoi(value.c_str());
				httpResponse("thanks");
				}
			} else if(ctrl->controlType=="SliderFloat") {
				ofxSimpleGuiSliderFloat *t = (ofxSimpleGuiSliderFloat*)ctrl;
				if (value == "") {
					httpResponse(ofToString(*(t->value)));
				} else {
				*(t->value) = atof(value.c_str());
				httpResponse("thanks");
				}
			} else if(ctrl->controlType=="Button") {
				ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)ctrl;
				if (value == "") {
					httpResponse(ofToString(*(t->value)));
				} else {
//				t->onPress(0, 0, 0);
//				t->onRelease(0, 0, 0);
				*(t->value) = true;
				httpResponse("thanks");
// 				printf("Hello here!!\n");
				}
			}
		}
	}
}

string ofxWebSimpleGuiToo::describePage(ofxSimpleGuiPage *page) {
	string desc = "{\"name\": \""+page->name+"\", \"controls\":[";
	for(int i = 0; i < page->getControls().size(); i++) {
		desc += describeControl(page->getControls()[i]);
		if(i+1<page->getControls().size()) {
			desc += ",";
		}
	}
	desc += "]}";
	return desc;
}
string ofxWebSimpleGuiToo::describeControl(ofxSimpleGuiControl *ctrl) {
	string desc = "{\"name\":\""+ctrl->name + "\",";
	desc += "\"type\": \""+ctrl->controlType+"\"";
	
	if(ctrl->controlType=="Toggle") {
		ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)ctrl;
		if(*t->value) desc += ", \"value\":\"true\"";
		else desc += ", \"value\":\"false\"";
	} else if(ctrl->controlType=="SliderInt") {
		ofxSimpleGuiSliderInt *t = (ofxSimpleGuiSliderInt*)ctrl;
		desc += ", \"value\":\""+ofToString(*(t->value))+"\"";
		desc += ", \"min\":\""+ofToString(t->min)+"\"";
		desc += ", \"max\":\""+ofToString(t->max)+"\"";
		
	} else if(ctrl->controlType=="SliderFloat") {
		ofxSimpleGuiSliderFloat *t = (ofxSimpleGuiSliderFloat*)ctrl;
		desc += ", \"value\":\""+ofToString(*(t->value))+"\"";
		desc += ", \"min\":\""+ofToString(t->min)+"\"";
		desc += ", \"max\":\""+ofToString(t->max)+"\"";
		
	}
	desc += "}";
	return desc;
}

