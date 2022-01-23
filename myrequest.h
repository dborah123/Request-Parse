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
    char *body;
    
} Request;

typedef struct Pair {
    char *name;
    char *value;
    struct Pair *next;
} Pair;

struct Request *parse_request(char *input_buf);
void free_request(struct Request *request);
void free_header(struct Header *header);
void free_entire_request(struct Request *request);

#endif /* __MYREQUEST_H */
