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
		((ofxWSRequestHandler*)user_data)->httpPost(info->uri, info->query_string, sizeof(info->query_string)); //TODO wtf
	} else {
		printf("ofxWebServer: unhandled '%s' request method\n", info->request_method);
	}
}

ofxWebServer::ofxWebServer() {}

void ofxWebServer::start(string str, int port) {}
		/*TODOctx = mg_start();
		mg_set_option(ctx,"ports",ofToString(port).c_str());
		mg_set_error_callback(ctx, 404, ccwServer::show404, NULL);
		mg_set_error_callback(ctx, 403, ccwServer::show403, NULL);
		mg_set_option(ctx, "root", "/Applications/CCW/data/plugins/WebGUI/client/");*/

void ofxWebServer::start(const char** options)
{	
	ctx = mg_start(NULL,options);     // Start Mongoose serving thread
	
	if (ctx == NULL) {
		(void) printf("%s\n", "Cannot initialize Mongoose context");
		exit(EXIT_FAILURE);
	}
	
	printf("Mongoose %s web server started on port(s) %s with web root [%s]\n",
		   mg_version(), mg_get_option(ctx, "listening_ports"),
		   mg_get_option(ctx, "document_root"));
	
	/*#ifdef MACOSX
		mg_set_option(ctx, "cgi_interp", ofToDataPath("php/php-cgi").c_str());
	#endif*/
	
//	TODOmg_set_error_callback(ctx, 404, ccwServer::show404, NULL);
//	TODOmg_set_error_callback(ctx, 403, ccwServer::show403, NULL);
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
	
//TODO	mg_set_uri_callback(ctx, pattern.c_str(), webserverCallback, handler);
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