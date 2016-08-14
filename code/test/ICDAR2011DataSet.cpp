#include"stdafx.h"
#include "FsHelpers.h"
#include "ICDAR2011DataSet.h"
#include<fstream>
using std::cout;
using cv::Rect;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::string;
using std::vector;
void ICDAR2011DataSet::LoadAnnotation(const string file_path, vector< vector<Rect> > &boxes)
{
	vector<Rect> _boxes;
	ifstream in(file_path.c_str());

	if(false == in.is_open())
	{
		cout << "err in ICDAR2011DataSet::LoadAnnotation" << endl;
	}
	else
	{
		string str_buffer;
		while(getline(in, str_buffer))
		{
			int x1, y1, x2, y2;
			sscanf(str_buffer.c_str(), "%d,%d,%d,%d", &x1, &y1, &x2, &y2);
			_boxes.push_back(Rect(x1, y1, x2 - x1, y2 - y1));
		}
	}
	boxes.push_back(_boxes);
	in.close();
}

void ICDAR2011DataSet::LoadCharacterAnnotation(const string file_path, vector< vector<Rect> > &boxes)
{
	vector<Rect> _boxes;
	ifstream in(file_path.c_str());

	if(false == in.is_open())
	{
		cout << "err in ICDAR2011DataSet::LoadAnnotation" << endl;
	}
	else
	{
		string str_buffer;
		while(getline(in, str_buffer))
		{
			int x1, y1, x2, y2;
			sscanf(str_buffer.c_str(), "%d %d %d %d", &x1, &y1, &x2, &y2);
			if(x2 == x1 || y2 == y1)
			{
				continue;
			}
			_boxes.push_back(Rect(x1, y1, x2 - x1, y2 - y1));
		}
	}
	boxes.push_back(_boxes);
	in.close();
}

void ICDAR2011DataSet::LoadCharacterAnnotation(const string file_path, vector< vector<Rect> > &boxes,bool flag)
{
	vector<Rect> _boxes;
	ifstream in(file_path.c_str());

	if(false == in.is_open())
	{
		cout << "err in SVT::LoadAnnotation" << file_path<<endl;
	}
	else
	{
		string str_buffer;
		while(getline(in, str_buffer))
		{
			int x1, y1, x2, y2;
			sscanf(str_buffer.c_str(), "%d %d %d %d", &x1, &y1, &x2, &y2);
			_boxes.push_back(Rect(x1, y1, x2, y2));
		}
	}
	boxes.push_back(_boxes);
	in.close();
}

ICDAR2011DataSet::ICDAR2011DataSet(const string &_work_dir)
{
	string feature_type = "full_labgto/";
	work_dir = _work_dir;
	image_dir = work_dir + "Images/";
	annotation_dir = work_dir + "Annotation/";
	feature_dir = work_dir + "Feature/";
	result_dir = work_dir + "Result/";
	if ( !CmFile::FileExist( result_dir ) )
		CmFile::MkDir(result_dir);
	result_dir += feature_type;
	if ( !CmFile::FileExist( result_dir ) )
		CmFile::MkDir(result_dir);
	
	if ( !CmFile::FileExist( feature_dir ) )
		CmFile::MkDir(feature_dir);
	feature_dir += feature_type;
	if ( !CmFile::FileExist( feature_dir ) )
		CmFile::MkDir(feature_dir);
	
	model_dir = work_dir + "Model/";
	if ( !CmFile::FileExist( model_dir ) )
		CmFile::MkDir(model_dir);
	model_dir += feature_type;
	if ( !CmFile::FileExist( model_dir ) )
		CmFile::MkDir(model_dir);

	CmFile::GetNames(image_dir + "train/", "*.jpg", train_set);
	CmFile::GetNames(image_dir + "test/", "*.jpg", test_set);

	train_num = train_set.size();
	test_num = test_set.size();

	//load annotation
	if (work_dir.find("SVT") != string::npos) {
		for(int i = 0; i < train_num; i++) {
			string name_ne = CmFile::GetNameNE(train_set[i]);
			LoadCharacterAnnotation(annotation_dir + "train/" + "gt_" + name_ne + ".txt", gt_train_boxes,1);
		}
		
		for(int i = 0; i < test_num; i++) {
			string name_ne = CmFile::GetNameNE(test_set[i]);
			LoadCharacterAnnotation(annotation_dir + "test/" + "gt_" + name_ne + ".txt", gt_test_boxes,1);
		}	
	} else {
		for(int i = 0; i < train_num; i++) {
			string name_ne = CmFile::GetNameNE(train_set[i]);
			LoadCharacterAnnotation(annotation_dir + "train/" +  "gt_" + name_ne + ".txt", gt_train_boxes);
		}

		for(int i = 0; i < test_num; i++) {
			string name_ne = CmFile::GetNameNE(test_set[i]);
			LoadAnnotation(annotation_dir + "test/" + "gt_" + name_ne + ".txt", gt_test_boxes);
		}
	}
}

ICDAR2011DataSet::ICDAR2011DataSet(void)
{
}


ICDAR2011DataSet::~ICDAR2011DataSet(void)
{
}
