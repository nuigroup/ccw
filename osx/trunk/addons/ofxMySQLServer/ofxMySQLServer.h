#ifndef __OFXMYSQLSERVER__
#define __OFXMYSQLSERVER__
#pragma comment ( linker, "libmysql.lib" )
/*
 *  ofxMySQLServer.h
 *  communitycoreweb
 *
 *  Created by Jake on 17/08/2010.
 *  Copyright 2010 NUI Group. All rights reserved.
 *
 */

#include <iostream>
using namespace std
#include "mysql.h"

class ofxMySQLServer {
public:
	ofxMySQLServer();
	void connect();
	void disconnect();
	
	
private:
	MYSQL *mysql;
	MYSQL_RES *results;
	MYSQL_ROW record;
};

#endif