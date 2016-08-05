/*************************************************************************
	> File Name: SvmTrain.cpp
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 20时10分08秒
 ************************************************************************/

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<core/core.hpp>
#include"CommonDataStructure.h"
#include"ICDAR2011DataSet.h"
#include"FsHelpers.h"
#include<assert.h>
#include<ml/ml.hpp>

using namespace std;
using namespace cv;

#define NORM 0

void svm_train(const FeatureCollection& features,const ICDAR2011DataSet& dataset ,vector<int>& false_idx) {
	size_t feature_count = features.size();
	int validation_feature_count = 5000;
    size_t dim = features[0].featureArray.size();
	Mat train_data(feature_count - validation_feature_count, dim, CV_32F);
    Mat train_label(feature_count - validation_feature_count, 1, CV_32F);
	
	for(int feature_idx = 0; feature_idx < feature_count - validation_feature_count; feature_idx++) {
		float sum_norm = 0;
		if (NORM) 
		    sum_norm = accumulate(features[feature_idx].featureArray.begin(),features[feature_idx].featureArray.end(),0.0);
		else {
		    for (int i = 0;i< dim;i++) 
		        sum_norm += features[feature_idx].featureArray[i] * features[feature_idx].featureArray[i];
		    sum_norm = sqrt(sum_norm);
		}
		for(int dim_idx = 0; dim_idx < dim; dim_idx++) {
			FeatureAtPoint fea = features[feature_idx];
			train_data.at<float>(feature_idx, dim_idx) = features[feature_idx].featureArray[dim_idx]/sum_norm;
		}
		train_label.at<float>(feature_idx) = features[feature_idx].label;
    }
	
	std::cout<<"svm train-------------------------------------"<<std::endl; 
	CvSVM SVM;
	CvBoost boost;  
	CvBoostParams params = CvBoostParams(CvBoost::REAL, 50, 0.95, 5, false, 0 );	
	boost.train(train_data, CV_ROW_SAMPLE,train_label, Mat(), Mat(), Mat(),Mat(),params);
	
	std::cout<<"svm validad-----------------------------------"<<std::endl;
    // Calculate trainning error
	cout<<feature_count<<endl;

	Mat test_data(1, dim, CV_32F);
    float predict_correct = 0, positive_cnt = 0, negative_cnt = 0;
	for(int feature_idx = feature_count - validation_feature_count; feature_idx < feature_count; feature_idx++) {
        float sum_norm = 0 ;
        if (NORM) 
            sum_norm = accumulate(features[feature_idx].featureArray.begin(),features[feature_idx].featureArray.end(),0.0);
        else {
            for (int i = 0;i< dim;i++) 
                sum_norm += features[feature_idx].featureArray[i] * features[feature_idx].featureArray[i];
            sum_norm = sqrt(sum_norm);
        }
        
        for(int dim_idx = 0; dim_idx < dim; dim_idx++) {
            test_data.at<float>(0, dim_idx) = features[feature_idx].featureArray[dim_idx]/sum_norm;
        }
		
		if(features[feature_idx].label == boost.predict(test_data)) {
			predict_correct++;
		} else {
			false_idx.push_back(feature_idx);
		}
        positive_cnt += features[feature_idx].label == 1 ? 1 : 0;
        negative_cnt += features[feature_idx].label == 0 ? 1 : 0;
    }
	cout << "Training accuracy:" << predict_correct / 5000 << " pos_cnt:" << positive_cnt << " neg_cnt:" << negative_cnt << endl;
	boost.save(((dataset.model_dir + "boost.model").c_str()));
}

int SvmTrain( const ICDAR2011DataSet& dataset, FeatureCollection& features ) {
	for (int j = 0;j < 3;j++) {
		vector<int>false_idx;
		false_idx.clear();
		svm_train(features,dataset, false_idx);
		random_shuffle(false_idx.begin(),false_idx.end());
		for (int i = 0;i<100;i++) {
			int idx = false_idx[i];
			features.push_back(features[idx]);
		}
		random_shuffle(features.begin(),features.end());
	}
}

int SvmTest( const ICDAR2011DataSet& dataset, FeatureCollection& features , const vector<int>&ProposalLen, const vector< vector<Rect> >&Proposal) {
	long sumProposal = accumulate( ProposalLen.begin(), ProposalLen.end(), 0 );
	assert( sumProposal == features.size() );
	
	CvBoost boost;  
	boost.load((dataset.model_dir + "boost.model").c_str());
	
	for( size_t image_idx = 0; image_idx < ProposalLen.size() ;image_idx ++ ) {
		string filename = dataset.test_set[ image_idx ];
		string resultPath = dataset.result_dir + CmFile::GetFileNameWithoutExtension( filename ) + ".txt"; 
		cout<<"result save path "<<resultPath<<" "<<image_idx<<" of "<<dataset.test_num<<endl;

		ofstream score_out( resultPath.c_str() );
		int num = ProposalLen[ image_idx ];
		vector<Rect>proposal = Proposal[ image_idx ];
		assert( num == proposal.size() );

		long StartIndex = 0;
		for (int i = 0;i < num ;i++) {
			FeatureAtPoint featurePoint = features[ i + StartIndex ];
			int dim = featurePoint.featureArray.size();

			Mat test_data(1, dim, CV_32F);
			double sum_norm = 0;
			if (NORM) {
				sum_norm = accumulate(featurePoint.featureArray.begin(),featurePoint.featureArray.end(),0.0);
			} else {
				for (int i =  0;i< dim;i++)  
					sum_norm += featurePoint.featureArray[i] * featurePoint.featureArray[i];
				sum_norm = sqrt(sum_norm);
			}
			vector<float>sample;
			for (int dim_idx = 0;dim_idx < dim;dim_idx++) {
				test_data.at<float>(0, dim_idx) = featurePoint.featureArray[dim_idx]/ sum_norm;
				sample.push_back(featurePoint.featureArray[dim_idx]/sum_norm);
			}
			double score = boost.predict(test_data,Mat(),Range::all(),false,true);
	//		score = score * (-1);
			score_out<< proposal[i].x << " " << proposal[i].y << " "
				<< proposal[i].width << " "  << proposal[i].height << " "
				<< score << endl;
		}                
		StartIndex += num;
		score_out.close();
	}
}
