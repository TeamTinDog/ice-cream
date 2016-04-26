#include <vector>
#include <string>
#include <map>
#include <unordered_set>
const int CITY_FILE = 0;
const int ROUTE_FILE = 1;
const int LOAD_FILES = 2;

using namespace std;

typedef map<string, string> stringMap;


class fileCheck
{
public:
	fileCheck();
	~fileCheck(){};
	int checkFile(string fileName, int fileType);
	bool VerifyCityLabel(string label);
	void writeErrorfile(string fileName);

	int getNumErrors() { return mErrorLog.size(); }
	void clearErrors() { mErrorLog.clear(); }

	// Utility function to convert strings to upper case
	string stringToUpper(string str);
private:
	vector<string> mErrorLog;
	unordered_set<string> mCityLabels;
	unordered_set<string> mCityNames;
	stringMap mRouteCityMap;
	void logError(string filename, string date, string seqNum, int lineNum, string message);
	int mFileType;

};