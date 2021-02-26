#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "AsyncHttpd.h"
#pragma comment(lib, "AsyncHttpd.lib")

//your customized function to handle http requests
IAsyncHttpServer::HANDLER_RESULT handler(IAsyncHttpServer::REQUEST* req, LBUF* lbuf,
	uint16_t callback_port, void* callback_ptr, uint32_t callback_uint)
{
	//print out the incoming request
	printf("path:[%s]\n", req->file_path);
	printf("file name:[%s]\n", req->file_name);
	printf("file extention:[%s]\n", req->file_ext);
	printf("query strings:\n");
	for (int i = 0; i < req->v_qs.size(); i += 2) {
		printf("\t[%s]:[%s]\n", req->v_qs[i], req->v_qs[i + 1]);
	}

	printf("content-length:[%d]\n", req->content_length);

	if (lbuf->len == 0) {
		//the net work connection failed
		//you should clear any asynchronous operation for the specific callback_ptr
		//...
		return IAsyncHttpServer::HR_COMPLETED;
	}

	//交给默认处理
	return IAsyncHttpServer::HR_DEFAULT;

	//发送简单内容
	lbuf->len = sprintf(lbuf->buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", 2, "ok");
	return IAsyncHttpServer::HR_COMPLETED;

	//异步处理
	//...
	return IAsyncHttpServer::HR_ASYNC;
	//在其他线程中处理数据并通过UDP包ASYNC_NOTIFY_MSG返回处理结果
}

int main()
{
	//put your web pages in d:/work/www, the default file name is index.html
	auto svr = IAsyncHttpServer::Create("d:/work/www/", &handler);
	svr->Run(0, 80);
}