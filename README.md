# Request-Parse

## Description
Custom parsing library I created to handle server-side network requests from browsers. Parses request information, headers, and body. Can parse GET and POST requests and keeps track of header and body information using linked lists. This is meant to be a super basic parser and is not meant for optimized uses. Rather, it is merely an exercise to familiarize myself with requests and using parsing tools.

## Building
1. Clone repo into project of folder you want to use Request-Parse
2. Add these two lines to Makefile:
    ```
    myrequest: ./request/myrequest.c
	        gcc $(CFLAGS) -c -o myrequest.o ./request/myrequest.c
    ```
    and object file will be added upon make to your project folder. It is on the user to use this object file correctly to whatever program you are creating
3. Remember to include `#include <Request-Parse/myrequest.h>` in the files using Request-Parse

## Limitations
* Linked Lists: Linked lists were chosen as the data structure for storing headers and body elements because of ease of creation. If I were to try to optimize Request-Parse, I would use a hashtable because of the speedy lookup time for particular elements.
* Parser is rigid meaning that it relies on a lot of givens. In particular, `\n` and other characters as such to know when to store the next element. Alongside this, parser does not accept other types of form data. Can only parse basic HTML form data for POST request.
