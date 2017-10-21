
#ifndef hu_lib_h__
#define hu_lib_h__

//#include <string.h>

class HuLib
{
public:
    static bool get_hu_info(char* const hand_cards, int gui_num);
private:
	static bool split(char* const cards, int gui_num);
	static int _split(char* const cards, int gui_num, int min, int max, bool chi, int& eye_num);
};

#endif // hu_lib_h__
