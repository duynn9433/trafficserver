#pragma once
#define USIG_HMAC_SHA1 1
#define USIG_HMAC_MD5 2
#define CIP_STRLEN 20
#define EXP_STRLEN 16
#define PAR_STRLEN 16
#define MAX_PARTS 32
#define MAX_SEGMENTS 64

#define MAX_HTTP_REQUEST_SIZE 8192 //

#define MAX_SIG_SIZE 20
#define SHA1_SIG_SIZE 20
#define MD5_SIG_SIZE 16

#define MAX_REQ_LEN 8192
#define MAX_KEY_LEN 256
#define MAX_KEY_NUM 16
#define MAX_QUERY_LEN 4096
#define MAX_HASH_QUERY_PARAM_NUM 16
#define MAX_HASH_QUERY_LEN 256

char *rewrite_m3u8(char *data, 
                char *schema, char *host, char *usePathNoFilename,
                char *hashQuery, char *query,
                char *key, int algorithm);
char *rewrite_line(char *schema, char *host, char *usePathNoFilename, char *line, char *hashQuery, char *query, char *key, int algorithm);
char *createUrlWithSignHttpBase(char *schema, char *host, char *usePathNoFilename, char *filename, char *hashQuery, char *query, char *key, int algorithm);
char *processURI(char *schema, char *host, char *usePathNoFilename, char *line, char *hashQuery, char *query, char *key, int algorithm);
char *generateUrlWithoutQuery(char *schema, char *host, char *usePathNoFilename, char *filename); 
char *generateUrlSign(char *urlWithoutQuery, char *hashQuery);
char *generateToken(char *data, char *key, int algorithm);
char *ltrim(char *str);
char *rtrim(char *str);
char *trim(char *str);