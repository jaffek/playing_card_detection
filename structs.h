#ifndef STRUCTS_H
#define STRUCTS_H

#include <opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

struct img_data					
{
	String img_name;			// name of image of 4 cards created from path string
	Mat table_img;				// image of 4 cards
	Mat card_img[4];			// single cards images (cut from table_img)
};
struct single_cards				
{
	String table_img_name;	
	string suit[4];				// name of card symbols (4 in every table_img)
	int value[4];				// number of symbols on card
	Mat card[4];				// every single card in one Mat
	Mat thresholded_card[4];	// every single thresholded card in one Mat
	float harmonic_mean;		// calculated harmonic mean of every table card image 
};

#endif