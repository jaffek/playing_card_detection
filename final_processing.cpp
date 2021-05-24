#include "final_processing.h"
#include "initial_processing.h"

void base_filters(vector<single_cards> &detected_cards, int table_img_num, int card_num)
{
	medianBlur(detected_cards[table_img_num].card[card_num], detected_cards[table_img_num].card[card_num], 5);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	erode(detected_cards[table_img_num].card[card_num], detected_cards[table_img_num].card[card_num], element);
	detected_cards[table_img_num].thresholded_card[card_num] = thresholding(detected_cards[table_img_num].card[card_num], 150, 255);
	bitwise_not(detected_cards[table_img_num].thresholded_card[card_num], detected_cards[table_img_num].thresholded_card[card_num]);
}
int mean_symbols_color(const Mat single_card, const vector<vector<Point>> contours, int contour_num, vector<Vec4i> hierarchy)
{
	Mat mask = Mat::zeros(701, 451, CV_8U);
	drawContours(mask, contours, contour_num, Scalar(255, 255, 255), -1, 8, hierarchy);
	Scalar card_symbol_color = mean(single_card, mask);
	return card_symbol_color.val[2];
}
string symbols_recognition(const Mat single_card, const vector<vector<Point>> contours, int contour_num, vector<Vec4i> hierarchy)
{
	string suit;
	int card_color = mean_symbols_color(single_card, contours, contour_num, hierarchy);
	Rect symbol = boundingRect(contours[contour_num]);
	if ((float)symbol.width / symbol.height > 0.9 && card_color < 100)
		return "Clover";
	if ((float)symbol.width / symbol.height > 0.8 && card_color > 100)
		return "Heart";
	if ((float)symbol.width / symbol.height <= 0.9 && card_color < 100)
		return "Pike";
	if ((float)symbol.width / symbol.height <= 0.8 && card_color > 100)
		return "Tile";
}
void saving_card_data(vector<single_cards> &detected_cards, const string card_suit, const int card_value, int table_img_num, int card_number)
{
	detected_cards[table_img_num].suit[card_number] = card_suit;
	detected_cards[table_img_num].value[card_number] = card_value;
}
void harmonic_mean_calculation(vector<single_cards> &detected_cards)
{
	for (int i = 0; i < detected_cards.size(); i++)
	{
		float harmonic_mean = 0;
		harmonic_mean = 4.0 / ((1.0 / detected_cards[i].value[0]) + (1.0 / detected_cards[i].value[1]) + (1.0 / detected_cards[i].value[2]) + (1.0 / detected_cards[i].value[3]));
		detected_cards[i].harmonic_mean = harmonic_mean;
	}
}
void final_processing(vector<single_cards> &detected_cards)
{
	for (int k = 0; k < detected_cards.size(); k++)
		for (int i = 0; i < 4; i++)
		{
			base_filters(detected_cards, k, i);
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			int card_value = 0;
			string suit;
			findContours(detected_cards[k].thresholded_card[i], contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			for (int j = 0; j < contours.size(); j++)
			{
				float area = contourArea(contours[j]);
				if (area > 2300)
				{
					card_value++;
					if (card_value != 1) continue;
					suit = symbols_recognition(detected_cards[k].card[i], contours, j, hierarchy);
				}
			}
			saving_card_data(detected_cards, suit, card_value, k, i);
		}
}