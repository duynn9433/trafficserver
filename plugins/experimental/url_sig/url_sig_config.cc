
#include "url_sig_config.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <ctime>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <climits>
#include <cctype>
#include <cstdint>
#include <cstdbool>
#include <string>
#include <list>
#include <fstream>
#include <string_view>
#include <memory>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <cstddef> // Add this line to include the necessary header for size_t

#include "url_sig_config.h"

#include <cinttypes>
#include <cstdio>

using namespace std;

void print_log(const char *tag, const char *format_str, ...){
  va_list args;
  va_start(args, format_str);
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), format_str, args);
  std::cout << tag << ": " << buffer << std::endl;
  va_end(args);
  // print_log(tag, buffer, args);
}

void print_err(const char *format_str, ...) {
  va_list args;
  va_start(args, format_str);
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), format_str, args);
  std::cerr << buffer << std::endl;
  va_end(args);
}

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

shared_ptr<config>
get_config_from_file(string_view config_file) {
  shared_ptr<config> cfg = make_shared<config>();
  //open & check exist file
  ifstream file("example.txt"); // Open the file
  if (!file) {
      print_err("[url_sig] Error opening file %s", config_file);
      return nullptr;
  }

  string line;
  int line_no = 0;
  int param_num = 0;
  int method_num = 0;
  int timeshift_param_num = 0;
  bool enable_watermark = false;


  while (std::getline(file, line)) { // Read the file line by line
    line_no++;
    print_log(PLUGIN_NAME, "LINE %d: %s (%d)", line_no, line.c_str(), (int)line.size());

    if (line[0] == '#' || line.size() <= 1) {
      continue;
    }

    ltrim(line);
    rtrim(line);
    size_t pos = line.find('=');
    if(pos == string::npos) {
      print_err("[url_sig] Error parsing line %d of file %s (%s)", line_no, config_file, line.c_str());
      continue;
    }

    string key = line.substr(0, pos);
    string value = line.substr(pos + 1);
    while (isspace(value[0])) { // remove whitespace
      value = value.substr(1);
    }
    if (value[value.size() - 1] == '\n') {
      value = value.substr(0, value.size() - 1);
    }

    if(key.compare("key") == 0) {
      int keynum = 0;
      if (key.compare(3, 1, "0") == 0) {
        keynum = 0;
      } else {
        keynum = atoi(key.substr(3).c_str());
        if (keynum == 0) {
          keynum = -1; // Not a Number
        }
      }
      print_log(PLUGIN_NAME, "key number %d == %s", keynum, value.c_str());
      if (keynum >= MAX_KEY_NUM || keynum < 0) {
        print_err("[url_sig] Error parsing line %d of file %s (%s)", line_no, config_file, line.c_str());
        continue;
      }
      cfg->keys_map[keynum] = value;
    } else if(key.compare("error_url") == 0) {
      if (atoi(value.c_str())) {
        cfg->err_status = static_cast<TSHttpStatus>(atoi(value.c_str()));
      }
      value = value.substr(3);
      while (isspace(value[0])) {
        value = value.substr(1);
      }
      if (cfg->err_status == TS_HTTP_STATUS_MOVED_TEMPORARILY) {
        cfg->err_url = value;
      } else {
        cfg->err_url = "";
      }
    } else if(key.compare("sig_anchor") == 0) {
      cfg->sig_anchor = value;
    } else if(key.compare("excl_regex") == 0) {
      // compile and study regex
      // const char *errptr;
      unique_ptr<char*> errptr;
      int erroffset, options = 0;

      if (cfg->regex) {
        print_log(PLUGIN_NAME, "Skipping duplicate excl_regex");
        continue;
      }

      cfg->regex = make_shared<pcre>(pcre_compile(value.c_str(), options, errptr.get(), &erroffset, nullptr ));
      if (cfg->regex == nullptr ) {
        print_log(PLUGIN_NAME, "Regex compilation failed with error (%s) at character %d", errptr, erroffset);
      } else {
        #ifdef PCRE_STUDY_JIT_COMPILE
          options = PCRE_STUDY_JIT_COMPILE;
        #endif
        cfg->regex_extra = make_shared<pcre_extra>(pcre_study(cfg->regex.get(), options, errptr.get())); // We do not need to check the error here because we can still run without the studying?
      }
    } else if(key.compare("ignore_expiry") == 0) {
      if(value.compare("true") == 0) {
        cfg->ignore_expiry = true;
        print_err("[url_sig] Plugin IGNORES sig expiration");
      }
    } else if (key.compare("url_type") == 0) {
      if (value.compare("pristine") == 0) {
        cfg->pristine_url_flag = 1;
        print_log(PLUGIN_NAME, "Pristine URLs (from config) will be used");
      }
    } else if (key.compare("hash_query_param") == 0) {
      string param;
      stringstream ss(value);
      char del = ',';
      string word;
      while (!ss.eof()) {
          getline(ss, word, del);
          print_log(PLUGIN_NAME, "Param number %d: %s", cfg->hash_query_param.size(), word.c_str());
          cfg->hash_query_param.push_back(word);
      }
      cfg->paramNum = cfg->hash_query_param.size();
    } else if (key.compare("use_parts") == 0) {
      cfg->use_parts = value;
      print_log(PLUGIN_NAME, "Use_part: %s", cfg->use_parts.c_str());
    } else if (key.compare("algorithm") == 0) {
      cfg->algorithm = atoi(value.c_str());
    } else if (key.compare("knumber") == 0) {
      cfg->knumber = atoi(value.c_str());
    } else if (key.compare("bypass_method") == 0) {
      stringstream ss(value);
      char del = ',';
      string word;
      while (!ss.eof()) {
          getline(ss, word, del);
          print_log(PLUGIN_NAME, "Bypass method number %d: %s", cfg->bypass_method.size(), word.c_str());
          cfg->bypass_method.push_back(word);
      }
      cfg->method_num = cfg->bypass_method.size();
    } else if (key.compare("timeshift_param") == 0) {
      stringstream ss(value);
      char del = ',';
      string word;
      while (!ss.eof()) {
          getline(ss, word, del);
          print_log(PLUGIN_NAME, "Timeshift param number %d: %s", cfg->timeshift_param.size(), word.c_str());
          cfg->timeshift_param.push_back(word);
      }
      cfg->timeshift_param_num = cfg->timeshift_param.size();
    } else if (key.compare("enable_watermark") == 0) {
      int enable = atoi(value.c_str());
      if (enable == 1) {
        cfg->enable_watermark = true;
      }
    } else {
      print_err("[url_sig] Error parsing line %d of file %s (%s)", line_no, config_file, line.c_str());
    }
  }

  file.close(); // Close the file



  return cfg;
}



void
destroy_config(config *cfg){
  //TODO: remove att in struct config


  delete cfg;
}

void
print_config(config cfg) {
  print_log(PLUGIN_NAME, "Url_sig plugin config: ");
  //print all attribute in struct config

}

int main() {
  shared_ptr<config> cfg = get_config_from_file("url_sig.conf");

  print_config(*cfg);


}
