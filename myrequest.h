#ifndef __MYREQUEST_H
#define __MYREQUEST_H
#include <string.h>

typedef enum Method { GET, POST, UNSUPPORTED } Method;

typedef struct Header {
    char *name
}


typedef struct Request {
    enum Method method;
    char *uri;
    struct Header *headers;
    char *body;
    
}

struct Request *parse_request(char *input_buf);
void *free_request(struct Request *request);
void *free_header(struct Header *header);

#endif /* __MYREQUEST_H */
