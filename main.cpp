#include "initial_processing.h"
#include "final_processing.h"


void display_in_console(const vector<single_cards> detected_cards)
{
	cout << "Number of images: " << detected_cards.size() << endl;
	cout << "Number of cards: " << detected_cards.size() * 4 << endl;
	for (int i = 0; i < detected_cards.size(); i++)
	{
		cout << detected_cards[i].table_img_name << "\t";
		if (detected_cards[i].table_img_name.length() < 9)
			cout << "\t";
		for (int j = 0; j < 4; j++)
		{
			cout << detected_cards[i].value[j] << " " << detected_cards[i].suit[j] << ", ";
		}
		cout << "  Harmonic mean = " << detected_cards[i].harmonic_mean << endl;
	}
}
//********************************************************************************************
int main()
{
	vector<single_cards> detected_cards = initial_processing();
	final_processing(detected_cards);
	harmonic_mean_calculation(detected_cards);
	display_in_console(detected_cards);
	getchar();
	waitKey(0);
	return 0;
}
