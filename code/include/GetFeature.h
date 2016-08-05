/*************************************************************************
	> File Name: GetFeature.h
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 16时02分52秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<core/core.hpp>
#include "TextlineDetector.h"

int getFeature( const std::string& , const std::vector<cv::Rect>&, const std::vector<cv::Rect> &, const std::vector< std::vector<float> >&, FeatureCollection& feature_tmp );
