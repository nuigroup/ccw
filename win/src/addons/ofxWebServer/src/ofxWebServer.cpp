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

void ofxWebServer::start(string root, int port, bool dirList, string indexes, int maxThreads, int idleTime, string aLog, string eLog) {
	ctx = mg_start();     // Start Mongoose serving thread
	mg_set_option(ctx, "root", ofToDataPath(root, true).c_str());  // Set document root
	mg_set_option(ctx, "ports", ofToString(port).c_str());    // Listen on port XXXX
	mg_set_option(ctx, "dir_list", ofToString(dirList).c_str());
	mg_set_option(ctx, "index_files", indexes.c_str());
	mg_set_option(ctx, "max_threads", ofToString(maxThreads).c_str());
	mg_set_option(ctx, "idle_time", ofToString(idleTime).c_str());
	mg_set_option(ctx, "access_log", ofToDataPath(aLog).c_str());
	mg_set_option(ctx, "error_log", ofToDataPath(eLog).c_str());
	mg_set_option(ctx, "cgi_ext", "php");
	
	#ifdef _WIN32
		mg_set_option(ctx, "cgi_interp", ofToDataPath("php/php-cgi.exe").c_str());
	#endif // WIN32

	
	#ifdef MACOSX
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
		strPath.append("/../../../data/php/php-cgi");
		mg_set_option(ctx, "cgi_interp", strPath.c_str());
	}
	else cout << "Couldn't get absolute path. No PHP. \n";
	#endif //osx

	

	aLogFile = ofToDataPath(aLog).c_str();
	eLogFile = ofToDataPath(eLog).c_str();
	
	mg_set_error_callback(ctx, 404, &show404, NULL);
	mg_set_error_callback(ctx, 403, &show403, NULL);
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

void ofxWebServer::show404(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data){
	mg_printf(conn, "<html><head><title>404 | Community Core Server</title><style type=\"text/css\">* {margin: 0; padding: 0;}body {font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:11px;background-color: #f9faf9;}#wrap {width: 700px;margin: 4em auto;height: 4em;overflow:hidden;}a {text-decoration: none;}h1 {font-size: 3.27em;line-height:1em;font-weight: 100;color: #3a3a3e;float: left;border-right: 1px solid #5b5b5f;padding-right: 0.5em;margin-right: 0.5em;}h2 {color: #5b5b5f;font-size: 1.45em;}</style></head><body><div id=\"wrap\"><h1>404</h1><h2>The resource you requested could not be found</h2><p>Powered by <a href=\"http://www.nuicode.com/nxml/\">Community Core Server</a></p></div></body></html>");
}

void ofxWebServer::show403(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data){
	mg_printf(conn, "<html><head><title>403 | Community Core Server</title><style type=\"text/css\">* {margin: 0; padding: 0;}body {font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:11px;background-color: #f9faf9;}#wrap {width: 700px;margin: 4em auto;height: 4em;overflow:hidden;}a {text-decoration: none;}h1 {font-size: 3.27em;line-height:1em;font-weight: 100;color: #3a3a3e;float: left;border-right: 1px solid #5b5b5f;padding-right: 0.5em;margin-right: 0.5em;}h2 {color: #5b5b5f;font-size: 1.45em;}</style></head><body><div id=\"wrap\"><h1>404</h1><h2>Directory listings are disabled</h2><p>Powered by <a href=\"http://www.nuicode.com/nxml/\">Community Core Server</a></p></div></body></html>");
}
void ofxWebServer::logOutput(struct mg_connection *conn, const struct mg_request_info *ri, void *user_data){
	cout << "[Log] " << user_data << endl;	
}

void ofxWebServer::setLogging(bool setting) {
	if (setting == false){
			mg_set_option(ctx, "access_log", NULL);
			mg_set_option(ctx, "error_log", NULL);
	} else {
		mg_set_option(ctx, "access_log", aLogFile.c_str());
		mg_set_option(ctx, "error_log", eLogFile.c_str());
	}
}

void ofxWebServer::setDirectoryListing(bool setting) {
		mg_set_option(ctx, "dir_list", ofToString(setting).c_str());
}
