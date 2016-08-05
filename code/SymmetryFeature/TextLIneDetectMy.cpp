#include "stdafx.h"
#include "FsHelpers.h"
#include <opencv/highgui.h>
#include <opencv/ml.h>
extern "C" {
	#include "vl/lbp.h"
}
#include "TextlineDetector.h"
#include "CommonDataStructure.h"

#include<deque>
#include<queue>
using namespace cv;
using namespace std;
void TextlineDetector::SaveFeatureMy(const string &path, FeatureCollection &feature)
{
	FILE * file = fopen(path.c_str(), "wb");
	int feature_count = feature.size();
	fwrite(&feature_count, sizeof(int), 1, file);
	for(int feature_idx = 0; feature_idx < feature_count; feature_idx++)
	{
		int feature_dim = feature[feature_idx].featureArray.size();
		fwrite(&(feature[feature_idx].x), sizeof(int), 1, file);
		fwrite(&(feature[feature_idx].y), sizeof(int), 1, file);
		fwrite(&(feature[feature_idx].label), sizeof(int), 1, file);
		fwrite(&feature_dim, sizeof(int), 1, file);
		fwrite(feature[feature_idx].featureArray.data(), sizeof(float), feature_dim, file);
	}
	fclose(file);
}

void TextlineDetector::LoadFeatureMy(const string &path, FeatureCollection &feature)
{
	FILE * file = fopen(path.c_str(), "rb");
	int feature_count;
	size_t count = fread(&feature_count, sizeof(int), 1, file);
	feature.resize(feature_count);
	for(int feature_idx = 0; feature_idx < feature_count; feature_idx++)
	{
		int x, y, label, feature_dim;
		float feature_value;
		count = fread(&x, sizeof(int), 1, file);
		count = fread(&y, sizeof(int), 1, file);
		count = fread(&label, sizeof(int), 1, file);
		count = fread(&feature_dim, sizeof(int), 1, file);

		for(int dim_idx = 0; dim_idx < feature_dim; dim_idx++)
		{
			count = fread(&feature_value, sizeof(float), 1, file);
			feature[feature_idx].featureArray.push_back(feature_value);
		}
		feature[feature_idx].x = x;
		feature[feature_idx].y = y;
		feature[feature_idx].label = label;
	}
	fclose(file);
}

/*void TextlineDetector::CollectFeatureMy(const Mat& image,const vector<Rect>& gt_boxes,FeatureCollection& features) {	
	for(int gt_boxes_idx = 0; gt_boxes_idx < gt_boxes.size(); gt_boxes_idx++) {
		Mat croped_image(image, gt_boxes[gt_boxes_idx]);
		FeatureCollection feature_buff;
		feature_buff.clear();
		int resized_scale = 1;	
		ExtractFeatureAtScale(croped_image, resized_scale, feature_buff);
		features.insert(features.end(),feature_buff.begin(), feature_buff.end() );
	}
}*/

/*void TextlineDetector::CollectTrainingFeatureMy(const Mat &image, const vector<Rect> &positive_boxes,const vector<Rect>& negtive_boxes, FeatureCollection &features) {
	FeatureCollection feature_positive,feature_negtive;
	feature_positive.clear();
	feature_negtive.clear();	

	CollectFeatureMy(image,positive_boxes,feature_positive);
	for (int i = 0;i< feature_positive.size();i++) {
		feature_positive[i].label = 1;	
	}
	random_shuffle(feature_positive.begin(), feature_positive.end());
	if(feature_positive.size() > parameter.max_positive_features){
		features.insert(features.end(), feature_positive.begin(), feature_positive.begin() + parameter.max_positive_features);
	} else{
		features.insert(features.end(), feature_positive.begin(), feature_positive.end());
	}
	
	CollectFeatureMy(image,negtive_boxes,feature_negtive);
	for (int i = 0;i< feature_negtive.size();i++) {
		feature_negtive[i].label = 0;	
	}
	random_shuffle(feature_negtive.begin(), feature_negtive.end());
	if(feature_negtive.size() > parameter.max_positive_features){
		features.insert(features.end(), feature_negtive.begin(), feature_negtive.begin() + parameter.max_positive_features);
	} else{
		features.insert(features.end(), feature_negtive.begin(), feature_negtive.end());
	}
}*/
