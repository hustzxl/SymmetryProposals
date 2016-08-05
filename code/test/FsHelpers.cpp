#include "FsHelpers.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
//using boost::filesystem::detail;
int CmFile::GetNames(const string& rootFolder, const string &fileW, vector<string> &names) {
	 std::string ext = fileW;
	// erase '*'
	if (ext[0] = '*'){
		string::iterator it = ext.begin();
		ext.erase(it,it+ 1);
	}
        boost::filesystem::path p(rootFolder);
	
	boost::filesystem::directory_iterator endIter;
        for (boost::filesystem::directory_iterator iter(p);
             iter != endIter; ++iter)
        {
            if (boost::filesystem::is_regular_file(iter->path()) && iter->path().extension().string() == ext)
            {
                names.push_back(iter->path().filename().string());
            }
        }
	return (int)names.size();
}
bool CmFile::MkDir(const string&  path){
	 boost::filesystem::path p(path);
	 return create_directory(p);
}
bool CmFile::FileExist(const string& filePath)
{
	if (filePath.size() == 0)
		return false;
	boost::filesystem::path p(filePath);
    	return boost::filesystem::exists(p);
}

std::string CmFile::GetFileNameWithoutExtension(const std::string& path)
{
        boost::filesystem::path p(path);

            return p.stem().string();
}


