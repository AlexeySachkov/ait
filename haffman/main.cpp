#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void print_usage(const string &);

enum alg_t { HUFFMAN, SHANNON };

void huffman_alg(const string &, ostream &);
void shannon_alg(const string &, ostream &);

struct letter
{
	char c;
	int next_id;
	bool binary_code;
	double probability;

	letter(): 
		c(0), 
		next_id(-1), 
		binary_code(false), 
		probability(0) 
	{ };
	
	letter(char _c): 
		c(_c), 
		next_id(-1), 
		binary_code(false), 
		probability(0) 
	{ };

	letter(char _c, double _probability): 
		c(_c), 
		next_id(-1), 
		binary_code(false), 
		probability(_probability) 
	{ };

	letter(double _probability): 
		c(0), 
		next_id(-1), 
		binary_code(false), 
		probability(_probability) 
	{ };

	letter(char _c, int _next_id, bool _binary_code, double _probability): 
		c(_c), 
		next_id(_next_id), 
		binary_code(_binary_code), 
		probability(_probability) 
	{ };
};

int main(int argc, char* argv[])
{
	vector<string> args(argv, argv + argc);

	if (find(args.begin(), args.end(), "-h") != args.end() || find(args.begin(), args.end(), "--help") != args.end())
	{
		print_usage(args[0]);
	}
	else
	{
		string filename_for_input;
		string filename_for_output = "-";
		alg_t alg = HUFFMAN;

		ifstream input_file;
		ofstream output_file_stream;
		streambuf * output_file;

		if (args.size() < 2)
		{
			print_usage(args[0]);
		}
		else
		{
			filename_for_input = args.back();

			input_file.open(filename_for_input);
			if (!input_file.is_open())
			{
				cout << "Cannot open file " << filename_for_input << " for reading input!" << endl;
				return -1;
			}

			vector<string>::iterator it;
			
			it = find(args.begin(), args.end(), "-m");
			if (it == args.end())
			{
				it = find(args.begin(), args.end(), "--method");
			}

			if (it != args.end())
			{
				it = next(it);
				if (it == args.end())
				{
					cout << "Cannot find value for " << *(--it) << " argument!" << endl;
					return -1;
				}
				else
				{
					if (*it == "huffman")
					{
						alg = HUFFMAN;
					}
					else if (*it == "shannon")
					{
						alg = SHANNON;
					}
					else
					{
						cout << "Cannot parse value for " << *(--it) << " argument!" << endl;
						return -1;
					}
				}
			}

			it = find(args.begin(), args.end(), "-o");
			if (it == args.end())
			{
				it = find(args.begin(), args.end(), "--output");
			}

			if (it != args.end())
			{
				it = next(it);
				if (it == args.end())
				{
					cout << "Cannot find value for " << *(--it) << " argument!" << endl;
					return -1;
				}
				else
				{
					filename_for_output = *it;

					if (filename_for_output == "-")
					{
						output_file = cout.rdbuf();
					}

					output_file_stream.open(filename_for_output);
					if (!output_file_stream.is_open())
					{
						cout << "Cannot open file " << filename_for_output << " for print results!" << endl;
						return -1;
					}
					else
					{
						output_file = output_file_stream.rdbuf();
					}
				}
			}

			string input((std::istreambuf_iterator<char>(input_file)),
							std::istreambuf_iterator<char>());

			ostream out(output_file);

			switch (alg)
			{
				case HUFFMAN:
					huffman_alg(input, out);
					break;
				case SHANNON:
					shannon_alg(input, out);
					break;
			}
		}
	}

	return 0;
}

void huffman_alg(const string &input, ostream &out)
{
	map<char, int> letterCounts;
	map<char, double> probability;

	out << "Source text (" << input.size() << " symbols): " << endl << input << endl;

	for (size_t i = 0; i < input.size(); ++i)
	{
		letterCounts[input[i]]++;
	}

	vector<letter> v(letterCounts.size());

	out << "Letters info: " << endl;

	size_t pos = 0;
	for (map<char, int>::iterator it = letterCounts.begin(); it != letterCounts.end(); ++it, ++pos)
	{
		letter l(it->first, (double)it->second / (double)input.size());
		v[pos] = l;
		out << "Symbol '" << l.c << "' presents " << it->second << " times. Probability: " << l.probability << endl;
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

	size_t new_file_size = 0;

	for (size_t i = 0; i < letterCounts.size(); ++i)
	{
		out << "Code for '" << v[i].c << "' symbol is: ";
		letter temp = v[i];
		string code = temp.binary_code ? "1" : "0";
		
		while (temp.next_id >= 0)
		{
			temp = v[temp.next_id];
			code += temp.binary_code ? '1' : '0';
		}

		reverse(code.begin(), code.end());

		new_file_size += code.size() * letterCounts[v[i].c];

		out << code << endl;
	}

	out << endl;

	out << "Previous file size: " << input.size() * 8 << " bits" << endl;
	out << "New file size: " << new_file_size << " bits" << endl;
}

void shannon_alg(const string &input, ostream &out)
{
	out << "Not implemented yet" << endl;
}

void print_usage(const string &executable)
{
	cout << "Usage: " << endl;
	cout << "\t" << executable << "[options] input_file" << endl;
	cout << "Arguments: " << endl;
	cout << "\t input_file" << endl;
	cout << "\t\tFile which will be encoded" << endl;
	cout << "\t --method, -m" << endl;
	cout << "\t\t Available methods: huffman, shannon. Default: huffman " << endl;
	cout << "\t --output, -o" << endl;
	cout << "\t\t File in which results will be printed. Default: stdout " << endl;
	cout << "\t Example: " << endl;
	cout << "\t\t " << executable << " -m shannon -o result.txt text.txt" << endl;
}