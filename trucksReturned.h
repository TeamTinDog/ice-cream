// William Do
// CIS 375
// Team Tin Dog

#ifndef TRUCKS_RETURNED
#define TRUCKS_RETURNED

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

void trucksReturned()
{
	// Read from truckUpload and write to errorLog
	ifstream uploadFileIn("trucksReturned.txt");
	ofstream errorFileOut("errorLog.txt", ios::app);

	int lineCount = 0;
	int checkDigit;
	int rowTotal = 0;
	char lineIn[256];
	bool validRowTotal = true;
	bool validTruckID = true;

	

	uploadFileIn.getline(lineIn, 256, '\n'); // read in first line and ignore
	uploadFileIn.getline(lineIn, 256, '\n'); // read in second line

	while (lineIn[0] != 'T' && !uploadFileIn.eof()) // Repeat until Trailer or end of file
	{
		lineCount++;
		validTruckID = true;

		// Make sure first 4 characters are integers
		for (int i = 0; i<4; i++)
		{
			checkDigit = lineIn[i];
			if (checkDigit < 48 || checkDigit > 57)
			{
				errorFileOut << "LINE" << to_string(lineCount) << ": Invalid truck ID" << endl;
				validTruckID = false;
				break;
			}
		}

		// Make sure line does not contain any other characters besides whitespace
		if (validTruckID)
		{
			for (int i = 4; i < 256; i++)
			{
				if (lineIn[i] == '\n' || lineIn[i] == '\0')
					break;
				if (lineIn[i] != ' ')
				{
					errorFileOut << "LINE " << to_string(lineCount) << ": Invalid truck ID" << endl;
					break;
				}
			}
		}

		/*
		TO BE IMPLEMENTED: Find Truck ID in Maintenance, remove from Maintenance, add to Available
		*/

		uploadFileIn.getline(lineIn, 256, '\n'); // get next line
	}

	if (lineIn[0] == 'T')
	{
		// Make sure row total is four integers
		for (int i = 2; i < 6; i++)
		{
			checkDigit = lineIn[i];
			if (checkDigit < 48 || checkDigit > 57)
			{
				errorFileOut << "TRAILER: Invalid row total" << endl;
				validRowTotal = false;
				break;
			}
		}

		// Calculate row total and compare to lineCount
		if (validRowTotal)
		{
			for (int i = 2; i < 6; i++)
			{
				checkDigit = lineIn[i];
				checkDigit -= 48;
				rowTotal += checkDigit * pow(10, 5 - i);
			}
			if (rowTotal != lineCount)
			{
				errorFileOut << "TRAILER: Inaccurate row total" << endl;
			}
		}
	}
	else
	{
		errorFileOut << "NO TRAILER/TRAILER FORMATTED INCORRECTLY" << endl;
	}


	uploadFileIn.close();
	errorFileOut.close();
}

#endif
