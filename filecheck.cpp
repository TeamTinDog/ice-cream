#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "filecheck.h"

using namespace std;

fileCheck::fileCheck()  // Constructor
{
}

// Creates Error Log Vector
void fileCheck::logError(string filename, string date, string seqNum, int lineNum, string message)
{
	stringstream s;
	s << filename << " SEQ# " << seqNum << " Line# " << lineNum << " " << message;
	mErrorLog.push_back(s.str());
}

// Checks Header/Trailer of all Upload Files, and includes City and Route Upload File Checks.
int fileCheck::checkFile(string fileName, int fileType)
{
	std::ifstream file(fileName);
	mFileType = fileType;
	string str;
	string seqNum = "NA";
	int currentSeqNum = -1;
	int prevSeqNum = -9999;
	int lineNumber = 0;
	int prevDateNum = 0;
	string actionCode;
	string routeNum;
	string citylabel;
	string city;
	string state;

	if (fileType == CITY_FILE)		// Clears City Names, Labels, and routes.
	{
		mCityNames.clear();
		mCityLabels.clear();
		mRouteCityMap.clear();
	}

	while (std::getline(file, str))
	{
		int count = 0;
		lineNumber++;


		// Header Check

		if (str.substr(0, 2) != "HD")	    // HD Check
		{
			logError(fileName, "NA", seqNum, lineNumber, " Expected header::not found.");
			return false;
		}

		if (str.substr(2, 1) != " ")
		{
			logError(fileName, "NA", seqNum, lineNumber, " Expected Single Space after HD.");
			return false;
		}

		if (str.substr(3, 4).find_first_not_of("0123456789") != std::string::npos)   // SEQ # Check
		{
			logError(fileName, "NA", seqNum, lineNumber, " Seq# is invalid.");
			return false;
		}
		else
		{
			seqNum = str.substr(3, 4);
			currentSeqNum = atoi(seqNum.c_str());
			if ((prevSeqNum != -9999) && (currentSeqNum != prevSeqNum + 1))			// Compares Currect Seq# to Previous for correctness.
			{
				if (currentSeqNum == 0)
					logError(fileName, "NA", seqNum, lineNumber, " Seq# equals 0000 :: Invalid.");
				else
					logError(fileName, "NA", seqNum, lineNumber, " Seq# is not the next incremental value.");
				return false;
			}

		}

		if (str.substr(7, 6) != "      ")
		{
			logError(fileName, "NA", seqNum, lineNumber, " Expected 6 spaces after Seq#.");
			return false;
		}

		if (str.substr(13, 4).find_first_not_of("0123456789") != std::string::npos)		// Year format Check
		{
			logError(fileName, "NA", seqNum, lineNumber, " Year is formatted incorrect.");
			return false;
		}
		if (str.substr(17, 1) != "-")
		{
			logError(fileName, "NA", seqNum, lineNumber, " Expected - :: Not Found.");
			return false;
		}

		string month = str.substr(18, 2);
		if (month.find_first_not_of("0123456789") != std::string::npos)					// Month format Check
		{
			logError(fileName, "NA", seqNum, lineNumber, " Month is formatted incorrect.");
			return false;
		}
		else
		{
			if (atoi(month.c_str()) < 1 || (atoi(month.c_str()) > 31))
			{
				logError(fileName, "NA", seqNum, lineNumber, " Invalid Month of the Year.");
				return false;
			}

		}

		if (str.substr(20, 1) != "-")
		{
			logError(fileName, "NA", seqNum, lineNumber, " Expected - :: Not Found.");
			return false;
		}

		string day = str.substr(21, 2);
		if (str.substr(21, 2).find_first_not_of("0123456789") != std::string::npos)		// Day format Check
		{
			logError(fileName, "NA", seqNum, lineNumber, " Day is formatted incorrect.");
			return false;
		}
		else
		{
			if (atoi(day.c_str()) < 1 || (atoi(day.c_str()) > 31))
			{
				logError(fileName, "NA", seqNum, lineNumber, " Invalid day of the month.");
				return false;
			}


		}

		// Converts Date Strings to int
		string dateString = str.substr(13, 4) + str.substr(18, 2) + str.substr(21, 2);
		int dateNum = atoi((str.substr(13, 4) + str.substr(18, 2) + str.substr(21, 2)).c_str());
		if (dateNum < prevDateNum)
		{
			logError(fileName, dateString, seqNum, lineNumber, " Date is out of Sequence.");
			return false;
		}


		while (std::getline(file, str))		// Loops File lines
		{
			bool lineValid = true;
			lineNumber++;
			if (str.substr(0, 1) != "T")	// T Check
			{
				switch (mFileType)			// File Type Error Checking
				{
				case CITY_FILE:				// City File Check
					citylabel = str.substr(0, 20);
					city = str.substr(20, 20);
					state = str.substr(40, 2);
					mCityLabels.insert(citylabel);
					mCityNames.insert(city);
					count++;
					break;

				case ROUTE_FILE:			// Route File Check
					actionCode = str.substr(0, 1);
					if (actionCode.find_first_not_of("ACD") != std::string::npos)
					{
						logError(fileName, dateString, seqNum, lineNumber, " Expected Action Code of A, C, or D :: Not Found. Skipping line.");
						count++;
						continue;
					}
					routeNum = str.substr(1, 4);
					if (routeNum.find_first_not_of("0123456789") != std::string::npos)
					{
						logError(fileName, dateString, seqNum, lineNumber, " Route # is formatted incorrect. Skipping line.");
						count++;
						continue;
					}

					if (actionCode == "A")
					{
						int pos = 5;
						citylabel = str.substr(pos, 20);  // first city label in route

						if (!VerifyCityLabel(citylabel))
						{
							logError(fileName, dateString, seqNum, lineNumber, "First city in Route invalid or blank, Skipping line.");
							count++;
							continue;
						}

						if (mRouteCityMap.find(citylabel) != mRouteCityMap.end())
						{
							logError(fileName, dateString, seqNum, lineNumber, "City Label already exists in a different route, Skipping line.");
							count++;
							continue;
						}

						mRouteCityMap[citylabel] = routeNum;

						for (pos = 25; pos <= 185 && lineValid; pos += 20)		// Loops CityLabel 2-9 if they exist.
						{
							citylabel = str.substr(pos, 20);
							if (citylabel == "                    ")
								break;

							if (!VerifyCityLabel(citylabel) || mRouteCityMap.find(citylabel) != mRouteCityMap.end())
							{
								logError(fileName, dateString, seqNum, lineNumber, "Invalid City Label, Skipping line.");
								lineValid = false;
							}

							// This is a map of used City Labels to RouteNum string.
							mRouteCityMap[citylabel] = routeNum;
						}
					}

					else if (actionCode == "C")		// Change Action.
					{
						citylabel = str.substr(5, 20);
						if (mCityNames.find(citylabel) == mCityNames.end())
						{
							logError(fileName, dateString, seqNum, lineNumber, "Invalid City Name in Change Request, Skipping line.");
							lineValid = false;
						}
						else
						{
							vector<string> newLabels;
							vector<string> oldLabels;
							for (auto it : mRouteCityMap)
							{
								if (it.second == routeNum)
								{
									oldLabels.push_back(it.first);
									int num1 = it.first.find_first_of("123456789");
									string suffix = it.first.substr(num1);
									num1 = citylabel.find_last_of("abcdefghijklmnopqrstuvwxyz");
									newLabels.push_back(citylabel.substr(0,num1+1) + " " + suffix + "                                     ");
								}
							}

							// Check if newly formed labels are valid or already exist in a route
							for (auto it : newLabels)
							{
								it = it.substr(0, 20);
								if (!VerifyCityLabel(it) || mRouteCityMap.find(it) != mRouteCityMap.end())
								{
									logError(fileName, dateString, seqNum, lineNumber, "Invalid City Label in Change Request, Skipping line.");
									lineValid = false;
									break;
								}
							}
							if (lineValid)
							{
								// Change out old labels for new ones...
								for (auto it : oldLabels)
									mRouteCityMap.erase(it);

								for (auto it : newLabels)
									mRouteCityMap[it.substr(0,20)] = routeNum;
							}
						}
					}
					else if (actionCode == "D")		// Delete Action
					{
						if (str.substr(5).find_first_not_of(" ") != string::npos)
						{
							logError(fileName, dateString, seqNum, lineNumber, "City Label appears on delete line, Skipping line.");
							lineValid = false;
						}
						vector<string> deletions;
						for (auto it : mRouteCityMap)
						{
							if (it.second == routeNum)
								deletions.push_back(it.first);
						}
						for (auto it : deletions)
							mRouteCityMap.erase(it);

					}

					count++;

					break;

				case LOAD_FILES:		// Load File's Row Count Check
					
					if ( !(str.substr(0, 2) == "IR" || str.substr(0, 2) == "TR" || str.substr(0, 2) == "SR") )
						count++;

					break;


				}
			
			}

			else
			{
				if (str.substr(2, 4).find_first_not_of("0123456789") != std::string::npos)			// Row Count Format
					logError(fileName, "NA", seqNum, lineNumber, " Trailer Row# is formatted incorrect.");

				// Trailer Row Count Check
				int Tcount = std::atoi(str.substr(2, 4).c_str());
				if (count != Tcount)
					logError(fileName, "NA", seqNum, lineNumber, " Trailer line count is invalid.");

				//cout << "Line #" << lineNumber << " VALID ENTRY" << endl;
				break;
			}
		}
		prevDateNum = dateNum;
	}

	return true;
}

bool fileCheck::VerifyCityLabel(string label)		// Verification of City File CityLabels to Route File CityLabels.
{
	for (auto cityIter : mCityLabels)
		if (cityIter == label)
			return true;

	return false;
}

void fileCheck::writeErrorfile(string fileName)		// Error Log Output.
{
	for (auto it : mErrorLog)
	{
		cout << it << endl;
		ofstream ErrorLog;
		ErrorLog.open(fileName);
		ErrorLog << it;
		ErrorLog.close();
	}

}