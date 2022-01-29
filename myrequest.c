#include "myrequest.h"
#include <stdio.h>

struct Request *
parse_request(char *input_buf) {
    /**
     * Parses raw request data, placing it into a Request struct
     */

    struct Request *request = malloc(sizeof(struct Request));

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
    struct Header *curr_header, *prev_header = NULL;
    size_t name_len, value_len;
    int body_len;

    // Setting headers to NULL
    request->headers = NULL;

    while (input_buf[0] != '\r' || input_buf[1] != '\n') {
        prev_header = curr_header;
        curr_header = malloc(sizeof(struct Header));

        // Allocating new header struct
        if (curr_header == NULL) {
            free_entire_request(request);
            return NULL;
        }

        // Name
        name_len = strcspn(input_buf, ":");
        curr_header->name = malloc(name_len + 1);

        if (curr_header->name == NULL) {
            free_entire_request(request);
            return NULL;
        }

        strncpy(curr_header->name, input_buf, name_len);
        curr_header->name[name_len] = '\0';
        input_buf += name_len + 2;

        // Value
        value_len = strcspn(input_buf, "\n");
        curr_header->value = malloc(value_len + 1);

        if (curr_header->value == NULL) {
            free_entire_request(request);
            return NULL;
        }

        strncpy(curr_header->value, input_buf, value_len);
        curr_header->value[value_len] = '\0';
        input_buf += value_len + 1;

        //Setup for next iteration
        if (!request->headers) {
            request->headers = curr_header;
        } else {
            prev_header->next = curr_header;
        }
    }
    input_buf += 1;

    /* Body */
    body_len = strlen(input_buf);

    // Check if body is empty
    if (body_len <= 1) {
        request->body = NULL;
    } else if (strchr(request->uri, '.')) { // If uri is a file request
        request->body = NULL;
    } else if (request->method == GET) {
        request->body = NULL;
    } else {
        request->body = parse_body(input_buf, body_len);
        if (request->body == NULL) {
            free_entire_request(request);
            return NULL;
        }
    }

    return request;
}


struct Pair *
parse_body(char *body, int body_len) {
    /**
     * Parses body into a linked list of key value pairs
     * Note: This should most likely be a hashtable, but I do not want extra dependencies
     */
    int position = 0;
    struct Pair *curr_pair, *prev_pair, *original_pair = NULL;

    while (position < body_len) {
        prev_pair = curr_pair;
        curr_pair = malloc(sizeof(Pair));

        if (!curr_pair) {
            return NULL;
        }

        if (!original_pair) {
            original_pair = curr_pair;
        } else {
            prev_pair->next = curr_pair;
        }


        if (!curr_pair) {
            free_pairs(original_pair);
            return NULL;
        }

        // Getting key value pair
        if (position == 0) {
            curr_pair->name = strtok(body, "=");
        } else {
            curr_pair->name = strtok(NULL, "=");
        }
        position += strlen(curr_pair->name) + 1;

        curr_pair->value = strtok(NULL,"&");
        position += strlen(curr_pair->value) + 1;
    }
    return original_pair;
}


char *
get_value_pair(char *name, struct Pair *pair) {
    /**
     * Used in conjunction with pairs module in this lib. Takes name and returns 
     * value, much like get functions for hashtables
     */

    while (pair) {
        if (strcmp(pair->name, name) == 0) {
            return pair->value;
        }
        pair = pair->next;
    }

    return NULL;
}


/* FREE FUNCTIONS */

void
free_request(struct Request *request) {
    /**
     * Frees request struct and returns void upon completion
     * Is null safe.
     *
     * Note: This function does not free header linked list. Must call 
     * free_header function to do this. Or free_entire_request.
     */
    if (!request) {
        return;
    }

    if (!request->uri) {
        free(request->uri);
    }

    if (!request->body) {
        free(request->body);
    }

    free(request);
}


void
free_headers(struct Header *header) {
    /**
     * Frees header structs nested in linked list and returns void upon completion.
     * Is null safe.
     * It is best practice to call this function from the head of the linked list
     */
    if (header != NULL) {
        if (!header->name) {
            free(header->name);
        }

        if (!header->value) {
            free(header->value);
        }

        free_headers(header->next);
        free(header);
    }
}


void
free_entire_request(struct Request *request) {
    /**
     * Frees the entire requesr struct denoted as a pointer in param
     * Is null safe
     */

    if (!request) {
        return;
    }

    if (!request->uri) {
        free(request->uri);
    }

    free_headers(request->headers);

    if (!request->body) {
        free_pairs(request->body);
    }

    free(request);   
}


void
free_pairs(struct Pair *pair) {
    /**
     * Frees Pairs in LL starting from head and iterating through
     * Is null safe.
     */

    if (!pair) {
         return;
    }

    if (pair->name) {
        free(pair->name);
    }

    if (pair->value) {
        free(pair->value);
    }

    free_pairs(pair->next);

    free(pair);
}
