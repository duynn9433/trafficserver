
#include <map>
extern "C" {
  #include "url_sig_config.h"
  #include <stdio.h>
  #include "str_builder.h"
}

bool getLine(char *line, FILE *file);

static void
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
readRemapConfig(char *remapConfigPath)
{
  FILE *file = fopen(remapConfigPath, "r");
  if (file == NULL) {
    TSError("[%s] Unable to open file %s", PLUGIN_NAME, remapConfigPath);
    return TS_ERROR;
  }
  char line[300];
  while (getLine(line, file) != false) {
    // print line
    TSDebug(PLUGIN_NAME, "line: %s", line);
    // check if remap config is contain url_sig
    if (strstr(line, "url_sig") != NULL) {
      // parse remap config
      //parseRemapConfig(line);
    }
  }
}

bool
getLine(char *line, FILE *file)
{
  str_builder_t *sb = str_builder_create();
  while (fgets(line, sizeof(line), file) != NULL) {
    str_builder_add_str(sb, line, 0);
    // get last char of line
    char lastChar = line[strlen(line) - 1];
    if (lastChar == '\\') {
      // remove last char
      str_builder_drop(sb, 1);
      continue;
    } else {
      // get all the line of 1 remap config
      line = str_builder_dump(sb, NULL);
      str_builder_destroy(sb);
      return true;
    }
  }
  str_builder_destroy(sb);
  return false;
}