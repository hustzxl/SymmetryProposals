/*************************************************************************
	> File Name: main.cpp
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 10时37分53秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include"Feature.h"
#include"CommonDataStructure.h"
#include"ICDAR2011DataSet.h"
#include"Svm.h"

using namespace std;
using namespace cv;

string Phase[2] = {"train", "test"};
string Image[2] = {"ICDAR2013","SVT"};

int TrainPhase( const ICDAR2011DataSet& dataset, const int delta, bool train ) {
	FeatureCollection feature;
	vector<int>proposalLen;
	vector< vector<Rect> >Proposal;
	std::cout<<"collect train feature-------------------------"<<endl;
	GetFeature( dataset, delta, train , feature , proposalLen , Proposal);
	
	cout<<"train svm -----------------------------------------"<<endl;
	SvmTrain( dataset, feature );
}

int TestPhase( const ICDAR2011DataSet& dataset, const int delta, bool test ) {
	FeatureCollection feature;
	vector<int>proposalLen;
	vector< vector<Rect> >Proposal;
	
	std::cout<<"collect test feature-------------------------"<<endl;
	GetFeature( dataset, delta, test , feature, proposalLen, Proposal);
	cout<<"test svm -----------------------------------------"<<endl;

	SvmTest( dataset, feature , proposalLen, Proposal);
}

int main( int argv, char** argc ) {
	string workdir = argc[1];
	int delta = atoi(argc[2]);

	ICDAR2011DataSet dataset(workdir);
	cout<<"start train --------------------------- "<<endl;
	TrainPhase( dataset, delta, 1 );
	cout<<"start test  --------------------------- "<<endl;
    
	TestPhase( dataset, delta, 0 );
}
