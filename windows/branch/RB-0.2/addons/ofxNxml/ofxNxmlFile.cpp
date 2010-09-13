/*
 *  ofxNxmlFile.cpp
 *  ccw
 *
 *  Created by Jake on 01/07/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxNxmlFile.h"

ofxNxmlFile::ofxNxmlFile(string location){
	if( XML.loadFile(location) ) {
		cout << "NXML file successfully opened\n";
		
		XML.pushTag("nxml");
		
		/* Nested for loops to read the file. Straightforward until object level */
		int numApplications = XML.getNumTags("nx:Application");
		
		/* Arrays to store objects */
		//ofxNxmlObject *application[numApplications-1]; 
		
		/* TEMP */
		ofxNxmlObject *application[2]; 

		for (int i = 0; i < numApplications; i++)
		{
			application[i] = new ofxNxmlObject(XML.getAttribute("nx:Application","id","",i));
		
			// Push inside each application
			XML.pushTag("nx:Application", i);
			int numStages = XML.getNumTags("nx:Stage");
			cout << "App\n";
			
			for (int j = 0; j < numStages; j++)
			{
				application[i]->stage[j] = new ofxNxmlObject(XML.getAttribute("nx:Stage","id","",j));
				XML.pushTag("nx:Stage", j);
				int numScenes = XML.getNumTags("nx:Scene");
				
				for (int k = 0; k < numScenes; k++)
				{
					application[i]->stage[j]->scene[k] = new ofxNxmlObject(XML.getAttribute("nx:Scene","id","",k));
					XML.pushTag("nx:Scene", k);
					
					// Objects are more complicated, as they may be nested within each other.
					// Get number of top level objects:
					int numObjects = XML.getNumTags("nx:Object");
					int numChildObjects = 0;
					int currentDepth = 0;
					
					// For each top level object
					for (int l = 0; l < numObjects; l++)
					{
						/* Pointer to track the address of instances, add children correctly, initiated to new top-level object */
					    ofxNxmlObject * currPosition = application[i]->stage[j]->scene[k]->object[l] = new ofxNxmlObject(XML.getAttribute("nx:Object","id","",l));
						ofxNxmlObject * prevPosition = NULL;
					 	cout << "Object\n";
						
						// Push inside the object tag.
						XML.pushTag("nx:Object", l);
						int currDepth = 0;
						
						int currObjectIndex[MAX_OBJECT_DEPTH] = {}; // all to zero??
						bool allChildrenEvaluated = false;
						
						// Loops until flag indicating creation of all sub-objects is set.
						// Drills down through object tags adding 0 indexed objects.
						// Once the last 0 indexed tag is reached, parent index is incremented and evaluated recursively until top level reached.
						while (!allChildrenEvaluated)
						{
							numChildObjects = XML.getNumTags("nx:Object");
							if (currObjectIndex[currDepth] < numChildObjects)
							{
								// prevPosition = currentPosition;
								// currPosition = currentPosition->object[currentObjectIndex[currDepth]] = new Object();
								cout << "SubObject\n";
								XML.pushTag("nx:Object",currObjectIndex[currDepth]);
								currDepth++;
							}
							else if ( (currDepth == 0) && ( currObjectIndex[0] == numChildObjects ))
							{
								// All children have been created, break out of while loop
								allChildrenEvaluated = true;
							}
							else
							{
								// Back up and increment index
								XML.popTag();
								currDepth--;
								currObjectIndex[currDepth]++;
								// currPosition = prevPosition;
							}
						}
						XML.popTag();							
					}
					XML.popTag();
				}
				XML.popTag();
			}
			XML.popTag();
		}
		
		} else // if the file wasn't opened correctly
		{
			cerr << "NXML ERROR: Could not open configuration file. Check path.\n";
		}
}