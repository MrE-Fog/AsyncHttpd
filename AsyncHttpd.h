#pragma once

/*
* AsyncHttpd is a foundational framework library for C++ programmers to 
* build a high performance non-block http server without any knowledge of sockets or network.
* AsyncHttpd runs in a main single thread, but the user should implement the handler in multiple-thread way 
* if the asynchronous feature is wanted, at this situation, return HR_ASYNC in the handler callback function. 
*/

#include <vector>

struct LBUF {
	int len;
	char buf[];
};

class IAsyncHttpServer
{
public:

	//the http request decoded by the framework
	struct REQUEST {
		const char* file_path;
		const char* file_name;
		const char* file_ext;
		std::vector<const char*> v_qs;
		char* after_qs;
		int content_length; //should be zero for http GET method
		char* content;
		int content_recved;
	};

	enum HANDLER_RESULT {
		HR_DEFAULT,		//let the framework to handle the request
		HR_COMPLETED,	//handling completed, data in lbuf
		HR_PARTIAL,		//lbuf length not enough, only return part data, framework will call handler with the same callback_ptr again to get data
		HR_ASYNC,		//asynchronouse operation
	};

	enum ASYNC_RESULT {
		ASYNC_COMPLETED,	//handling completed, data in lbuf
		ASYNC_PARTIAL,	//partial data in lbuf, framework will call handler with the same callback_ptr for more data
		ASYNC_FAILED,		//handling failed, framework will close the network connection
	};

	struct ASYNC_NOTIFY_MSG {
		uint32_t tag;		//must be 0x746e6263
		int async_result;		//ASYNC_RESULT
		LBUF* lbuf;
		void* skt_ptr;
		uint32_t callback_uint;
	};

	/* 
	* User should provide a customized handler callback function to the static Create function to create the server inferface object.
	* The framework will call back the handler function upon http requests.
	* Following parameters are passed to the handler function by framework.
	*		req: the http request parsed by the framework
	*		lbuf: the buffer provided by the framework for handler to put the response content in.
	*			  lbuf->len is an in/out parameter
	*			  if lbuf->len != 0, it is the buffer size provided by the framework, should be 8172 now
	*			  if lbuf->len == 0, callback_port will be 0, connection has been closed, handler should clean itself
	*			  lbuf->len should be the response content's length when returned
	*
	*		The following 3 parameters are for asynchonous handling operation. 
	*		If the data preparation takes time, handler should return HR_ASYNC immediately, and prepare the data in a background thread.
	*
	*		callback_port:	Send a ASYNC_NOTIFY_MSG message to port number callback_port of localhost(127.0.0.1) when you get the response data ready.
	*		skt_ptr:		A pointer to the underlying tcp socket of the network connection. You should put this parameter in the ASYNC_NOTIFY_MSG message when you notify framework.
	*						It's strongly recommended that you don't directly use the underlying socket unless you need to handle post data or send large chunk of data. The underlying socket is non-block.
	*		callback_uint:	A verification data. Put this parameter in the ASYNC_NOTIFY_MSG message when you notify framework.
	*/
	typedef HANDLER_RESULT(*Handler)(REQUEST* req, LBUF* lbuf, uint16_t callback_port, void* skt_ptr, uint32_t callback_uint);

public:
	//external implemented in foundational framework lib
	static IAsyncHttpServer* Create(const char* root, Handler handler);

public:
	//usage, set ip to 0 to allow any ip
	virtual int Run(uint32_t ip, uint16_t port) = 0;
};