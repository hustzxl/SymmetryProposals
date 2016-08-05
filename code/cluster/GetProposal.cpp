/*************************************************************************
	> File Name: GetProposal.cpp
	> Author: hustzxl
	> Mail: hustzxl@126.com 
	> Created Time: 2016年08月01日 星期一 09时46分36秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include <time.h>       /* time */
#include<highgui/highgui.hpp>
#include<features2d/features2d.hpp>
#include<imgproc/imgproc.hpp>
#include<core/core.hpp>
#include"FsHelpers.h"
#include"region.h"
#include"agglomerative_clustering.h"
#include"utils.h"
#include"def.h"
#include "ICDAR2011DataSet.h"
#include "TextlineDetector.h"

using namespace std;
using namespace cv;

bool conf_channels[4]={CHANNEL_R,CHANNEL_G,CHANNEL_B,CHANNEL_I};
bool conf_cues[5]={CUE_D,CUE_FGI,CUE_BGI,CUE_G,CUE_S};

static string Phase[2] = {"train", "test"};
static string Image[2] = {"../ICDAR2013","../SVT"};

int getProposals( const string& ImagePath,const int delta, vector<Rect>& outputProposal,vector< vector<float> >& proposal_feature) {
	//const string ResultPath = "../ICDAR2013/Result";
    
	//CmFile::MkDir(ResultPath + "/test/");
    //string ImageName = CmFile::GetFileNameWithoutExtension(ImagePath);
    //string full_file_path = ResultPath + "/test/" + fileName + ".txt"; 
    //std::cout<<full_file_path<<std::endl;

    Mat src, img, grey, lab_img, gradient_magnitude;
    // Pipeline configuration
    // initialize random seed: 
	srand (time(NULL));
	img = imread(ImagePath.c_str());
	if (img.empty()) {
		std::cout<<"image is empty "<<ImagePath<<endl;
		exit(1);
	}
    img.copyTo(src);
	int img_area = img.cols*img.rows;
    cv::MSER cv_mser(delta,(int)(0.00002*img_area),(int)(0.11*img_area),55,0.);
	
	cvtColor(img, grey, CV_BGR2GRAY);
    cvtColor(img, lab_img, CV_BGR2Lab);
    gradient_magnitude = Mat_<double>(img.size());
    get_gradient_magnitude( grey, gradient_magnitude);
	
	vector<Mat> channels;
    split(img, channels);
    channels.push_back(grey);
    int num_channels = channels.size();
	if (PYRAMIDS) {
      for (int c=0; c<num_channels; c++) {
        Mat pyr;
        resize(channels[c],pyr,Size(channels[c].cols/2,channels[c].rows/2));
        //resize(pyr,pyr,Size(channels[c].cols,channels[c].rows));
        channels.push_back(pyr);
      }
      for (int c = 0;c < num_channels;c ++) {
        Mat pyr;
        resize(channels[c],pyr,Size(channels[c].cols/4,channels[c].rows/4));
        //resize(pyr,pyr,Size(channels[c].cols,channels[c].rows));
        channels.push_back(pyr);
      }
    }
    
    //ofstream file3(full_file_path.c_str());
    for (int c=0; c<channels.size(); c++)
    {

		if (!conf_channels[c%4]) continue;

        if (channels[c].size() != grey.size()) // update sizes for smaller pyramid lvls
        {
          resize(grey,grey,Size(channels[c].cols,channels[c].rows));
          resize(lab_img,lab_img,Size(channels[c].cols,channels[c].rows));
          resize(gradient_magnitude,gradient_magnitude,Size(channels[c].cols,channels[c].rows));
        }

        // Initial over-segmentation using MSER algorithm /
        vector<vector<Point> > contours;
        double t = (double)getTickCount();
        cv_mser(channels[c], contours);
        cout << " OpenCV MSER found " << contours.size() << " regions in " << ((double)getTickCount() - t)*1000/getTickFrequency() << " ms." << endl;
   
        // Extract simple features for each region // 
        vector<Region> regions;
        Mat mask = Mat::zeros(grey.size(), CV_8UC1);
        double max_stroke = 0;
        for (int i=contours.size()-1; i>=0; i--)
        {
            Region region;
            region.pixels_.push_back(Point(0,0)); //cannot swap an empty vector
            region.pixels_.swap(contours[i]);
            region.extract_features(lab_img, grey, gradient_magnitude, mask, conf_cues);
            max_stroke = max(max_stroke, region.stroke_mean_);
            regions.push_back(region);
        }

        // Single Linkage Clustering for each individual cue //
        
        for (int cue=0; cue<5; cue++)
        {

			if (!conf_cues[cue]) continue;
    
			int f=0;
			unsigned int N = regions.size();
			if (N<3) continue;
			int dim = 3;
			t_float *data = (t_float*)malloc(dim*N * sizeof(t_float));
			int count = 0;
			for (int i=0; i<regions.size(); i++) {
				data[count] = (t_float)(regions.at(i).bbox_.x+regions.at(i).bbox_.width/2)/img.cols*0.25;
				data[count+1] = (t_float)(regions.at(i).bbox_.y+regions.at(i).bbox_.height/2)/img.rows;
				switch(cue) {
					case 0:
						data[count+2] = (t_float)max(regions.at(i).bbox_.height, regions.at(i).bbox_.width)/max(img.rows,img.cols);
						break;
					case 1:
						data[count+2] = (t_float)regions.at(i).intensity_mean_/255;
						break;
					case 2:
						data[count+2] = (t_float)regions.at(i).boundary_intensity_mean_/255;
						break;
					case 3:
						data[count+2] = (t_float)regions.at(i).gradient_mean_/255;
						break;
					case 4:
						data[count+2] = (t_float)regions.at(i).stroke_mean_/max_stroke;
						break;
				}
				count = count+dim;
			}
			HierarchicalClustering h_clustering(regions);
			vector<HCluster> dendrogram;
			h_clustering(data, N, dim, (unsigned char)0, (unsigned char)3, dendrogram);
            //std:;cout<<"dendrogram size: "<<dendrogram.size()<<std::endl;
            
			for (int k=0; k<dendrogram.size(); k++) {
				int ml = 1;
				if (c>=num_channels) ml=2;// update sizes for smaller pyramid lvls
				if (c>=2*num_channels) ml=4;// update sizes for smaller pyramid lvls
				outputProposal.push_back(Rect(dendrogram[k].rect.x*ml,dendrogram[k].rect.y*ml,dendrogram[k].rect.width*ml,dendrogram[k].rect.height*ml));
			    proposal_feature.push_back(dendrogram[k].feature);	
    //            cout<< dendrogram[k].rect.x*ml << " " << dendrogram[k].rect.y*ml << " " 
	//				<< dendrogram[k].rect.width*ml << " " << dendrogram[k].rect.height*ml << " " 
	//				<< (float)dendrogram[k].probability << endl; 
	//				<< (float)dendrogram[k].nfa << endl 
	// 				<< (float)(k) * ((float)rand()/RAND_MAX) << endl 
	// 				<< (float)dendrogram[k].nfa * ((float)rand()/RAND_MAX) << endl;
			}
			free(data);
		}
        
	}
    return outputProposal.size();
}

