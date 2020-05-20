#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cctype>
#include <string>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;

/*
 * A helper function to findWikiLinks
 * The function takes in the html texts in the
 * page as a single string and a unordered set of
 * string containing all the links find in the page.
 */
void makeLinksSet(string page_html, unordered_set<string>& webLinks) {
    //base case: no weblink found in the rest of the text
    string prefix = "<a href=\"/wiki/";
    string suffix = "\"";
    string::iterator link_begin = std::search(page_html.begin(),page_html.end(),prefix.begin(),prefix.end());
    if (link_begin==page_html.end()) return;
    string::iterator link_end = std::search(link_begin+15,page_html.end(),suffix.begin(),suffix.end());
    if (link_end==page_html.end()) return;
    //recursive case: weblink found, proceed with the rest of the text
    string linkName = string(link_begin+15,link_end);
    string::iterator findChar1 = std::find(linkName.begin(),linkName.end(),'#');//check if the name contains #
    string::iterator findChar2 = std::find(linkName.begin(),linkName.end(),':');//check if the name contains :
    if (findChar1 == linkName.end() && findChar2 == linkName.end()) {//if it doesn't
        webLinks.insert(linkName);//add the link name to the unordered set.
    }
    page_html = string(link_end+2, page_html.end());
    return makeLinksSet(page_html, webLinks);
}

/*
 * Note that you have to pass in the file as a single string
 * into the findWikiLinks function!
 * So we have to turn the passed-in file into a single string...
 * does that sound familiar at all?
 */
unordered_set<string> findWikiLinks(const string& page_html) {
    // TODO: delete this return statement and implement the
    //       function!
    unordered_set<string> webLinks;
    makeLinksSet(page_html, webLinks);
    return webLinks;
}

int main() {
    /* Note if your file reading isn't working, please go to the
     * projects tab on the panel on the left, and in the run section,
     * uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */

    cout << "Enter a file name: ";
    string filename;
    getline(std::cin, filename);
    string page_html;

    // TODO: Create a filestream from the filename, and convert it into
    //       a single string of data called page_html (declared above)

    // Write code here
    std::ifstream input(filename);
    while (!input) {
        cout<<"File cannot be found, please input a valid filename."<< endl;
        cout << "Enter a file name: ";
        getline(std::cin, filename);
        input.open(filename);
    }
    string temp;
    while(getline(input,temp)) {
        page_html += temp;
    }


    unordered_set<string> validLinks = findWikiLinks(page_html);

    // TODO: Print out the validLinks set above.
    //       You can print it out in any format; this is just for you
    //       to be able to compare your results with the [output] results
    //       under the /res folder.

    // Write code here
    for (unordered_set<string>::iterator itr = validLinks.begin(); itr != validLinks.end(); ++itr) {
        string linkName = *itr;
        cout << linkName << endl;
    }

    return 0;
}
