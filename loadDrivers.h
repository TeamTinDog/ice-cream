// William Do
// CIS 375
// Team Tin Dog

#ifndef LOAD_DRIVERS
#define LOAD_DRIVERS

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void loadDrivers()
{
	ifstream driversFileIn("loadDrivers.txt");
	ofstream errorFileOut("errorLog.txt", ios::app);

	char lineIn[256];				// To get lines from truck route file
	int checkDigit;					// To verify IDs are composed of integers
	int lineCount = 0;				// Keep track of line count in case errors need to be reported
	string date;					// Store date to label error file
	string seqNum;					// Store sequence number to label error file
	string pushDriver;				// 
	bool noErrors = true;			// Keep track of whether errors occured
	bool validDriverID = true;		// 
	bool validLine = true;			// 
	bool catJobsLeft = true;		// Keep track of whether unassigned catering jobs exist
	bool routesLeft = true;			// Keep track of whether unassigned routes exist
	vector<int> driverIDs;


	if (driversFileIn)
	{
		// Get first line and assign sequence numbers and dates
		driversFileIn.getline(lineIn, 256, '\n');
		seqNum = lineIn;
		seqNum = seqNum.substr(3, 4);
		date = lineIn;
		date = date.substr(13, 10);

		driversFileIn.getline(lineIn, 256, '\n'); // Get second line

		// Store all valid driver IDs in a vector
		while (lineIn[0] != 'T' && !driversFileIn.eof())
		{
			lineCount++;
			validDriverID = true;

			// Make sure driver ID is four consecutive integers
			for (int i = 0; i < 4; i++)
			{
				checkDigit = lineIn[i];
				if (checkDigit < 48 || checkDigit > 57)
				{
					cout << "LINE " << lineCount << ": Invalid Driver ID" << endl;
					errorFileOut << "LINE " << lineCount << ": Invalid Driver ID" << endl;
					validDriverID = false;
					noErrors = false;
					break;
				}
			}

			// Make sure line does not contain extraneous characters that are not spaces
			if (validDriverID)
			{
				for (int i = 4; i < 256; i++)
				{
					if (lineIn[i] == '\n' || lineIn[i] == '\0')
						break;
					if (lineIn[i] != ' ')
					{
						cout << "LINE " << lineCount << ": Invalid truck ID" << endl;
						errorFileOut << "LINE " << lineCount << ": Invalid truck ID" << endl;
						noErrors = false;
						validLine = false;
						break;
					}
				}
			}

			// Make sure driver ID exists in database and add to vector
			if (validLine)
			{
				pushDriver = lineIn;
				pushDriver = pushDriver.substr(0, 4);
				/*
				TO BE IMPLEMENTED: Check database for this driver ID before pushing to driverIDs vector
				If the driver ID does not exist in database, print error message and do not add to driverIDs
				*/
				driverIDs.push_back(stoi(pushDriver));
			}

			driversFileIn.getline(lineIn, 256, '\n'); // Get next line
		}

		
		while (catJobsLeft && driverIDs.size() != 0)
		{
			/*
			TO BE IMPLEMENTED
			- Query database for unassigned catering jobs
			- If any exist, assign first available driver, then remove driver ID from driverIDs vector
			- Repeat above until no catering jobs or no drivers remaining
			*/
			catJobsLeft = false; // This statement is here to prevent an infinite loop and SHOULD NOT be left in once the above comment is implemented
		}

		while (routesLeft && !driversFileIn.eof())
		{
			/*
			TO BE IMPLEMENTED
			- Query database for an unassigned route
			- If one exists, query database for the city zones on that route
			- For each driver ID in driverIDs:
				- Query the database for that driver's average sales for each city zone
				- Sum the averages
				- Keep track of the driver with the highest total
			- Once this total has been found for each driver in driverIDs, assign the driver with the highest total to the route
			- REMOVE THIS DRIVER from driverIDs
			- Repeat above until no routes or no drivers remain
			*/
			routesLeft = false; // This statement is here to prevent an infinite loop and SHOULD NOT be left in once the above comment is implemented
		}
		


	}
	else
	{
		cout << "Load Drivers upload file: ERROR - loadDrivers.txt missing" << endl;
		errorFileOut << "Load Drivers upload file: ERROR - loadDrivers.txt missing" << endl;
		noErrors = false;
	}

	if (noErrors)
	{
		cout << "There were no errors" << endl;
		errorFileOut << "There were no errors" << endl;
	}

	driversFileIn.close();
	errorFileOut.close();

}

#endif
