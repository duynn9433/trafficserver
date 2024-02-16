
#include <map>
#include <iostream>
#include <utility>
#include <algorithm>
extern "C" {
  #include "url_sig_config.h"
  #include <stdio.h>
  #include "str_builder.h"
  #include <ctype.h>
  #include "str_builder.h"
}

void
free_cfg(struct config *cfg)
{
  TSDebug(PLUGIN_NAME, "Cleaning up");
  TSfree(cfg->err_url);
  TSfree(cfg->sig_anchor);

  if (cfg->regex_extra) {
#ifndef PCRE_STUDY_JIT_COMPILE
    pcre_free(cfg->regex_extra);
#else
    pcre_free_study(cfg->regex_extra);
#endif
  }

  if (cfg->regex) {
    pcre_free(cfg->regex);
  }

  TSfree(cfg);
}
int
readRemapConfig(char *remapConfigPath, std::map<std::string, config_t> *configMap){
  struct config *cfg;
  const char *tsConfigDir = TSConfigDirGet();
  cfg = static_cast<struct config *>(TSmalloc(sizeof(struct config)));
  memset(cfg, 0, sizeof(struct config));
  FILE *file = fopen(remapConfigPath, "r");
  if (file == NULL) {
    TSError("[%s] Unable to open file %s", PLUGIN_NAME, remapConfigPath);
    return TS_ERROR;
  }
  char line[300];
  int line_no = 0;
  int keynum;
  int paramNum = 0;

  std::string fullLine;
  while (fgets(line, sizeof(line), file) != NULL) {
    std::string domainKey;
    char *urlConfigFileName;
    if (line[0] == '#' || strlen(line) <= 1) {
      continue;
    }
    //TODO: check get full line
    if (line[strlen(line) - 1] == '\n' && line[strlen(line) - 2] == '\\') {
      fullLine.append(line);
      fullLine.erase(fullLine.length() - 2, 2);
      continue;
    } else if (line[strlen(line) - 1] == '\n') {
      std::string tempLine(line);
      //trim left
      tempLine.erase(tempLine.begin(), std::find_if(tempLine.begin(), tempLine.end(), [](unsigned char ch) {
        return !std::isspace(ch);}));

      fullLine.append(" ").append(tempLine);
      fullLine.erase(fullLine.length() - 1, 1);
      fullLine.append("\0");
      strcpy(line, fullLine.c_str());
      fullLine.clear();
    }

    line_no++;
    TSDebug(PLUGIN_NAME, "LINE %d: %s (%d)", line_no, line, (int)strlen(line));
    //get first domain and pparam behind url_sig
    char * pch;
    pch = strtok (line," ");
    //ignore the first value
    int i = 1;
    pch = strtok (NULL, " ");
    while (pch != NULL)
    {
      //first domain
      if(i==1) {
        i++;
        TSDebug(PLUGIN_NAME, "first domain: %s", pch);
        //get only domain
        std::string domain(pch);
        size_t pos = domain.find("//");
        if (pos != std::string::npos) {
            domain = domain.substr(pos + 2);
        }
        pos = domain.find("/");
        if (pos != std::string::npos) {
            domain = domain.substr(0, pos);
        }
        pos = domain.find(":");
        if (pos != std::string::npos) {
            domain = domain.substr(0, pos);
        }
        domainKey = domain;
        TSDebug(PLUGIN_NAME, "get only domain: %s", domainKey.c_str());
      }
      //param behind url_sig
      if(strncmp(pch, "@plugin=url_sig", 15) == 0){
        pch =strtok (NULL, " ");
        TSDebug(PLUGIN_NAME, "param behind url_sig: %s", pch);
        // get only file name
        std::string filename(pch);
        filename = filename.substr(8);
        urlConfigFileName = strdup(filename.c_str());
        TSDebug(PLUGIN_NAME, "get only file name: %s", urlConfigFileName);
        break;
      }
      //next
      pch = strtok (NULL, " ");
    }
    //------------------------------process remap config element-------------------------
    char url_config_file_buf[300], *url_config_file;
    snprintf(url_config_file_buf, sizeof(url_config_file_buf), "%s/%s", tsConfigDir, urlConfigFileName);
    url_config_file = url_config_file_buf;
    TSDebug(PLUGIN_NAME, "url_config_file: %s", url_config_file);
    readRemapConfigElement(domainKey, url_config_file, configMap);


    //------------------------------free-------------------------
    TSfree(urlConfigFileName);
  }
  TSDebug(PLUGIN_NAME, "Map size: %d", configMap->size());
  // Print configMap
  for (auto it = configMap->begin(); it != configMap->end(); ++it) {
    std::string domain = it->first;
    config_t cfg = it->second;
    TSDebug(PLUGIN_NAME, "Domain: %s\n", domain.c_str());
    TSDebug(PLUGIN_NAME, "Keys:");
    for (int i = 0; i < MAX_KEY_NUM; ++i) {
      TSDebug(PLUGIN_NAME, "  Key %d: %s", i, cfg.keys[i]);
    }
    TSDebug(PLUGIN_NAME, "Error URL: %s", cfg.err_url ? cfg.err_url : "NULL");
    TSDebug(PLUGIN_NAME, "Signature Anchor: %s", cfg.sig_anchor);
    TSDebug(PLUGIN_NAME, "Ignore Expiry: %s", cfg.ignore_expiry ? "true" : "false");
    TSDebug(PLUGIN_NAME, "Pristine URL Flag: %d", cfg.pristine_url_flag);
    TSDebug(PLUGIN_NAME, "Hash Query Parameters:");
    for (int i = 0; i < cfg.paramNum; ++i) {
      TSDebug(PLUGIN_NAME, "  Param %d: %s", i, cfg.hash_query_param[i]);
    }
    TSDebug(PLUGIN_NAME, "");
  }
}

int
readRemapConfigElement(std::string domain, char *remapConfigElementPath, std::map<std::string, config_t> *configMap)
{
  struct config *cfg;
  cfg = static_cast<struct config *>(TSmalloc(sizeof(struct config)));
  memset(cfg, 0, sizeof(struct config));
  FILE *file = fopen(remapConfigElementPath, "r");
  if (file == NULL) {
    TSError("[%s] Unable to open file %s", PLUGIN_NAME, remapConfigElementPath);
    return TS_ERROR;
  }
  char line[300];
  int line_no = 0;
  int keynum;
  int paramNum = 0;
  bool eat_comment = false;

  while (fgets(line, sizeof(line), file) != NULL) {
    TSDebug(PLUGIN_NAME, "LINE: %s (%d)", line, (int)strlen(line));
    line_no++;

    if (eat_comment) {
      // Check if final char is EOL, if so we are done eating
      if (line[strlen(line) - 1] == '\n') {
        eat_comment = false;
      }
      continue;
    }
    if (line[0] == '#' || strlen(line) <= 1) {
      // Check if we have a comment longer than the full buffer if no EOL
      if (line[strlen(line) - 1] != '\n') {
        eat_comment = true;
      }
      continue;
    }
    char *pos = strchr(line, '=');
    if (pos == NULL) {
      TSError("[url_sig_generate_token] Error parsing line %d of file %s (%s)", line_no, remapConfigElementPath, line);
      continue;
    }
    *pos        = '\0';
    char *value = pos + 1;
    while (isspace(*value)) { // remove whitespace
      value++;
    }
    pos = strchr(value, '\n'); // remove the new line, terminate the string
    if (pos != NULL) {
      *pos = '\0';
    }
    if (pos == NULL || strlen(value) >= MAX_KEY_LEN) {
      // snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Maximum key length (%d) exceeded on line %d", MAX_KEY_LEN - 1, line_no);
      fclose(file);
      free_cfg(cfg);
      return TS_ERROR;
    }
    //-------------------------add to config-------------------------
    if (strncmp(line, "key", 3) == 0) {
      if (strncmp(line + 3, "0", 1) == 0) {
        keynum = 0;
      } else {
        TSDebug(PLUGIN_NAME, ">>> %s <<<", line + 3);
        keynum = atoi(line + 3);
        if (keynum == 0) {
          keynum = -1; // Not a Number
        }
      }
      TSDebug(PLUGIN_NAME, "key number %d == %s", keynum, value);
      if (keynum >= MAX_KEY_NUM || keynum < 0) {
        // snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Key number (%d) >= MAX_KEY_NUM (%d) or NaN", keynum, MAX_KEY_NUM);
        fclose(file);
        free_cfg(cfg);
        return TS_ERROR;
      }
      snprintf(&cfg->keys[keynum][0], MAX_KEY_LEN, "%s", value);
    } else if (strncmp(line, "error_url", 9) == 0) {
      if (atoi(value)) {
        // cfg->err_status = atoi(value);
      }
      value += 3;
      while (isspace(*value)) {
        value++;
      }
      if (cfg->err_status == TS_HTTP_STATUS_MOVED_TEMPORARILY) {
        cfg->err_url = TSstrndup(value, strlen(value));
      } else {
        cfg->err_url = NULL;
      }
    } else if (strncmp(line, "sig_anchor", 10) == 0) {
      cfg->sig_anchor = TSstrndup(value, strlen(value));
    } else if (strncmp(line, "excl_regex", 10) == 0) {
      // compile and study regex
      const char *errptr;
      int erroffset, options = 0;

      if (cfg->regex) {
        TSDebug(PLUGIN_NAME, "Skipping duplicate excl_regex");
        continue;
      }

      cfg->regex = pcre_compile(value, options, &errptr, &erroffset, NULL);
      if (cfg->regex == NULL) {
        TSDebug(PLUGIN_NAME, "Regex compilation failed with error (%s) at character %d", errptr, erroffset);
      } else {
#ifdef PCRE_STUDY_JIT_COMPILE
        options = PCRE_STUDY_JIT_COMPILE;
#endif
        cfg->regex_extra = pcre_study(
          cfg->regex, options, &errptr); // We do not need to check the error here because we can still run without the studying?
      }
    } else if (strncmp(line, "ignore_expiry", 13) == 0) {
      if (strncmp(value, "true", 4) == 0) {
        cfg->ignore_expiry = true;
        TSError("[url_sig_generate_token] Plugin IGNORES sig expiration");
      }
    } else if (strncmp(line, "url_type", 8) == 0) {
      if (strncmp(value, "pristine", 8) == 0) {
        cfg->pristine_url_flag = 1;
        TSDebug(PLUGIN_NAME, "Pristine URLs (from config) will be used");
      }
    } else if (strncmp(line, "hash_query_param", 16) == 0) {
      char* param;
      while ((param = strtok_r(value, ",", &param))) {
        TSDebug(PLUGIN_NAME, "Param number %d: %s", paramNum, param);
        snprintf(&cfg->hash_query_param[paramNum][0], MAX_HASH_QUERY_LEN, "%s", param);
        value = value + strlen(param) + 1;
        paramNum = paramNum + 1;
      }
      cfg->paramNum = paramNum;
    } else {
      TSError("[url_sig_generate_token] Error parsing line %d of file %s (%s)", line_no, remapConfigElementPath, line);
    }
  }
  //add to map
  configMap->insert(std::pair<std::string, config_t>(domain, *cfg));
  TSDebug(PLUGIN_NAME, "Map size in element: %d", configMap->size());
  return 0;
}


