/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::feof;

string remove_punct(const string& str)
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(ret),
                        [](int c) { return std::ispunct(c); });
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    // initialize all member variables
    init_file_word_maps(filenames);
    init_common();
}

void CommonWords::init_file_word_maps(const vector<string>& filenames)
{
    // make the length of file_word_maps the same as the length of filenames
    file_word_maps.resize(filenames.size());

    // go through all files
    for (size_t i = 0; i < filenames.size(); i++) {
        // get the corresponding vector of words that represents the current file
        vector<string> words = file_to_vector(filenames[i]);

        // go through all the words in the file
        map<string, unsigned int> mp;
        for (string s : words) {
            map<string, unsigned int>::iterator lookup = mp.find(s);
            if (lookup != mp.end()) lookup->second +=1;
            else mp[s] =1;
        }
        file_word_maps[i] = mp;
    }
}

void CommonWords::init_common()
{
    for (map<string, unsigned int> mp : file_word_maps) {
        for (std::pair<string, unsigned int> kv : mp) {
            common[kv.first]++;
        }
    }

}

/**
 * @param n The number of times to word has to appear.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector<string> CommonWords::get_common_words(unsigned int n) const
{
    vector<string> out;
    /* Your code goes here! */
    unsigned int size = file_word_maps.size();

    for (std::pair<string, unsigned int> kv : common) {
        if (kv.second == size) {
            bool meetsC = true;
            string s = kv.first;

            for (unsigned int i = 0; i < size && meetsC; i++) {
                if (file_word_maps[i].at(s) < n) meetsC = false;
        
            }
            if (meetsC) out.push_back(s);

        
        
        }

    }

    return out;
}

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector<string> CommonWords::file_to_vector(const string& filename) const
{
    ifstream words(filename);
    vector<string> out;

    if (words.is_open()) {
        std::istream_iterator<string> word_iter(words);
        while (!words.eof()) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}
