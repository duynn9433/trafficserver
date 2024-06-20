#include "url_helper.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

string generateAllQueryParams(map<string, string> queryParams) {
    string allQueryParams = "";
    for (map<string, string>::iterator it = queryParams.begin(); it != queryParams.end(); ++it) {
        allQueryParams += it->first + "=" + it->second + "&";
    }
    //remove the last '&'
    allQueryParams = allQueryParams.substr(0, allQueryParams.size() - 1);

    return allQueryParams;
}

void logInfo(string log) {
    cout << log << endl;
}

void logError(string error) {
    cerr << error << endl;
}

map<string, string> parseQueryParams(string queryParams) {
    map<string, string> queryParamsMap;
    //parse query params
    size_t pos = 0;
    while ((pos = queryParams.find("&")) != string::npos) {
        string queryParam = queryParams.substr(0, pos);
        size_t equalPos = queryParam.find("=");
        if (equalPos != string::npos) {
            string key = queryParam.substr(0, equalPos);
            string value = queryParam.substr(equalPos + 1);
            queryParamsMap[key] = value;
        } else {
            logError("Cannot find '=' in query param: " + queryParam);
        }
        queryParams.erase(0, pos + 1);
    }
    return queryParamsMap;
}

void getAllPath(string url, string &urlToSend, string &pathToHash, string &tokenInfo,
map<string, string> queryParamsMap, map<string, string> &tokenInfoMap)
{
    size_t firstCharOfQueryParams = url.find("?");
    // if (firstCharOfQueryParams != string::npos) {
    //     string allQueryParams = url.substr(firstCharOfQueryParams + 1);
    //     queryParamsMap = parseQueryParams(allQueryParams);
    // } else {
    //     logInfo("not have query params");
    // }
    //get the token info in path
    string tokenKeyWord = "/" + string(SIG_QSTRING);
    logInfo("tokenKeyWord: " + tokenKeyWord);
    size_t foundFirstOfTokenInfo = url.find(tokenKeyWord);
    //find last char of token info
    // 1. /token=123&x=123&y=123/abc/def/ghi...
    // 2. /token=123&x=123&y=123?abc=123...
    // 3. /token=123&x=123&y=123
    size_t foundLastOfTokenInfo = url.find("/", foundFirstOfTokenInfo + 1);
    if (foundLastOfTokenInfo == string::npos) {
        //check if have query params
        if (firstCharOfQueryParams != string::npos) {
            if(firstCharOfQueryParams > foundFirstOfTokenInfo){
                foundLastOfTokenInfo = firstCharOfQueryParams;
            }else {
                logError("Cannot find last of token info in path");
            }
        } else {
            foundLastOfTokenInfo = url.size();
        }
    }

    if (foundFirstOfTokenInfo != string::npos && foundLastOfTokenInfo != string::npos) {
        tokenInfo = url.substr(foundFirstOfTokenInfo + 1, foundLastOfTokenInfo - foundFirstOfTokenInfo - 1);
    } else
    {
        //TODO: print error and return
        logError("Cannot find token info in path");
        return;
    }
    //get url without token info
    string urlWithoutTokenInfo = url.substr(0, foundFirstOfTokenInfo) + url.substr(foundLastOfTokenInfo);
    logInfo("urlWithoutTokenInfo: " + urlWithoutTokenInfo);
    //get url to send copy data from urlWithoutTokenInfo
    urlToSend = urlWithoutTokenInfo;
    //TODO: remove other query params if you want in urlToSend

    //get the path not have query params
    string urlWithoutQueryParams = urlWithoutTokenInfo;
    //new first char of query params after remove token info
    firstCharOfQueryParams = firstCharOfQueryParams - (foundLastOfTokenInfo - foundFirstOfTokenInfo);
    if (firstCharOfQueryParams != string::npos) {
        urlWithoutQueryParams = urlWithoutQueryParams.substr(0,firstCharOfQueryParams);
        logInfo("urlWithoutQueryParams: " + urlWithoutQueryParams);
    } else {
        logInfo("not have query params");
    }
    //get path to hash
    size_t findEndSchema = urlWithoutQueryParams.find("://");
    pathToHash = urlWithoutQueryParams.substr(findEndSchema + 3);
    //TODO: remove tail of file
}


int main() {
    map<string, string> queryParams;
    queryParams["C"] = "123";
    queryParams["timestamp"] = "123";
    queryParams["A"] = "123";
    cout << generateAllQueryParams(queryParams) << endl;
    //test ge all path
    string url = "http://localhost:8080/abc/def/ghi/token=123123&timestamp=123123?a=1&b=2";
    string urlToSend;
    string pathToHash;
    string tokenInfo;
    getAllPath(url, urlToSend, pathToHash, tokenInfo);
    cout << "urlToSend: " << urlToSend << endl;
    cout << "pathToHash: " << pathToHash << endl;
    cout << "tokenInfo: " << tokenInfo << endl;

    return 0;
}
