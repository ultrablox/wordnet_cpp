
#include "wn_db.h"
#include "wn.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <set>
#include <queue>

extern  "C" char * uo_dir_path;

wn_db::wn_db(const std::string & base_path)
	:opened(false)
{
	std::cout << "opening dicts at " << base_path << std::endl;
	uo_dir_path = (char*)base_path.c_str();
	/*memset(uo_dir_path, 0, 256);
	//memcpy(uo_dir_path, base_path.c_str(), base_path.size());
	sprintf(uo_dir_path, "%s", base_path.c_str());*/
	int r = wninit();
	r = re_wninit();

	this->opened = (r == 0);
}

void wn_db::search(const std::string & word, int max_depth, std::vector<rec_t> & syns, int max_word_count)
{
	IndexPtr idx = getindex((char*)word.c_str(), ADJ);
	if (idx == 0)
		return;

	std::set<std::string> found;

	struct search_node
	{
		int depth;
		SynsetPtr syn;
	};

	std::queue<search_node> search_queue;

	for (int sense = 0; sense < idx->off_cnt; sense++)
	{
		search_node new_node;
		new_node.depth = 0;
		new_node.syn = read_synset(ADJ, idx->offset[sense], idx->wd);
		search_queue.push(new_node);
	}

	while ((!search_queue.empty()) && (found.size() < max_word_count))
	{
		auto cur_node = search_queue.front();
		search_queue.pop();

		for (int i = 0; i < cur_node.syn->wcount; ++i)
		{
			rec_t new_rec;
			new_rec.word = std::string(cur_node.syn->words[i], strlen(cur_node.syn->words[i]));
			new_rec.level = cur_node.depth;

			if (found.find(new_rec.word) == found.end())
			{
				syns.push_back(new_rec);
				found.insert(new_rec.word);
			}
		}

		if (cur_node.depth < max_depth)
		{
			for (int i = 0; i < cur_node.syn->ptrcount; i++)
			{
				if (cur_node.syn->ptrtyp[i] == SIMPTR)
				{
					search_node new_node;
					new_node.depth = cur_node.depth + 1;
					new_node.syn = read_synset(cur_node.syn->ppos[i], cur_node.syn->ptroff[i], "");
					search_queue.push(new_node);
				}
			}
		}

		free_synset(cur_node.syn);
	}

	while (!search_queue.empty())
	{
		free_synset(search_queue.front().syn);
		search_queue.pop();
	}


	std::sort(syns.begin(), syns.end(), [](const rec_t & lhs, const rec_t & rhs){
		return lhs.level < rhs.level;
	});
}

wn_db::~wn_db()
{
	closefps();
}
