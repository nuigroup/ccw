/*
 *  ofxWebServer.cpp
 *  webServerExample
 *
 *  Created by Marek Bereza on 26/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofxWebServer.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#endif // WIN32

#include "ccwServer.h"

void ipLongToString(long ipLong, char *ipString) {
	unsigned short a, b, c, d;
	a = (ipLong & (0xff << 24)) >> 24;
	b = (ipLong & (0xff << 16)) >> 16;
	c = (ipLong & (0xff << 8)) >> 8;
	d = ipLong & 0xff;
	sprintf(ipString, "%hu.%hu.%hu.%hu", a, b, c, d);
}

// callback from server
void webserverCallback(struct mg_connection *conn,
					   const struct mg_request_info *info, 
					   void *user_data) {
	
	
	//char ipAddress[4*3+2];
	//ipLongToString(info->remote_ip, ipAddress);
	string query = "";
	if(info->query_string!=NULL) query = info->query_string;
		
	((ofxWSRequestHandler*)user_data)->setConnection(conn, "", query);
	
	if(strcmp(info->request_method,"GET")==0 || strcmp(info->request_method,"get")) {
		((ofxWSRequestHandler*)user_data)->httpGet(info->uri);
	} else if(strcmp(info->request_method,"POST")==0 || strcmp(info->request_method,"post")) {
		((ofxWSRequestHandler*)user_data)->httpPost(info->uri, info->post_data, info->post_data_len);
	} else {
		printf("ofxWebServer: unhandled '%s' request method\n", info->request_method);
	}
}

ofxWebServer::ofxWebServer() {
}

void ofxWebServer::start(string str, int port) {
		ctx = mg_start();
		mg_set_option(ctx,"ports",ofToString(port).c_str());
		mg_set_error_callback(ctx, 404, ccwServer::show404, NULL);
		mg_set_error_callback(ctx, 403, ccwServer::show403, NULL);
		mg_set_option(ctx, "root", "/Applications/CCW/data/plugins/WebGUI/client/");
}

void ofxWebServer::start(ofxWebServerSettings *settings) {
	ctx = mg_start();     // Start Mongoose serving thread
	
	// Set document root
	if (!mg_set_option(ctx, "root", ofToDataPath(settings->directory, true).c_str())) 
		cout << "[ERROR] Could not set HTTP serving directory, check ccw-config syntax" << endl; 
	
	// Listen on port XXXX
	if (!mg_set_option(ctx, "ports", ofToString(int(settings->port)).c_str()))
		cout << "[ERROR] Could not set HTTP port, check ccw-config syntax" << endl;
	
	// Should directory listing be enabled?	
	if (!mg_set_option(ctx, "dir_list", ofToString(settings->dirListEnabled).c_str()))
		cout << "[ERROR] Could not set directory listing, check ccw-config syntax" << endl;
	
	// Which index files to search for?
	if (!mg_set_option(ctx, "index_files", settings->validIndices.c_str()))
		cout << "[ERROR] Could not set index files, check ccw-config syntax" << endl;
	
	// Maximum threads to spawn
	if (!mg_set_option(ctx, "max_threads", ofToString(settings->maxThreads).c_str()))
		cout << "[ERROR] Could not set max threads, check ccw-config syntax" << endl;
	
	// Idle time
	if (!mg_set_option(ctx, "idle_time", ofToString(settings->idleTime).c_str()))
		cout << "[ERROR] Could not set idle time, check ccw-config syntax" << endl;
	
	if (settings->logEnabled) {
		// Access log location
		if (!mg_set_option(ctx, "access_log", ofToDataPath(settings->accessLog).c_str()))
			cout << "[ERROR] Could not set access log location, check ccw-config syntax" << endl;
	
		// Error log location
		if (!mg_set_option(ctx, "error_log", ofToDataPath(settings->errorLog).c_str()))
			cout << "[ERROR] Could not set error log location, check ccw-config syntax" << endl;
	}
	
	// Currently, force php as only cgi extension
	mg_set_option(ctx, "cgi_ext", ".php"); // TODO: revise
	
	/*
	 * TODO: Unimplemented mongoose options
	 */
	
	#ifdef _WIN32
		mg_set_option(ctx, "cgi_interp", ofToDataPath("plugins/PHP/php-cgi.exe").c_str());
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
		string strPath = strExecFile.substr(0, found);
		strPath.append("/../../../data/plugins/PHP/php-cgi");
		if (mg_set_option(ctx, "cgi_interp", strPath.c_str()))
			cout << "Looking for " << strPath << endl;
		else cout << "[ERROR] Could not set php-cgi path" << endl;
	}
	else cout << "Couldn't get absolute path. No PHP. \n";
	#endif //osx
	
	/*#ifdef MACOSX
		mg_set_option(ctx, "cgi_interp", ofToDataPath("php/php-cgi").c_str());
	#endif*/
	
//	aLogFile = ofToDataPath(settings->accessLog).c_str();
//	eLogFile = ofToDataPath(settings->errorLog).c_str();
	
	mg_set_error_callback(ctx, 404, ccwServer::show404, NULL);
	mg_set_error_callback(ctx, 403, ccwServer::show403, NULL);
}

void ofxWebServer::stop() {
	mg_stop(ctx);
}

void ofxWebServer::addHandler(ofxWSRequestHandler *handler, string pattern) {
	
	// make sure there's a forward slash at the beginning
	if(pattern.size()==0) {
		pattern = "/";
	} else if(pattern[0]!='/') {
		pattern = "/" + pattern;
	}
	
	mg_set_uri_callback(ctx, pattern.c_str(), webserverCallback, handler);
}

string ofxWebServer::getIPAddress() {
	char baseHostName[255];
	gethostname(baseHostName, 255);
	
	// Adjust for iPhone -- add .local to the host name
	char hn[255];
	sprintf(hn, "%s.local", baseHostName);
	
	struct hostent *host = gethostbyname(hn);
	if (host == NULL)
	{
		//herror("resolv");
		return NULL;
	} else {
		struct in_addr **list = (struct in_addr **)host->h_addr_list;
		return inet_ntoa(*list[0]);
	}
}