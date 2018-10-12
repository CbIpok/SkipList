#include <iostream>
#include "skip_list.h"
#include <list>
using namespace std;


int main()
{
	
	auto s =skip_list<int, int>();
	for (size_t i = 0; i < 10; i++)
	{
		s[i] = i;
	}
	auto it = s.begin();
	s.erase(s.begin(),++++s.begin());
	for (const auto& i : s)
	{
		cout << i.second<<endl;
	}
	
	int i = 0;
	return 0;
}