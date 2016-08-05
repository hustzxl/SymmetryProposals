#pragma once
#include<iostream>
#include <string>
#include <vector>
using std::vector;
using std::string;
class CmFile
{
public:
	static int GetNames(const string& rootFolder, const string &fileW, vector<string> &names);
	static bool MkDir(const string&  path);
	static inline string GetNameNE(const string& path);
	static bool FileExist(const string& filePath);
    static string GetFileNameWithoutExtension(const std::string& path);
};
string CmFile::GetNameNE(const string& path)
{
	int start = path.find_last_of("\\/")+1;
	int end = path.find_last_of('.');
	if (end >= 0)
		return path.substr(start, end - start);
	else
		return path.substr(start,  path.find_last_not_of(' ')+1 - start);
}


