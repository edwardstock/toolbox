/**
 * cpphelpers
 * main.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include <iostream>
#include <vector>
#include <regex>
#include "include/cpphelpers.h"

using namespace std;
using namespace std::chrono;

map<string, milliseconds> profile;

string lastTag = "";

void _begin(const std::string &tag) {
	if (profile.count(tag) > 0) return;

	lastTag = tag;
	profile[lastTag] = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

void _end(const std::string &tag) {
	if (profile.count(tag) == 0) return;

	profile[tag] = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - profile[tag];
}

void _end() {
	if (profile.count(lastTag) == 0) return;

	profile[lastTag] = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - profile[lastTag];
}

std::regex exp(R"(([a-z0-9_\-\+\.]+)?(?:\[\])?\=(.*))");

void printProfile() {
	deque<string> results;
	for (auto &p: profile) {
		stringstream ss;
		ss << p.first << ": " << p.second.count() << "ms";
		results.push_front(ss.str());
	}

	for (auto &p: results) {
		cout << p << endl;
	}

	results.clear();
	profile.clear();
}

void readWhole() {
	_begin("read file");
	ifstream in("npef.ini");
	_end();

	_begin("read whole file");
	string inFile = Strings::toString(in);
	_end();

	_begin("match whole file");
	vector<vector<string>> allMatches = Strings::matchAllRegexp(exp, inFile);
	_end();

	cout << "Count all groups: " << allMatches.size() << endl;

	_begin("match line");
	int i = 0;
	vector<string> withArrs;
	for (auto &sub: allMatches) {
		string whole = sub[0];
		if (Strings::hasSubstring(whole, '[')) {
			i++;
			withArrs.push_back(whole);
		}
	}
	_end();
	in.close();
	inFile.clear();
}

void readByLine() {
	_begin("read file");
	ifstream in("npef.ini");
	_end();

	string row;
	int line = 1;
	int grps = 0;
	int arrCnt = 0;
	_begin("match by line");

//	cout << endl;
	while (getline(in, row)) {
		if (row == ";" || row == "#" || row.empty()) {
			line++;
			continue;
		}

		smatch found = Strings::matchRegexp(exp, row);
		if (found.empty()) {
			line++;
			continue;
		};

		bool isArr = Strings::hasSubstring(found[0], '[');
		if (isArr) {
			arrCnt++;
		}
		grps++;
//		cout << "\rLine: " << line;

		line++;
	}
//	cout << endl;
	_end();

	cout << "Count all groups: " << grps << endl;
	in.close();
}

int main(int argc, char **argv) {

	cout << "Whole file match" << endl;
	_begin("WHOLE SUMMARY");
	readWhole();
	_end("WHOLE SUMMARY");
	printProfile();
	cout << endl;

	cout << "By line file match" << endl;
	_begin("BY LINE SUMMARY");
	readByLine();
	_end("BY LINE SUMMARY");
	printProfile();
	cout << endl;

	return 0;
}
