#include "initial_processing.h"

vector<String> images_paths()
{
	String path("Affek_karty//*.png");
	vector<String> pathes;
	glob(path, pathes, false);
	return pathes;
}
vector<img_data> load_source_img_data()
{
	vector<String> pathes = images_paths();
	vector<img_data> table_images(pathes.size());
	for (int i = 0; i < pathes.size(); i++)
	{
		Mat current_image = imread(pathes[i]);
		table_images[i].table_img = current_image;
		string name = pathes[i];
		size_t char_number = name.find_last_of('/');
		name = name.substr(char_number + 1);
		table_images[i].img_name = name;
	}
	return table_images;
}
Mat thresholding(Mat source_img, int min_thresh, int max_thresh)
{
	Mat output_img, gray_img;
	cvtColor(source_img, gray_img, COLOR_BGR2GRAY);
	threshold(gray_img, output_img, min_thresh, max_thresh, THRESH_BINARY | CV_THRESH_OTSU);
	return output_img;
}
void good_perspective(vector<img_data> &cards_images, const Point2f sorted_corners[], int card_num, int image_num)
{
	Point2f output_cards[] = { Point2f(0,0), Point2f(0, 700), Point2f(450, 700), Point2f(450, 0) };
	Mat lambda = getPerspectiveTransform(sorted_corners, output_cards);
	warpPerspective(cards_images[image_num].table_img, cards_images[image_num].card_img[card_num], lambda, Size(450, 700));
}
void cards_corners_sorted(vector<img_data> &cards_images, const vector<vector<Point>> finded_contours, int card_num, int contour_num, int image_num)
{
	vector<Point> approx(4);
	Point2f approx_sorted[4];
	approxPolyDP(finded_contours[contour_num], approx, 40, true);
	int point_index = 0;
	for (int i = 0; i < 3; i++)
		if (approx[i + 1].y + approx[i + 1].x < approx[point_index].y + approx[point_index].x)
			point_index = i + 1;
	for (int i = 0; i < 4; i++)
		approx_sorted[i] = approx[(point_index + i) % 4];
	good_perspective(cards_images, approx_sorted, card_num, image_num);
}
vector<single_cards> single_cards_rewrite(const vector<img_data> src_images)
{
	int table_img_number = src_images.size();
	vector<single_cards> detected_cards(table_img_number);
	for (int i = 0; i < table_img_number; i++)
	{
		detected_cards[i].table_img_name = src_images[i].img_name;
		for (int j = 0; j < 4; j++)
			detected_cards[i].card[j] = src_images[i].card_img[j];
	}
	return detected_cards;
}
vector<single_cards> initial_processing()
{
	vector<img_data> table_images_data = load_source_img_data();
	for (int k = 0; k < table_images_data.size(); k++)
	{
		vector<vector<Point> > all_img_contours;
		vector<Vec4i> contours_hierarchy;
		Mat thresholded = thresholding(table_images_data[k].table_img, 0, 255);
		findContours(thresholded, all_img_contours, contours_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		int iterator = 0;
		for (int i = 0; i < all_img_contours.size(); i++)
		{
			double area = contourArea(all_img_contours[i]);
			if (area > 20000)
			{
				cards_corners_sorted(table_images_data, all_img_contours, iterator, i, k);
				iterator++;
			}
		}
	}
	vector<single_cards> detected_cards = single_cards_rewrite(table_images_data);
	vector<img_data>().swap(table_images_data);											 // free memory
	return detected_cards;
}

