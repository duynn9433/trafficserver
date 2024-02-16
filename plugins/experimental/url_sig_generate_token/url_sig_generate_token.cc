/** @file
  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#define min(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })
#include <map>
#include <string>

extern "C" {
  #include "url_sig_config.h"
#include "url_sig_generate_token.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include "hls_add_token.h"
#include <ts/remap.h>
}

static std::map<std::string, config_t> config_map;

TSReturnCode
TSRemapInit(TSRemapInterface *api_info, char *errbuf, int errbuf_size)
{
  if (!api_info) {
    snprintf(errbuf, errbuf_size, "[tsremap_init] - Invalid TSRemapInterface argument");
    return TS_ERROR;
  }

  if (api_info->tsremap_version < TSREMAP_VERSION) {
    snprintf(errbuf, errbuf_size, "[TSRemapInit] - Incorrect API version %ld.%ld", api_info->tsremap_version >> 16,
             (api_info->tsremap_version & 0xffff));
    return TS_ERROR;
  }

  TSDebug(PLUGIN_NAME, "plugin is successfully initialized");

  return TS_SUCCESS;
}

// To force a config file reload touch remap.config and do a "traffic_ctl config reload"
TSReturnCode
TSRemapNewInstance(int argc, char *argv[], void **ih, char *errbuf, int errbuf_size)
{
  char config_filepath_buf[PATH_MAX], *config_file, remap_config_file_buf[300], *remap_config_file;
  struct config *cfg;
  const char *tsConfigDir = TSConfigDirGet();

  if ((argc < 3) || (argc > 4)) {
    snprintf(errbuf, errbuf_size,
             "[TSRemapNewInstance] - Argument count wrong (%d)... config file path is required first pparam, \"pristineurl\" is"
             "optional second pparam.",
             argc);
    return TS_ERROR;
  }
  TSDebug(PLUGIN_NAME, "Initializing remap function of %s -> %s with config from %s", argv[0], argv[1], argv[2]);

  if (argv[2][0] == '/') {
    config_file = argv[2];
  } else {
    snprintf(config_filepath_buf, sizeof(config_filepath_buf), "%s/%s", tsConfigDir, argv[2]);
    config_file = config_filepath_buf;
  }
  TSDebug(PLUGIN_NAME, "config file name: %s", config_file);
  snprintf(remap_config_file_buf, sizeof(remap_config_file_buf), "%s/%s", tsConfigDir, "remap.config");
  remap_config_file = remap_config_file_buf;
  // read file
  config_map.clear();
  readRemapConfig(remap_config_file, &config_map);

  //

  FILE *file = fopen(config_file, "r");
  if (file == NULL) {
    snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Error opening file %s", config_file);
    return TS_ERROR;
  }

  char line[300];
  int line_no = 0;
  int keynum;
  int paramNum     = 0;
  bool eat_comment = false;

  cfg = static_cast<struct config *>(TSmalloc(sizeof(struct config)));
  memset(cfg, 0, sizeof(struct config));

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
      TSError("[url_sig] Error parsing line %d of file %s (%s)", line_no, config_file, line);
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
      snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Maximum key length (%d) exceeded on line %d", MAX_KEY_LEN - 1, line_no);
      fclose(file);
      free_cfg(cfg);
      return TS_ERROR;
    }
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
        snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Key number (%d) >= MAX_KEY_NUM (%d) or NaN", keynum, MAX_KEY_NUM);
        fclose(file);
        free_cfg(cfg);
        return TS_ERROR;
      }
      snprintf(&cfg->keys[keynum][0], MAX_KEY_LEN, "%s", value);
    } else if (strncmp(line, "error_url", 9) == 0) {
      if (atoi(value)) {
        cfg->err_status = static_cast<TSHttpStatus>(atoi(value));
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
        TSError("[url_sig] Plugin IGNORES sig expiration");
      }
    } else if (strncmp(line, "url_type", 8) == 0) {
      if (strncmp(value, "pristine", 8) == 0) {
        cfg->pristine_url_flag = 1;
        TSDebug(PLUGIN_NAME, "Pristine URLs (from config) will be used");
      }
    } else if (strncmp(line, "hash_query_param", 16) == 0) {
      char *param;
      while ((param = strtok_r(value, ",", &param))) {
        TSDebug(PLUGIN_NAME, "Param number %d: %s", paramNum, param);
        snprintf(&cfg->hash_query_param[paramNum][0], MAX_HASH_QUERY_LEN, "%s", param);
        value    = value + strlen(param) + 1;
        paramNum = paramNum + 1;
      }
      cfg->paramNum = paramNum;
    } else {
      TSError("[url_sig] Error parsing line %d of file %s (%s)", line_no, config_file, line);
    }
  }

  fclose(file);

  if (argc > 3) {
    if (strcasecmp(argv[3], "pristineurl") == 0) {
      cfg->pristine_url_flag = 1;
      TSDebug(PLUGIN_NAME, "Pristine URLs (from args) will be used");

    } else {
      snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - second pparam (if present) must be pristineurl");
      free_cfg(cfg);
      return TS_ERROR;
    }
  }

  switch (cfg->err_status) {
  case TS_HTTP_STATUS_MOVED_TEMPORARILY:
    if (cfg->err_url == NULL) {
      snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Invalid config, err_status == 302, but err_url == NULL");
      free_cfg(cfg);
      return TS_ERROR;
    }
    break;
  case TS_HTTP_STATUS_FORBIDDEN:
    if (cfg->err_url != NULL) {
      snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Invalid config, err_status == 403, but err_url != NULL");
      free_cfg(cfg);
      return TS_ERROR;
    }
    break;
  default:
    snprintf(errbuf, errbuf_size, "[TSRemapNewInstance] - Return code %d not supported", cfg->err_status);
    free_cfg(cfg);
    return TS_ERROR;
  }

  *ih = (void *)cfg;
  return TS_SUCCESS;
}

void
TSRemapDeleteInstance(void *ih)
{
  free_cfg((struct config *)ih);
}

static void
err_log(const char *url, const char *msg)
{
  if (msg && url) {
    TSDebug(PLUGIN_NAME, "[URL=%s]: %s", url, msg);
    TSError("[url_sig] [URL=%s]: %s", url, msg); // This goes to error.log
  } else {
    TSError("[url_sig] Invalid err_log request");
  }
}

// See the README.  All Signing parameters must be concatenated to the end
// of the url and any application query parameters.
// remove query param for url_sig plugin (not use for another purpose)
static void
log_callback(void *ptr, int level, const char *fmt, va_list vl)
{
  char log[1024];
  vsnprintf(log, 1024, fmt, vl);
  printf("%s", log);
}
static void
run()
{
  char schema[]            = "http";
  char host[]              = "example.com";
  char path[]              = "/path/to/playlist.m3u8";
  char hashQuery[]         = "timestamp=1906277895";
  char query[]             = "timestamp=1906277895&timeshift=123&a=1&c=2";
  char usePathNoFilename[] = "/path/to";
  // The m3u8 data buffer
  char buffer[] = "#EXTM3U \n"
                  "#EXT-X-PART:URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_753218.m4s\",DURATION=0.48\n"
                  "#EXT-X-MAP:URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_753218.m4s\",DURATION=0.48\n"
                  "#EXT-X-MAP:URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_753218.m4s\"\n"
                  "## Created with Broadpeak BkS350 Origin Packager  (version=1.12.8-28913) \n"
                  "# variants \n"
                  "#EXT-X-VERSION:3 \n"
                  "#EXT-X-STREAM-INF:BANDWIDTH=795000,AVERAGE-BANDWIDTH=723000,CODECS=\"mp4a.40.2,avc1.4D001E\",RESOLUTION=640x360,"
                  "FRAME-RATE=25.0 \n"
                  "158-audio_133600_eng=131600-video=556400.m3u8\n"
                  "#EXT-X-STREAM-INF:BANDWIDTH=2190000,AVERAGE-BANDWIDTH=1991000,CODECS=\"mp4a.40.2,avc1.4D0028\",RESOLUTION="
                  "1920x1080,FRAME-RATE=25.0 \n"
                  "158-audio_133600_eng=131600-video=1752000.m3u8\n"
                  "#EXT-X-STREAM-INF:BANDWIDTH=6157000,AVERAGE-BANDWIDTH=5597000,CODECS=\"mp4a.40.2,avc1.640028\",RESOLUTION="
                  "1920x1080,FRAME-RATE=25.0 \n"
                  "158-audio_133600_eng=131600-video=5154400.m3u8\n"
                  "#EXT-X-PART:URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_753218.m4s\",DURATION=0.48\n"
                  "#EXT-X-MAP:URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_753218.m4s\",DURATION=0.48\n"
                  "#EXT-X-MAP:URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_753218.m4s\"\n"
                  "#EXT-X-PRELOAD-HINT:TYPE=PART,URI=\"hls/5154400_avc1_640028_1920x1080_25_1-695825300_956506.m4s\"\n"
                  "#EXT-X-RENDITION-REPORT:URI=\"131600_mp4a_40_2_2ch_48000Hz_16bits_eng.m3u8\",LAST-MSN=695825300,LAST-PART=0\n"
                  "#EXT-X-RENDITION-REPORT:URI=\"556400_avc1_4d001e_640x360_25_1.m3u8\",LAST-MSN=695825300,LAST-PART=1\n";
  char key[]    = "px0KnwI_hxaS8uNzLOUZw6lVuBqVggJH";
  int algorithm = 2;
  char *res     = rewrite_m3u8(buffer, schema, host, usePathNoFilename, hashQuery, query, key, algorithm);
  printf("%s", res);

  // test read remap.config
  // readRemapConfig("remap.config");

  return;
}

static char *
getAppQueryString(const char *query_string, int query_length)
{
  //---------------------------------------------test ffmpeg---------------------------------------------
  run();

  // Open the M3U8 content from the buffer
  //---------------------------------------------test ffmpeg---------------------------------------------

  int done = 0;
  char *param_start, *param_end;
  char buf[MAX_QUERY_LEN + 10];

  if (query_length > MAX_QUERY_LEN) {
    TSError(PLUGIN_NAME, "Cannot process the query string as the length exceeds %d bytes", MAX_QUERY_LEN);
    return NULL;
  }
  memset(buf, 0, sizeof(buf));
  memcpy(buf, query_string, query_length);
  param_start = buf;
  param_end   = buf;

  TSDebug(PLUGIN_NAME, "query_string: %s, query_length: %d", query_string, query_length);
  /*----------remove token---------*/
  // TODO: split to func if need to remove more
  char param[] = SIG_QSTRING "=";
  param_start  = strstr(param_start, param);
  if (param_start != NULL) {
    // get end param
    param_end = strchr(param_start, '&');
    if (param_end != NULL) {
      param_end            = param_end + 1;
      int end_len          = strlen(param_end);
      param_start[end_len] = '\0';
      memmove(param_start, param_end, end_len);
    } else {
      *(param_start - 1) = '\0';
    }
  }

  if (strlen(buf) > 0) {
    param_start = TSstrdup(buf);
    return param_start;
  } else {
    return NULL;
  }
}

/** fixedBufferWrite safely writes no more than *dest_len bytes to *dest_end
 * from src. If copying src_len bytes to *dest_len would overflow, it returns
 * zero. *dest_end is advanced and *dest_len is decremented to account for the
 * written data. No null-terminators are written automatically (though they
 * could be copied with data).
 */
static int
fixedBufferWrite(char **dest_end, int *dest_len, const char *src, int src_len)
{
  if (src_len > *dest_len) {
    return 0;
  }
  memcpy(*dest_end, src, src_len);
  *dest_end += src_len;
  *dest_len -= src_len;
  return 1;
}

static char *
urlParse(char const *const url_in, char *anchor, char *new_path_seg, int new_path_seg_len, char *signed_seg,
         unsigned int signed_seg_len)
{
  char *segment[MAX_SEGMENTS];
  char url[8192]                     = {'\0'};
  unsigned char decoded_string[2048] = {'\0'};
  char new_url[8192]; /* new_url is not null_terminated */
  char *p = NULL, *sig_anchor = NULL, *saveptr = NULL;
  int i = 0, numtoks = 0, sig_anchor_seg = 0;
  size_t decoded_len = 0;

  strncat(url, url_in, sizeof(url) - strlen(url) - 1);

  char *new_url_end    = new_url;
  int new_url_len_left = sizeof(new_url);

  char *new_path_seg_end    = new_path_seg;
  int new_path_seg_len_left = new_path_seg_len;

  char *skip = strchr(url, ':');
  if (!skip || skip[1] != '/' || skip[2] != '/') {
    return NULL;
  }
  skip += 3;
  // preserve the scheme in the new_url.
  if (!fixedBufferWrite(&new_url_end, &new_url_len_left, url, skip - url)) {
    TSError("insufficient space to copy schema into new_path_seg buffer.");
    return NULL;
  }
  TSDebug(PLUGIN_NAME, "%s:%d - new_url: %.*s\n", __FILE__, __LINE__, (int)(new_url_end - new_url), new_url);

  // parse the url.
  if ((p = strtok_r(skip, "/", &saveptr)) != NULL) {
    segment[numtoks++] = p;
    do {
      p = strtok_r(NULL, "/", &saveptr);
      if (p != NULL) {
        segment[numtoks] = p;
        if (anchor != NULL && sig_anchor_seg == 0) {
          // look for the signed anchor string.
          if ((sig_anchor = strcasestr(segment[numtoks], anchor)) != NULL) {
            // null terminate this segment just before he signing anchor, this should be a ';'.
            *(sig_anchor - 1) = '\0';
            if ((sig_anchor = strstr(sig_anchor, "=")) != NULL) {
              *sig_anchor = '\0';
              sig_anchor++;
              sig_anchor_seg = numtoks;
            }
          }
        }
        numtoks++;
      }
    } while (p != NULL && numtoks < MAX_SEGMENTS);
  } else {
    return NULL;
  }
  if ((numtoks >= MAX_SEGMENTS) || (numtoks < 3)) {
    return NULL;
  }

  // create a new path string for later use when dealing with query parameters.
  // this string will not contain the signing parameters.  skips the fqdn by
  // starting with segment 1.
  for (i = 1; i < numtoks; i++) {
    // if no signing anchor is found, skip the signed parameters segment.
    if (sig_anchor == NULL && i == numtoks - 2) {
      // the signing parameters when no signature anchor is found, should be in the
      // last path segment so skip them.
      continue;
    }
    if (!fixedBufferWrite(&new_path_seg_end, &new_path_seg_len_left, segment[i], strlen(segment[i]))) {
      TSError("insufficient space to copy into new_path_seg buffer.");
      return NULL;
    }
    if (i != numtoks - 1) {
      if (!fixedBufferWrite(&new_path_seg_end, &new_path_seg_len_left, "/", 1)) {
        TSError("insufficient space to copy into new_path_seg buffer.");
        return NULL;
      }
    }
  }
  *new_path_seg_end = '\0';
  TSDebug(PLUGIN_NAME, "new_path_seg: %s", new_path_seg);

  // save the encoded signing parameter data
  if (sig_anchor != NULL) { // a signature anchor string was found.
    if (strlen(sig_anchor) < signed_seg_len) {
      memcpy(signed_seg, sig_anchor, strlen(sig_anchor));
    } else {
      TSError("insufficient space to copy into new_path_seg buffer.");
    }
  } else { // no signature anchor string was found, assume it is in the last path segment.
    if (strlen(segment[numtoks - 2]) < signed_seg_len) {
      memcpy(signed_seg, segment[numtoks - 2], strlen(segment[numtoks - 2]));
    } else {
      TSError("insufficient space to copy into new_path_seg buffer.");
      return NULL;
    }
  }
  TSDebug(PLUGIN_NAME, "signed_seg: %s", signed_seg);

  // no signature anchor was found so decode and save the signing parameters assumed
  // to be in the last path segment.
  if (sig_anchor == NULL) {
    if (TSBase64Decode(segment[numtoks - 2], strlen(segment[numtoks - 2]), decoded_string, sizeof(decoded_string), &decoded_len) !=
        TS_SUCCESS) {
      TSDebug(PLUGIN_NAME, "Unable to decode the  path parameter string.");
    }
  } else {
    if (TSBase64Decode(sig_anchor, strlen(sig_anchor), decoded_string, sizeof(decoded_string), &decoded_len) != TS_SUCCESS) {
      TSDebug(PLUGIN_NAME, "Unable to decode the  path parameter string.");
    }
  }
  TSDebug(PLUGIN_NAME, "decoded_string: %s", decoded_string);

  {
    int oob = 0; /* Out Of Buffer */

    for (i = 0; i < numtoks; i++) {
      // cp the base64 decoded string.
      if (i == sig_anchor_seg && sig_anchor != NULL) {
        if (!fixedBufferWrite(&new_url_end, &new_url_len_left, segment[i], strlen(segment[i]))) {
          oob = 1;
          break;
        }
        if (!fixedBufferWrite(&new_url_end, &new_url_len_left, (char *)decoded_string, strlen((char *)decoded_string))) {
          oob = 1;
          break;
        }
        if (!fixedBufferWrite(&new_url_end, &new_url_len_left, "/", 1)) {
          oob = 1;
          break;
        }

        continue;
      } else if (i == numtoks - 2 && sig_anchor == NULL) {
        if (!fixedBufferWrite(&new_url_end, &new_url_len_left, (char *)decoded_string, strlen((char *)decoded_string))) {
          oob = 1;
          break;
        }
        if (!fixedBufferWrite(&new_url_end, &new_url_len_left, "/", 1)) {
          oob = 1;
          break;
        }
        continue;
      }
      if (!fixedBufferWrite(&new_url_end, &new_url_len_left, segment[i], strlen(segment[i]))) {
        oob = 1;
        break;
      }
      if (i < numtoks - 1) {
        if (!fixedBufferWrite(&new_url_end, &new_url_len_left, "/", 1)) {
          oob = 1;
          break;
        }
      }
    }
    if (oob) {
      TSError("insufficient space to copy into new_url.");
    }
  }
  return TSstrndup(new_url, new_url_end - new_url);
}

TSRemapStatus
TSRemapDoRemap(void *ih, TSHttpTxn txnp, TSRemapRequestInfo *rri)
{

  return TSREMAP_NO_REMAP;
}
