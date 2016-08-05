/*************************************************************************
	> File Name: ./include/SvmTrain.h
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 20时15分46秒
 ************************************************************************/

#include<iostream>
#include<vector>
#include<core/core.hpp>
#include"ICDAR2011DataSet.h"
#include"CommonDataStructure.h"
int SvmTrain( const ICDAR2011DataSet& dataset, FeatureCollection& features ) ;
int SvmTest( const ICDAR2011DataSet& dataset, FeatureCollection& features, const std::vector<int>& , const vector< vector<cv::Rect> >&) ;
