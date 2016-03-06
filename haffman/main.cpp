#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct letter
{
	char c;
	int next_id;
	bool binary_code;
	double probability;

	letter(): c(0), next_id(-1), binary_code(false), probability(0) { };
	letter(char _c): c(_c), next_id(-1), binary_code(false), probability(0) { };
	letter(char _c, double _probability): c(_c), next_id(-1), binary_code(false), probability(_probability) { };
	letter(double _probability): c(0), next_id(-1), binary_code(false), probability(_probability) { };
	letter(char _c, int _next_id, bool _binary_code, double _probability): c(_c), next_id(_next_id), binary_code(_binary_code), probability(_probability) { };
};

int main(int argc, char* argv[])
{


	string text = "Hello, World!";
	map<char, int> letterCounts;
	map<char, double> probability;

	cout << "Source text (" << text.size() << " symbols): " << endl << text << endl;

	for (size_t i = 0; i < text.size(); ++i)
	{
		letterCounts[text[i]]++;
	}

	vector<letter> v(letterCounts.size());

	cout << "Letters info: " << endl;

	size_t pos = 0;
	for (map<char, int>::iterator it = letterCounts.begin(); it != letterCounts.end(); ++it, ++pos)
	{
		letter l(it->first, (double)it->second / (double)text.size());
		v[pos] = l;
		cout << "Symbol '" << l.c << "' presents " << it->second << " times. Probability: " << l.probability << endl;
	}

	vector<pair<double, int>> w;
	
	for (size_t i = 0; i < v.size(); ++i)
	{
		w.push_back(make_pair(v[i].probability, i));
	}

	while (w.size() > 1)
	{
		sort(w.rbegin(), w.rend());

		pair<double, int> a = *(--w.end());
		pair<double, int> b = *(--(--w.end()));

		int next_id = v.size();
		
		v[a.second].binary_code = 0;
		v[b.second].binary_code = 1;

		v[a.second].next_id = v[b.second].next_id = next_id;

		v.push_back(letter(v[a.second].probability + v[b.second].probability));

		w.pop_back();
		w.pop_back();
		w.push_back(make_pair(v[a.second].probability + v[b.second].probability, next_id));
	}

	for (size_t i = 0; i < letterCounts.size(); ++i)
	{
		cout << "Code for '" << v[i].c << "' symbol is: ";
		letter temp = v[i];
		string code = temp.binary_code ? "1" : "0";
		
		while (temp.next_id >= 0)
		{
			temp = v[temp.next_id];
			code += temp.binary_code ? '1' : '0';
		}

		reverse(code.begin(), code.end());

		cout << code << endl;
	}

	return 0;
}