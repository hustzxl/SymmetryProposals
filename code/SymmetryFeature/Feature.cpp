/*************************************************************************
	> File Name: Feature.cpp
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 16时13分27秒
 ************************************************************************/

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<core/core.hpp>
#include<imgproc/imgproc.hpp>
#include<highgui/highgui.hpp>
#include<algorithm>
#include<assert.h>
#include"ICDAR2011DataSet.h"
#include"TextlineDetector.h"
#include"FsHelpers.h"
#include"GetFeature.h"
#include"GetProposal.h"

using namespace std;
using namespace cv;

int SaveFeatureData( const string& filename, const FeatureCollection& feature_tmp, const vector<Rect>&proposal ) {
	FILE* file = fopen( filename.c_str(), "wb");
	size_t proposalCount = feature_tmp.size();
	fwrite(&proposalCount,sizeof( size_t ),1,file );
	
	for ( size_t i = 0; i < proposalCount;i++ ) {
		FeatureAtPoint temp = feature_tmp[i];
		Rect rect = proposal[ i ];
		int label = temp.label;
		size_t dim = temp.featureArray.size();

		if ( i == 0 ) {
			fwrite( &dim, sizeof(size_t ), 1, file );
		}
		fwrite( &label , sizeof( int ), 1, file );
		fwrite( &rect.x, sizeof( int), 1, file );
		fwrite( &rect.y, sizeof( int), 1, file );
		fwrite( &rect.width, sizeof( int), 1, file );
		fwrite( &rect.height, sizeof( int), 1, file );
		fwrite( &temp.featureArray[0] , sizeof(float) , dim, file );
	}
	fclose( file );
}

int LoadFeature( const string& featureSavePath, FeatureCollection& feature_tmp, vector<Rect>&proposal ) {
	FILE* file = fopen( featureSavePath.c_str(), "rb" );
	size_t dim ,count;
	int label;
	size_t line = fread( &count, sizeof( size_t ) , 1, file );
	line = fread( &dim, sizeof( size_t ), 1,file );
	
	float* temp = new float [ dim ];
	FeatureAtPoint feature ;

	for ( int i = 0; i < count ;i++ ) {
		Rect rect;
		line = fread( &label, sizeof( int ),1,file );
		line = fread( &rect.x, sizeof( int ),1,file );
		line = fread( &rect.y, sizeof( int ),1,file );
		line = fread( &rect.width, sizeof( int ),1,file );
		line = fread( &rect.height, sizeof( int ),1,file );
		line = fread( temp, sizeof( float ) , dim, file );
		feature.label = label;
		proposal.push_back( rect );
		feature.featureArray = vector<float>( temp , temp + dim );
		feature_tmp.push_back( feature );
	}
	delete [] temp;
	fclose( file );
}

int CalculateFeature( const string& filename, const int delta, bool train, vector<Rect>&boxes, FeatureCollection& feature_tmp,vector<Rect>&proposal,  const string& featureSavePath ) {
	vector< vector<float> > proposal_feature;
	getProposals( filename, delta, proposal, proposal_feature );
	
	if ( train ) {
		getFeature( filename, proposal, boxes, proposal_feature, feature_tmp );
	} else {
		feature_tmp.reserve( proposal.size() );
		vector< Rect >negtive_boxes;
		negtive_boxes.resize(0);
		Mat img = imread( filename.c_str() );
		TextlineDetector TextlineDetect;
		TextlineDetect.CollectTrainingFeatureMy(img, proposal, negtive_boxes,feature_tmp);
	}
	
	SaveFeatureData( featureSavePath ,feature_tmp, proposal );
}

int paramSet( const ICDAR2011DataSet& dataset, int& num, string& ImageDir, string& FeatureSavePath,vector<string>&set, vector< vector<Rect> >&gt_boxes, bool train) {
	num = dataset.train_num;
	ImageDir = dataset.image_dir + "train/";
	FeatureSavePath = dataset.feature_dir + "TrainFeature/"; 
	set = dataset.train_set;
	gt_boxes = dataset.gt_train_boxes;
	
	if ( !train ) {
		num = dataset.test_num;
		set = dataset.test_set;
		ImageDir = dataset.image_dir + "test/";
		FeatureSavePath = dataset.feature_dir + "TestFeature/" ;
		gt_boxes = dataset.gt_test_boxes;
	}
	if ( !CmFile::FileExist( FeatureSavePath ) )
		CmFile::MkDir( FeatureSavePath );
}

int GetFeature( const ICDAR2011DataSet& dataset, const int delta, bool train , FeatureCollection& features_buff, vector<int>&proposalLen, vector< vector<Rect> >&Proposal) {
    int num ;
	string ImageDir,FeatureSavePath;
	vector<string>set;
	vector< vector<Rect> >gt_boxes;
	paramSet( dataset, num,ImageDir,FeatureSavePath,set,gt_boxes ,train );

	for(int test_set_idx = 0; test_set_idx < num ; test_set_idx++) {
        string filename = ImageDir + set[test_set_idx];
		string featurePath = FeatureSavePath + CmFile::GetFileNameWithoutExtension( filename ) + ".data";	
		cout<<filename<<" "<<test_set_idx << " of "<<num<<endl;
		
		FeatureCollection feature_tmp;
		vector<Rect>proposal;
		if ( CmFile::FileExist( featurePath ) ) {
			LoadFeature( featurePath, feature_tmp, proposal );	
		} else {
			vector<Rect> boxes = gt_boxes[ test_set_idx ];
			CalculateFeature( filename, delta,train, boxes,feature_tmp, proposal, featurePath );
		}
		features_buff.insert(features_buff.end(), feature_tmp.begin(), feature_tmp.end());
		proposalLen.push_back( feature_tmp.size() );
		Proposal.push_back( proposal );
		assert( proposal.size() == feature_tmp.size() );
	}	
	//random_shuffle(features_buff.begin(),features_buff.end());
}
