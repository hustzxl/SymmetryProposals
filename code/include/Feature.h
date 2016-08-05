/*************************************************************************
	> File Name: ./include/Feature.h
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 16时19分54秒
 ************************************************************************/

#include<iostream>
#include<vector>
#include<core/core.hpp>
#include<imgproc/imgproc.hpp>
#include"ICDAR2011DataSet.h"
#include"CommonDataStructure.h"

int GetFeature( const ICDAR2011DataSet& dataset, const int delta, bool train , FeatureCollection& , vector<int>&, vector< vector<cv::Rect> >&) ;
