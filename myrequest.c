#include "myrequest.h"

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

    memcpy(request->uri, input_buf, uri_len + 1);
    request->uri[uri_len];
}
