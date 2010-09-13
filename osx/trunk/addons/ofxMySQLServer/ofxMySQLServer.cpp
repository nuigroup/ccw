/*
 *  ofxMySQLServer.cpp
 *  communitycoreweb
 *
 *  Created by Jake on 17/08/2010.
 *  Copyright 2010 NUI Group. All rights reserved.
 *
 */

#include "ofxMySQLServer.h"

ofxMySQLServer::ofxMySQLServer()
{
	cout << "Creating MYSQL Server" << endl;
	
	static char *server_options[] = { "mysql_test", "--defaults-file=../../../data/plugins/MySQL/my.cnf", NULL };
	int num_elements = (sizeof(server_options) / sizeof(char *)) - 1;
	static char *server_groups[] = { "libmysqld_server", NULL };
	
	
	if (!mysql_library_init(num_elements, server_options, server_groups))
	{
	
		mysql = mysql_init(NULL);
	
		mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
	
		mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
	
	} else cerr << "[ERROR] MySQLd could not be initialized. Check my.cnf is available.\n";
	
}

void ofxMySQLServer::connect() {
	
	mysql_real_connect(mysql, NULL,NULL,NULL, "database1", 0,NULL,0);
	
}

void ofxMySQLServer::disconnect() {
	
	mysql_close(mysql);
	
	mysql_library_end();
	
}
