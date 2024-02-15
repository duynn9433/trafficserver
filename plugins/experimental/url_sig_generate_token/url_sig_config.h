#pragma once
#include "url_sig_generate_token.h"
#ifdef HAVE_PCRE_PCRE_H
#include <pcre/pcre.h>
#else
#include <pcre.h>
#endif

struct config {
  TSHttpStatus err_status;
  char *err_url;
  char keys[MAX_KEY_NUM][MAX_KEY_LEN];
  pcre *regex;
  pcre_extra *regex_extra;
  int pristine_url_flag;
  char *sig_anchor;
  bool ignore_expiry;
  char hash_query_param[MAX_HASH_QUERY_PARAM_NUM][MAX_HASH_QUERY_LEN];
  int paramNum;
};

typedef struct config config_t;

static void free_cfg(config_t *cfg);
int readRemapConfig(char *remapConfigPath);
