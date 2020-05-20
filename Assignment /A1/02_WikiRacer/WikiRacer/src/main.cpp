#include <iostream>     // for cout, cin
#include <fstream>      // for ifstream
#include <sstream>      // for stringstream
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include "wikiscraper.h"

using std::cout;            using std::endl;
using std::ifstream;        using std::stringstream;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::cin;

/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    // TODO: 1. Once your file reading is working, replace the below
    //          return statement with "return {};".
    //       2. Make sure that you've copied over your findWikiLinks method
    //          from Part A into wikiscraper.cpp.
    //       3. Finally, implement this function per Part B in the handout!
    //
    //                Best of luck!
    WikiScraper scraper;
    auto target_set = scraper.getLinkSet(end_page);
    std::copy(target_set.begin(),target_set.end(),std::ostream_iterator<string>(cout,"\n"));
    auto cmpFn = [&target_set](const vector<string>& laddar1, const vector<string>& laddar2) {
        string curWebsite1 = laddar1.back();
        string curWebsite2 = laddar2.back();
        WikiScraper scraper;
        auto linkset1 = scraper.getLinkSet(curWebsite1);
        auto linkset2 = scraper.getLinkSet(curWebsite2);
        int count1 = 0;
        int count2 = 0;
        for (auto itr = linkset1.begin(); itr != linkset1.end(); itr++) {
            if (target_set.count(*itr)==1) count1++;
        }
        for (auto itr = linkset2.begin(); itr != linkset2.end(); itr++) {
            if (target_set.count(*itr)==1) count2++;
        }
        return count1<count2;
    };
    std::priority_queue<vector<string>, vector<vector<string>>, decltype(cmpFn)> laddarQueue(cmpFn);
    vector<string> start = {start_page};
    laddarQueue.push(start);

    while(laddarQueue.size() != 0) {
        vector<string> partialLaddar = laddarQueue.top();
        laddarQueue.pop();
        std::copy(partialLaddar.begin(),partialLaddar.end(),std::ostream_iterator<string>(cout,"\n"));//how does this impliment?
        string curWeb = partialLaddar.back();
        auto curSet = scraper.getLinkSet(curWeb);//can I use scraper stright?
        if (curSet.count(end_page) == 1) {
            partialLaddar.push_back(end_page);
            return partialLaddar;
        }
        for (auto itr = curSet.begin(); itr != curSet.end(); itr++) {
            if (find(partialLaddar.begin(),partialLaddar.end(),*itr) == partialLaddar.end()) continue;
            vector<string> newLaddar = partialLaddar;
            newLaddar.push_back(*itr);
            laddarQueue.push(newLaddar);
        }

    }

    return {};
}

int main() {
    /* Container to store the found ladders in */
    vector<vector<string>> outputLadders;

    cout << "Enter a file name: ";
    string filename;
    getline(cin, filename);

    // TODO: Create a filestream from the filename.
    //       For each pair {start_page, end_page} in the input file,
    //       retrieve the result of findWikiLadder(start_page, end_page)
    //       and append that vector to outputLadders.

    // Write code here
    ifstream input;
    input.open(filename.c_str());
    while (!input) {
        cout<< "File not found, please input a valid file name."<< endl;
        cout << "Enter a file name: ";
        getline(cin, filename);
        input.open(filename.c_str());
    }
    string start_page;
    string end_page;
    string number;
    getline(input, number);
    int n = std::stoi(number);//get the number of pairs
    for (int i = 0; i < n; i++) {
        getline(input, start_page,' ');
        getline(input, end_page);
        vector<string> laddars = findWikiLadder(start_page,end_page);
        outputLadders.push_back(laddars);
    }

    /*
     * Print out all ladders in outputLadders.
     * We've already implemented this for you!
     */
    for (auto& ladder : outputLadders) {
        if(ladder.empty()) {
            cout << "No ladder found!" << endl;
        } else {
            cout << "Ladder found:" << endl;
            cout << "\t" << "{";

            std::copy(ladder.begin(), ladder.end() - 1,
                      std::ostream_iterator<string>(cout, ", "));
            /*
             * The above is an alternate way to print to cout using the
             * STL algorithms library and iterators. This is equivalent to:
             *    for (size_t i = 0; i < ladder.size() - 1; ++i) {
             *        cout << ladder[i] << ", ";
             *    }
             */
            cout << ladder.back() << "}" << endl;
        }
    }
    return 0;
}




