#ifndef NORMALIZE_H
#define NORMALIZE_H

#include <string>

class normalize{

	public:
		normalize();
		
		int ShiftNum(int ascii);
    	std::string SpecialKeys(int key);
};

#endif