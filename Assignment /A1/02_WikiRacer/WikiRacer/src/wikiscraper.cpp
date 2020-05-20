#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "wikiscraper.h"
#include "error.h"

using std::cout;            using std::endl;
using std::cerr;            using std::string;
using std::unordered_map;   using std::unordered_set;

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
 * You should delete the code in this function and
 * fill it in with your code from part A of the assignment.
 *
 * If you used any helper functions, just put them above this function.
 */
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


/*
 * ==================================================================================
 * |                           DON"T EDIT ANYTHING BELOW HERE                       |
 * ==================================================================================
 */
unordered_set<string> WikiScraper::getLinkSet(const string& page_name) {
    if(linkset_cache.find(page_name) == linkset_cache.end()) {
        auto links = findWikiLinks(getPageSource(page_name));
        linkset_cache[page_name] = links;
    }
    return linkset_cache[page_name];
}


WikiScraper::WikiScraper() {
    (void)getPageSource("Main_Page");
#ifdef _WIN32
    // define something for Windows
    system("cls");
#else
    // define it for a Unix machine
    system("clear");
#endif
}


string createPageUrl(const string& page_name) {
    return "https://en.wikipedia.org/wiki/" + page_name;
}

void notFoundError(const string& msg, const string& page_name, const string& url) {
    const string title = "    AN ERROR OCCURED DURING EXECUTION.    ";
    const string border(title.size() + 4, '*');
    cerr << endl;
    errorPrint(border);
    errorPrint("* " + title + " *");
    errorPrint(border);
    errorPrint();
    errorPrint("Reason: " + msg);
    errorPrint();
    errorPrint("Debug Information:");
    errorPrint();
    errorPrint("\t- Input parameter: " + page_name);
    errorPrint("\t- Attempted url: " + url);
    errorPrint();
}

string WikiScraper::getPageSource(const string &page_name) {
    const static string not_found = "Wikipedia does not have an article with this exact name.";
    if(page_cache.find(page_name) == page_cache.end()) {
        QUrl url(createPageUrl(page_name).c_str()); // need c string to convert to QString

        QNetworkRequest request(url);

        QNetworkReply *reply(manager.get(request));

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()) , &loop, SLOT(quit()));
        loop.exec();

        string ret = reply->readAll().toStdString();
        if(std::search(ret.begin(), ret.end(), not_found.begin(), not_found.end()) != ret.end()){
            notFoundError("Page does not exist!", page_name, url.toString().toStdString());
            return "";
        }
        size_t indx = ret.find("plainlinks hlist navbar mini");
        if(indx != string::npos) {
            return ret.substr(0, indx);
        }
        page_cache[page_name] = ret;
    }
    return page_cache[page_name];
}





