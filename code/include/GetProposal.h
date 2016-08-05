/*************************************************************************
	> File Name: GetProposals.h
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 10时40分54秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<core/core.hpp>

int getProposals( const std::string& ImagePath,const int delta, std::vector<cv::Rect>& outputProposal,std::vector< std::vector<float> >& proposal_feature) ;
