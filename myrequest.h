#ifndef __MYREQUEST_H
#define __MYREQUEST_H
#include <string.h>
#include <stdlib.h>

typedef enum Method { GET, POST, UNSUPPORTED } Method;

typedef struct Header {
    char *name;
    char *value;
    struct Header *next;
} Header;

typedef struct Request {
    enum Method method;
    char *uri;
    char *http;
    struct Header *headers;
    struct Pair *body; 
} Request;

typedef struct Pair {
    char *name;
    char *value;
    struct Pair *next;
} Pair;

struct Request *parse_request(char *input_buf);
struct Pair *parse_body(char *body, int body_len);
char *get_value_pair(char *name, struct Pair *pair);
void free_request(struct Request *request);
void free_header(struct Header *header);
void free_entire_request(struct Request *request);
void free_pairs(struct Pair *pair);

#endif /* __MYREQUEST_H */
