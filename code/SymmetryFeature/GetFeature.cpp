/*************************************************************************
	> File Name: GetFeature.cpp
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 15时40分05秒
 ************************************************************************/

#include<iostream>
#include<vector>
#include<string>
#include<assert.h>
#include"core/core.hpp"
#include<highgui/highgui.hpp>
#include<features2d/features2d.hpp>
#include<imgproc/imgproc.hpp>
#include <algorithm>
#include "TextlineDetector.h"
#include"GetFeature.h"

using namespace std;
using namespace cv;

#define min(a,b) ((a)>(b))?(b):(a)
#define max(a,b) ((a) > (b)) ? (a):(b)

double cal_iou(Rect& gt,Rect& proposal) {
	double result;
	int x1 = min(gt.x,proposal.x);
	int y1 = min(gt.y,proposal.y);
	int x2 = max(gt.x + gt.width,proposal.x + proposal.width);
	int y2 = max(gt.y + gt.height,proposal.y + proposal.height);
	double unio = (x2 - x1 ) * (y2 - y1);
	x1 = max(gt.x,proposal.x);
	y1 = max(gt.y,proposal.y);
	x2 = min(gt.x + gt.width,proposal.x + proposal.width);
	y2 = min(gt.y + gt.height,proposal.y + proposal.height);
	if (x2 < x1 || y2 < y1)
		return 0;
	double insect = (x2- x1) * (y2 - y1);
	result = insect / unio;
	return result; 
}
	
int getFeature( const string& ImageName , const vector<Rect>& proposal, const vector< Rect> & gt_train_boxes, const vector< vector<float> >&proposal_feature, FeatureCollection& features_tmp ) {
	Mat img = imread( ImageName.c_str() );
	assert( !img.empty()  );

	vector<Rect> probbs_positive_boxes,probbs_negtive_boxes;
	vector<int>p_idx;
	vector<int>n_idx;

	for (size_t pro_idx = 0 ; pro_idx < proposal.size() ; pro_idx ++) {
		int flag = 0;				
		Rect probbs = proposal[pro_idx];
		
		for (int gt_idx = 0;gt_idx< gt_train_boxes.size();gt_idx++) {
			Rect gtbbs = gt_train_boxes[gt_idx];

			double iou = cal_iou(gtbbs,probbs);
			//double area_ratio = float(probbs.width * probbs.height) / float(gtbbs.width* gtbbs.height);
			if (iou > 0.7) {
				probbs_positive_boxes.push_back(proposal[pro_idx]);
				p_idx.push_back(pro_idx);
/*namedWindow("test");
rectangle( img, gtbbs, Scalar(0,0,255), 2 );
rectangle( img, proposal[pro_idx], Scalar(0,255,0), 2 );
imshow("test.jpg", img );
waitKey();*/
				break;
			} else if (iou < 0.1) {
				flag++;
			}
		}
		if ((flag == gt_train_boxes.size())) {
			n_idx.push_back(pro_idx);
		}
	}
	// unique negtive
	random_shuffle(n_idx.begin(),n_idx.end());
	size_t count = min(n_idx.size(),p_idx.size());

	for (size_t i = 0;i < count;i++) {
		int idx  = n_idx[i];
		probbs_negtive_boxes.push_back(proposal[idx]); 
	}
	std::cout<<"positive rect num :"<<probbs_positive_boxes.size()<<std::endl;

	//get feature
	TextlineDetector detector;
	features_tmp.reserve(probbs_positive_boxes.size() + probbs_negtive_boxes.size());
	detector.CollectTrainingFeatureMy(img, probbs_positive_boxes, probbs_negtive_boxes,features_tmp);
}
