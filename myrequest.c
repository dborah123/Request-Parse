#include "myrequest.h"
#include <stdio.h>

struct Request *
parse_request(char *input_buf) {
    /**
     * Parses raw request data, placing it into a Request struct
     */

    struct Request *request = malloc(sizeof(struct Request));

    printf("here\n");

    if (request == NULL) {
        return NULL;
    }

    // Parsing method
    size_t method_len = strcspn(input_buf, " ");

    if (strncmp(input_buf, "GET", method_len) == 0) {
        request->method = GET;
    } else if (strncmp(input_buf, "POST", method_len) == 0) {
        request->method = POST;
    } else {
        request->method = UNSUPPORTED;
    }

    input_buf += method_len + 1;

    // Parsing URI
    size_t uri_len = strcspn(input_buf, " ");
    request->uri = malloc(uri_len + 1);

    if (request->uri == NULL) {
        free_request(request);
        return NULL;
    }

    memcpy(request->uri, input_buf, uri_len);
    request->uri[uri_len] = '\0';
    input_buf += uri_len + 1;

    // Parsing HTTP
    size_t http_len = strcspn(input_buf, "\n");
    request->http = malloc(sizeof(http_len + 1));

    if (request->http == NULL) {
        free_request(request);
        return NULL;
    }

    memcpy(request->http, input_buf, http_len);
    request->http[http_len] = '\0';
    input_buf += http_len + 1;

    /* Handling Headers */
    struct Header *curr_header, *prev_header;
    size_t name_len, value_len;

    printf("Request Info: uri: %s, http %s\n", request->uri, request->http);

    while (input_buf[0] != '\n') {
        // Allocating new header struct
        prev_header = curr_header;
        curr_header = malloc(sizeof(struct Header));

        if (curr_header == NULL) {
            free_request(request);
            return NULL;
        }

        // Name
        name_len = strcspn(input_buf, ":");
        curr_header->name = malloc(name_len + 1);

        if (curr_header->name == NULL) {
            free_header(curr_header);
            free_request(request);
            return NULL;
        }

        strncpy(curr_header->name, input_buf, name_len);
        curr_header->name[name_len] = '\0';
        input_buf += name_len + 2;

        // Value
        value_len = strcspn(input_buf, "\n");
        curr_header->value = malloc(value_len + 1);

        if (curr_header->value == NULL) {
            free_header(curr_header);
            free_request(request);
            return NULL;
        }
/*
        if (strlen(curr_header->name) == 0) {
            break;
        }
*/
        strncpy(curr_header->value, input_buf, value_len);
        curr_header->value[value_len] = '\0';
        input_buf += value_len + 1;

        printf("name: %s\tvalue: %s\n", curr_header->name, curr_header->value);
 
        // Preparing for next iteration
        curr_header->next = prev_header;
    }
    printf("after while loop\n");
    request->headers = curr_header;
    input_buf += 2;

    printf("handling body\n");
    /* Body */
    size_t body_len = strlen(input_buf);
    request->body = malloc(body_len + 1);

    if (request->body == NULL) {
        free_header(request->headers);
        free_request(request);
        return NULL;
    }

    strncpy(request->body, input_buf, body_len);
    request->body[body_len] = '\0';

    return request;
}


//struct Pair *
//parse_body(char *body) {
    /**
     * Parses body into a linked list of key value pairs
     * Note: This should most likely be a hashtable, but I do not want extra dependencies
     */

    //struct Pair *curr_pair, *prev_pair

    //while (*body != '\n') {
        
  //  }
//}

void
free_request(struct Request *request) {
    /**
     * Frees request struct and returns void upon completion
     * Note: This function does not free header linked list. Must call 
     * free_header function to do this
     */

    free(request->uri);
    free(request->body);
    free(request);
}


void
free_header(struct Header *header) {
    /**
     * Frees header structs nested in linked list and returns void upon completion.
     * It is best practice to call this function from the head of the linked list
     */
    if (header != NULL) {
        free(header->name);
        free(header->value);
        free_header(header->next);
        free(header);
    }
}

