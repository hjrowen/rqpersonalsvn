#include <vector>
#include <queue>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <stdlib.h>


using namespace std;
class Aho_Corasick
{
public:
    Aho_Corasick() {
        clear();
    }

    void clear();

    int word_count() const {
        return _word_count;
    }

    void insert(const char* str, int id); 
    
	void build_automata(); 
	

    std::set<int> get_match(const char* str) ;

private:
    static const int None = -1;
    static const int Root = 0;

    struct Node {
        int id;
        int fail_off;
        std::map<char, int> child;  // charactor -> offset
    };

    int _word_count;
    std::vector<Node> _node;
};

