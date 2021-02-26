# AsyncHttpd
AsyncHttpd is a foundational framework library for C++ programmers to  build a high performance non-block http server without any knowledge of sockets or network. Current build is for Windows x64.

Users of this library just write a C function (handler) to handle parsed http request and put the result in the provided buffer, then create and run the framework which will callback your customized function.

The framework has a default handler, your customized handler can just return HR_DEFAULT if you don't want handle the request.

AsyncHttpd runs in a main single thread, but the user should implement the handler in multiple-thread way if asynchronous feature is wanted, in this situation, return HR_ASYNC in your handler callback function. 
