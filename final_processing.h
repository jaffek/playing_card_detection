#ifndef FINAL_PROCESSING_H
#define  FINAL_PROCESSING_H
#include "structs.h"

// opencv filters used to create correct thresholded image of card 
void base_filters(vector<single_cards> &detected_cards, int table_img_num, int card_num);

// calculates the average value of the red channel inside symbol contour
int mean_symbols_color(const Mat single_card, const vector<vector<Point>> contours, int contour_num, vector<Vec4i> hierarchy);

// recognize symbol based on the ratio of the sides of the bounded rectangle and its color
string symbols_recognition(const Mat single_card, const vector<vector<Point>> contours, int contour_num, vector<Vec4i> hierarchy);

// load data in card structure
void saving_card_data(vector<single_cards> &detected_cards, const string card_suit, const int card_value, int table_img_num, int card_number);

// calculate harmonic mean of the tanle image card
void harmonic_mean_calculation(vector<single_cards> &detected_cards);

// processing operations of single detected cards in the right order - find contours bigger than x
void final_processing(vector<single_cards> &detected_cards);

#endif
