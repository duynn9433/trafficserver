#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "str_builder.h"
#include "hls_add_token.h"

char *
rewrite_m3u8(char *data, char *schema, char *host, char *usePathNoFilename, char *hashQuery, char *query, char *key, int algorithm)
{
  str_builder_t *sb;
  sb = str_builder_create();
  char *p;
  p = strtok(data, "\n");
  while (p != NULL) {
    // rewrite the line
    char *urlWithToken = rewrite_line(schema, host, usePathNoFilename, p, hashQuery, query, key, algorithm);

    // add the line to the str_builder
    str_builder_add_str(sb, urlWithToken, 0);
    str_builder_add_str(sb, "\n", 0);
    // next line
    p = strtok(NULL, "\n");
  }
  // return data
  char *res = str_builder_dump(sb, NULL);
  str_builder_destroy(sb);
  return res;
}

char *
rewrite_line(char *schema, char *host, char *usePathNoFilename, char *line, char *hashQuery, char *query, char *key, int algorithm)
{
  char *res = NULL;
  trim(line);
  if (line == NULL || strlen(line) == 0) {
    return line;
  }
  // Skip empty lines.
  //   if (/^\s*$/.test(line)) {
  //     return line;
  //   }
  // if the line starts with # -> have TAG, then process it
  if (line != NULL && line[0] == '#') {
    // Process #EXT-X-MAP.
    if (strncmp(line, "#EXT-X-MAP", 10) == 0 || strncmp(line, "#EXT-X-PART", 11) == 0 ||
        strncmp(line, "#EXT-X-PRELOAD-HINT", 19) == 0 || strncmp(line, "#EXT-X-RENDITION-REPORT", 23) == 0) {
      res = processURI(schema, host, usePathNoFilename, line, hashQuery, query, key, algorithm);
      return res;
    }
    return line;
  } else {
    // else -> no TAG, it is a link
    res = createUrlWithSignHttpBase(schema, host, usePathNoFilename, line, hashQuery, query, key, algorithm);
    return res;
  }
}

char *
getSubstring(char *startPos, char *endPos)
{
  size_t length = endPos - startPos;
  char *result  = (char *)malloc(length + 1); // +1 for null terminator
  strncpy(result, startPos, length);
  result[length] = '\0'; // Null-terminate the string
  return result;
}

char *
processURI(char *schema, char *host, char *usePathNoFilename, char *line, char *hashQuery, char *query, char *key, int algorithm)
{
  // Find the position of "URI="
  char *uriPtr = strstr(line, "URI=");
  if (uriPtr == NULL) {
    // If "URI=" not found, return
    return line;
  }

  // Get the position of the first '"' after URI as the start position
  char *startPos = strchr(uriPtr, '"');
  if (startPos == NULL) {
    return line;
  }
  startPos++; // Skip the '"'

  // Find the position of the second '"' after startPos
  char *endPos    = strchr(startPos, '"');
  char *lastIndex = NULL;
  while (endPos != NULL) {
    // Check if the second '"' is followed by ',' -> that is last " of URL
    if (*(endPos + 1) == ',') {
      lastIndex = endPos;
      break;
    } else {
      // If not,
      // Find the next '"' character
      lastIndex = endPos;
      endPos++;
      endPos = strchr(endPos, '"');
    }
  }
  if (lastIndex == NULL) {
    return line;
  } else {
    endPos = lastIndex;
  }

  char *urlWithToken =
    createUrlWithSignHttpBase(schema, host, usePathNoFilename, getSubstring(startPos, endPos), hashQuery, query, key, algorithm);

  str_builder_t *sb = str_builder_create();

  char *prefix = getSubstring(line, startPos);
  str_builder_add_str(sb, prefix, 0);
  str_builder_add_str(sb, urlWithToken, 0);
  str_builder_add_str(sb, endPos, 0);

  char *res = str_builder_dump(sb, NULL);
  str_builder_destroy(sb);
  return res;
}

char *
createUrlWithSignHttpBase(char *schema, char *host, char *usePathNoFilename, char *filename, char *hashQuery, char *query,
                          char *key, int algorithm)
{
  char *urlWithoutQuery = generateUrlWithoutQuery(schema, host, usePathNoFilename, filename);
  char *hashUrl         = generateUrlSign(urlWithoutQuery, hashQuery);
  char *token           = generateToken(hashUrl, key, algorithm);
  str_builder_t *sb;
  sb = str_builder_create();

  str_builder_add_str(sb, urlWithoutQuery, 0);
  if (query == NULL || strlen(query) <= 0) {
    str_builder_add_str(sb, "?", 0);
  } else {
    str_builder_add_str(sb, "?", 0);
    str_builder_add_str(sb, query, 0);
    str_builder_add_str(sb, "&", 0);
  }
  str_builder_add_str(sb, "token=", 0);
  str_builder_add_str(sb, token, 0);

  char *res = str_builder_dump(sb, NULL);
  str_builder_destroy(sb);
  return res;
}
char *
generateUrlWithoutQuery(char *schema, char *host, char *usePathNoFilename, char *filename)
{
  str_builder_t *sb;
  sb = str_builder_create();
  if (schema != NULL) {
    str_builder_add_str(sb, schema, 0);
    str_builder_add_str(sb, "://", 0);
  }
  str_builder_add_str(sb, host, 0);
  str_builder_add_str(sb, usePathNoFilename, 0);
  str_builder_add_str(sb, "/", 0);
  str_builder_add_str(sb, filename, 0);

  char *res = str_builder_dump(sb, NULL);
  str_builder_destroy(sb);
  return res;
}
char *
generateUrlSign(char *urlWithoutQuery, char *hashQuery)
{
  str_builder_t *sb;
  sb = str_builder_create();

  str_builder_add_str(sb, urlWithoutQuery, 0);
  if (hashQuery != NULL) {
    str_builder_add_str(sb, "?", 0);
    str_builder_add_str(sb, hashQuery, 0);
  }
  char *res = str_builder_dump(sb, NULL);
  str_builder_destroy(sb);
  return res;
}

// TODO: implement the generateToken function with openssl
char *
generateToken(char *data, char *key, int algorithm)
{
  unsigned int sig_len = 0;
  unsigned char sig[MAX_SIG_SIZE];
  switch (algorithm) {
  case USIG_HMAC_SHA1:
    HMAC(EVP_sha1(), (const unsigned char *)key, strlen(key), 
    (const unsigned char *)data, strlen(data), 
    sig, &sig_len);
    if (sig_len != SHA1_SIG_SIZE) {
      // deny
    }

    break;
  case USIG_HMAC_MD5:
    HMAC(EVP_md5(), (const unsigned char *)key, strlen(key), 
    (const unsigned char *)data, strlen(data), 
    sig, &sig_len);
    if (sig_len != MD5_SIG_SIZE) {
      // deny
    }
    break;
  default:
    // log err
    break;
  }
    sig[sig_len] = '\0';
    char *res = (char *)malloc(2*sig_len + 1);
    memcpy(res, sig, sig_len);
    res[sig_len] = '\0';

    for (int i = 0; i < sig_len; i++) {
        sprintf(&(res[i * 2]), "%02x", sig[i]);
    }

    //free(sig);
    return res;
    // return (char *)sig;
}

//---------------------------------for trim------------------------------------
// Function to trim leading whitespace characters from a string
char *
ltrim(char *str)
{
  while (isspace((unsigned char)*str)) {
    str++;
  }
  return str;
}

// Function to trim trailing whitespace characters from a string
char *
rtrim(char *str)
{
  size_t len = strlen(str);
  while (len > 0 && isspace((unsigned char)str[len - 1])) {
    len--;
  }
  str[len] = '\0';
  return str;
}

// Function to trim leading and trailing whitespace characters from a string
char *
trim(char *str)
{
  return rtrim(ltrim(str));
}
//---------------------------------end for trim------------------------------------