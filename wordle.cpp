#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif
#include <vector>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void buildWords(
  const std::string& in,
  std::string& current,
  int need[26],
  int& needCount,
  const std::set<std::string>& dict,
  std::set<std::string>& result,
  const std::vector<int>& dashCountRem, int pos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;
    // frequency for the floating letters
    int need[26] = {0};
    for (char c : floating) {
      if (c >= 'a' && c <= 'z') {
        need[c - 'a']++;
      }
    }
    int needCount = floating.size();
    int n = in.size();
    std::vector<int> dashCountRem(n);
    int count = 0;
    for (int i = n - 1; i >= 0; i--) {
      if (in[i] == '-') {
        count++;
      }
      dashCountRem[i] = count;
    }
    
    std::string current = in;
    buildWords (in, current, need, needCount, dict, results, dashCountRem, 0);
    return results;
}

// Define any helper functions 
/**
in: fixed letters and -
current: word being built
need: remaining counts of floating letters (needs to be in word)
needCount: how many floating unused
pos index of word being filled
**/
void buildWords(const std::string& in, std::string& current, int need[26], int& needCount, const std::set<std::string>& dict, std::set<std::string>& results, const std::vector<int>& dashCountRem, int pos) {
  int n = in.size();
  // check if all positions have been filled and valid word
  if (pos == n) {
    if (needCount == 0 && dict.find(current) != dict.end()) {
      results.insert(current);
    }
    return;
  }
  // check if enough spots to fill left floating
  int remaining = n - pos;
  if (needCount > remaining) {
    return;
  }

  int remainingDashes = dashCountRem[pos];
  bool only_float = (needCount == remainingDashes);
  // fixed letter
  if (in[pos] != '-') {
    char c = in[pos];
    current[pos] = c;

    bool used_float = false;
    // floating or no
    if (c >= 'a' && c <= 'z' && need[c - 'a'] > 0) {
      need[c - 'a']--;
      needCount--;
      used_float = true;
    }
    // next position
    buildWords(in, current, need, needCount, dict, results, dashCountRem, pos + 1);

    //if we used a floating, restore the count
    if (used_float) {
      need[c - 'a']++;
      needCount++;
    }
    return;
    }
    for (char c = 'a'; c <= 'z'; c++) {
      current[pos] = c;
      bool used_float = false;
      // mark as floating
      if (only_float && need[c - 'a'] == 0) {
        continue;
      }
      if (need[c - 'a'] > 0) {
        need[c - 'a']--;
        needCount--;
        used_float = true;
      }
      int remaining = n - pos - 1;
      // only if there are enough spaces to fill rest of floating
      if (needCount <= remaining) {
        buildWords(in, current, need, needCount, dict, results, dashCountRem, pos + 1);
      }
      // if we used a floating, restore count
      if (used_float) {
        need[c - 'a']++;
        needCount++;
      }
    }
}