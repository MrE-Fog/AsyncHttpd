# AsyncHttpd
AsyncHttpd is a foundational framework library for C++ programmers to  build a high performance non-block http server without any knowledge of sockets or network. Current build is for Windows x64.
User of this library just write a C function (handler) which handles http requests and puts the result in the buffer provided by the framework, then create and run the library.
AsyncHttpd runs in a main single thread, but the user should implement the handler in multiple-thread way if asynchronous feature is wanted, in this situation, return HR_ASYNC in your handler callback function. 
