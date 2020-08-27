// string_matching.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <stdio.h>

static std::vector<size_t> longest_prefix_that_is_also_suffix(const std::string& s)
{
	// Definition: For all i in range(0, s.size()), p = lps[i] is the largest p <= i such that s[0:p] == s[(i+1-p):(i+1)]
	std::vector<size_t> lps;
	lps.resize(s.size(), 0);
	size_t n0 = 0;
	while (n0 < s.size() && s[n0] == s[0]) {
		lps[n0] = n0;
		++n0;
	}
	size_t c0 = 0;
	for (size_t i = n0; i < s.size(); ++i) {
		if (s[i] == s[0])
			++c0; // c0 indicates how many repetitions of the first character exist in the tail up to and including s[i]
		else
			c0 = 0;
		size_t prev = lps[i - 1];
		// s[0:prev] = s[(i-prev):i]
		if (s[prev] == s[i])
			lps[i] = prev + 1;
		else
			lps[i] = (c0 > n0) ? n0 : c0;
	}
	return std::move(lps);
}

static int first_string_match(const std::string& txt, const std::string& substring)
{
	if (substring.size() > txt.size())
		return -1;
	auto lps = longest_prefix_that_is_also_suffix(substring);

	size_t i = 0, j = 0;
	while (true) {
		// Try to increase j until txt[i+j] != substring[j]
		while (j < substring.size() && (i + j) < txt.size() && txt[i + j] == substring[j])
			++j;
		if (j == substring.size())
			return i;
		if (i + j == txt.size())
			return -1;
		if (j == 0)
			++i;
		else {
			// j is the maximal such that txt[i:(i+j)] == substring[0:j]
			// so txt[i+j] != substring[j]
			size_t p = lps[j - 1]; // p <= j-1
			// We know that substring[0:p] == substring[(j-p) : j]
			// Therefore the last p characters in txt[i:(i+j)] are substring[0:p]
			// In other words, txt[(i+j-p):(i+j)] == substring[0:p]
			i += j - p;
			j = p;
		}
	}
}

static void verify_match(const std::string& txt, const std::string& substring)
{
	const char* s = strstr(txt.c_str(), substring.c_str());
	int res = first_string_match(txt, substring);
	if (s)
		assert(s == txt.c_str() + res);
	else
		assert(res == -1);
}

static void verify_all_positive_matches(const char* txt)
{
	size_t n = strlen(txt);
	for(size_t i=0;i<n;++i)
		for (size_t j = i + 1; j < n; ++j)
			verify_match(std::string(txt), std::string(txt + i, txt + j));
}

static void verify_negative_matches(const char* txt)
{
	verify_match(std::string(txt), std::string(txt) + std::string("-"));
	verify_match(std::string(txt), "6y2o8uhjs");
}

static void verify_matches(const char* txt)
{
	verify_all_positive_matches(txt);
	verify_negative_matches(txt);
}

int main()
{
	verify_matches("adi levin");
	verify_matches("aabaacaabaa");
	verify_matches("aaaaaa");
	verify_matches("aaaaxa");
	verify_matches("xaaaax");

	std::cout << "Succeeded\n";
}

