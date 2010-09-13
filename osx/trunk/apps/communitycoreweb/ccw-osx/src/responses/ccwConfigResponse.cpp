
#include "ccwConfigResponse.h"
#include "ccwServer.h"

void ccwConfigResponse::httpGet(string url) {
	string responseStr = "<html><head><title>Config | Community Core Web</title><style type=\"text/css\">* {margin: 0; padding: 0;}body {font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:11px;background-color: #f9faf9;}#wrap {width: 500px;margin: 4em auto;overflow:hidden;}a {text-decoration: none;}h1 {font-size: 3.27em;font-weight: 100;color: #3a3a3e;width: 500px;}h2 {color: #5b5b5f;font-size: 1.45em;}p {display:block;} div.plugin,ul#settings{display:block;border-left: 1px solid #5b5b5f;padding-left:1em;} li{list-style:none;} div.config-block{margin-bottom:2em;} div.plugin{margin-bottom:1em;}</style></head><body><div id=\"wrap\">";
	
	// Server setup
	responseStr.append("<div class=\"config-block\" id=\"setup\"><h1>Server Setup</h1><ul id=\"settings\">");
	responseStr.append("<li>Port <b>");
	responseStr.append(ofToString(ccwServer::instance()->httpSettings.port));
	responseStr.append("</b></li><li>Serving from <b>");
	responseStr.append(ccwServer::instance()->httpSettings.directory);

	responseStr.append("</b></li><li>If a folder is requested, <b>");
	responseStr.append(ccwServer::instance()->httpSettings.validIndices);
	responseStr.append("</b> will be opened if they exist.</li><li>Otherwise, <b>");
	if (!ccwServer::instance()->httpSettings.dirListEnabled)
		responseStr.append("an error message will be displayed</b>");
	else responseStr.append("the contents of the directory will be listed</b>");
	
	responseStr.append("</li><li>Next time CCW is launched, the server will <b>");
	if (!ccwServer::instance()->httpSettings.autoRun)
		responseStr.append("wait for \'Run Server\' to be clicked");
	else responseStr.append("run automatically");
	
	responseStr.append("</b></li><li>The maximum thread count - ie. number of supported simultaneous connections - is <b>");
	responseStr.append(ofToString(ccwServer::instance()->httpSettings.maxThreads));
	responseStr.append(" </b></li><li>When \'Debug Logging\' is selected, logs will be written to <b>");
	responseStr.append(ccwServer::instance()->httpSettings.accessLog);
	responseStr.append("</b> and <b>");
	responseStr.append(ccwServer::instance()->httpSettings.errorLog);
	responseStr.append("</b></li></ul><p><br /><i>&#42;These settings may all be modified in the file <b>/config/ccw-config.xml</i></b></p></div>");
	
	responseStr.append("<div class=\"config-block\" id=\"plugins\"><h1>Plugins</h1>");
	
	for(int i = 0; i < ccwServer::instance()->activePlugins.size(); i++)
	{
		responseStr.append("<div class=\"plugin\" id=\"plugin-");
		responseStr.append(ofToString(i));
		responseStr.append("\">");
		
		responseStr.append("<h2 class=\"plugin-name\">");
		responseStr.append(ccwServer::instance()->activePlugins.at(i)->name);
		
		responseStr.append("</h2><p class=\"plugin-description\">");
		responseStr.append(ccwServer::instance()->activePlugins.at(i)->description);
		
		responseStr.append("</p><p class=\"plugin-version-author\">");
		responseStr.append(ccwServer::instance()->activePlugins.at(i)->version);
		responseStr.append(" | <a href=\"");
		responseStr.append(ccwServer::instance()->activePlugins.at(i)->url);
		responseStr.append("\">");
		responseStr.append(ccwServer::instance()->activePlugins.at(i)->author);
		responseStr.append("</a></p></div>");
	}

	
	responseStr.append("<div style=\"clear:both;\"></div></div>");
	
	
	// Close all the tags
	responseStr.append("</div></body></html>");
	
	httpResponse(responseStr);
}
