#pragma once
#include "url_sig_generate_token.h"

char *rewrite_m3u8(char *data, char *schema, char *host, char *usePathNoFilename, char *hashQuery, char *query, char *key,
                   int algorithm);
char *rewrite_line(char *schema, char *host, char *usePathNoFilename, char *line, char *hashQuery, char *query, char *key,
                   int algorithm);
char *createUrlWithSignHttpBase(char *schema, char *host, char *usePathNoFilename, char *filename, char *hashQuery, char *query,
                                char *key, int algorithm);
char *processURI(char *schema, char *host, char *usePathNoFilename, char *line, char *hashQuery, char *query, char *key,
                 int algorithm);
char *generateUrlWithoutQuery(char *schema, char *host, char *usePathNoFilename, char *filename);
char *generateUrlSign(char *urlWithoutQuery, char *hashQuery);
char *generateToken(char *data, char *key, int algorithm);
char *ltrim(char *str);
char *rtrim(char *str);
char *trim(char *str);