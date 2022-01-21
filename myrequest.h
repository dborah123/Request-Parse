#ifndef __MYREQUEST_H
#define __MYREQUEST_H
#include <string.h>

typedef enum Method { GET, POST, UNSUPPORTED } Method;

typedef struct Header {
    char *name;
    char *value;
    struct Head next;
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
    struct Pair next;
}

struct Request *parse_request(char *input_buf);
void free_request(struct Request *request);
void free_header(struct Header *header);

#endif /* __MYREQUEST_H */
