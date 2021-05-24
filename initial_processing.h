#ifndef INITIAL_PROCESSING_H
#define  INITIAL_PROCESSING_H
#include "structs.h"

// returns a vector with the paths of all images
vector<String> images_paths();

// load all images in folder and their names to structure
vector<img_data> load_source_img_data();

// thresholding operation using otsu
Mat thresholding(Mat source_img, int min_thresh, int max_thresh);

// make cards rectangular
void good_perspective(vector<img_data> &cards_images, const Point2f sorted_corners[], int card_num, int image_num);

// sorting detected cards corners counterclockwise beginning from left up
void cards_corners_sorted(vector<img_data> &cards_images, const vector<vector<Point>> finded_contours, int card_num, int contour_num, int image_num);

// rewrite from source structure to new structure
vector<single_cards> single_cards_rewrite(const vector<img_data> src_images);

// processing operations in the right order - find contours bigger than x
vector<single_cards> initial_processing();

#endif