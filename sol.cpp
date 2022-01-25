#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;
#define ll long long

// change these based on the input files
const ll NUMBER_OF_FILES = 10;
const string FILE_NAMES [NUMBER_OF_FILES] = {"sample.1", "sample.2", "sample.3", "sample.4", "sample.5", "sample.6",
                                             "sample.7", "sample.8", "sample.9", "sample.10"};

pair<string, vector<char>> files [NUMBER_OF_FILES];

bool cmp(pair<string, vector<char>>& a, pair<string, vector<char>>& b) {
    return a.second.size()>b.second.size();
}

int main() {
    // I am going to assume in my solution that only ONE unique longest strand occurs between some number of files
    // reading files:
    for (ll i=0; i<NUMBER_OF_FILES; i++) {
        ifstream file(FILE_NAMES[i], ios::binary);

        ostringstream ss;
        ss << file.rdbuf();
        const string &s = ss.str();

        files[i] = make_pair(FILE_NAMES[i], vector<char>(s.begin(), s.end()));
        file.close();
    }

    // I sort the files from largest to smallest size, that way once a largest match is bigger than the next file size we can stop.
    sort(files, files+NUMBER_OF_FILES, cmp);
    ll MAX_SIZE_OF_FILE = files[0].second.size(), max=0;
    unordered_map<string, ll> ans_files{}; // mapping filename to offset for final answer
    for (ll i=1; i<NUMBER_OF_FILES; i++) {
        ll n=files[i].second.size();
        if (max>n) break;
        for (ll j=0; j<i; j++) {
            if (max==n && ans_files.count(files[i].first)>0) break; // if max match is the entire file i, then we can quit checking file i with other files
            ll m=files[j].second.size();
            // originally used dp array, however it used too much memory given the file sizes, and most entries were 0 anyway
            // thus we can instead use a map, and only keep track of nonzero entries
            unordered_map<ll, ll> map{}; // dp map
            ll best=0, offset1=0, offset2=0;
            // using dp here to find the best length common substring between 2 files
            // keeping track of the offsets for the final answer
            for (ll a=1; a<=n; a++) {
                for (ll b=1; b<=m; b++) {
                    if (files[i].second[a-1]==files[j].second[b-1]) {
                        ll cur = a*MAX_SIZE_OF_FILE + b, prev = (a-1)*MAX_SIZE_OF_FILE + b-1;
                        if (map.count(prev)==1)
                            map[cur] = map[prev]+1;
                        else
                            map[cur]=1;
                        if (map[cur] > best) {
                            best=map[cur];
                            offset1=a;
                            offset2=b;
                        }
                    }
                }
            }

            if (best > max) {
                max=best;
                ans_files.clear();
                ans_files[files[i].first] = offset1-max;
                ans_files[files[j].first] = offset2-max;
            } else if (best == max) {
                // adding the new answer file found
                if (ans_files.count(files[i].first)==0) ans_files[files[i].first] = offset1-max;
                if (ans_files.count(files[j].first)==0) ans_files[files[j].first] = offset2-max;
            } else {
                // if max match is size of file i, and it doesn't match with a previous match; file i is suboptimal, and we can stop
                if (max==n && ans_files.count(files[j].first)>0) break;
            }
        }
    }

    // output final answer
    cout << "length of longest strand: " << max << endl;
    for (const auto& kv: ans_files) {
        cout << kv.first << ", offset: " << kv.second << endl;
    }
}

