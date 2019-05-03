
#ifndef U_wn_db_h
#define U_wn_db_h

#include <string>
#include <vector>

// #include "../../../src/global_conf.h"

// #ifdef WN_BUILD
// 	#define WN_API ULTRA_EXPORT
// #else
// 	#define WN_API ULTRA_IMPORT
// #endif

class wn_db {
public:
	struct rec_t
	{
		int level;
		std::string word;
	};

	wn_db(const std::string & base_path);
	~wn_db();
	void search(const std::string & word, int max_depth, std::vector<rec_t> & syns, int max_word_count = 100);

	bool opened;
};

#endif
