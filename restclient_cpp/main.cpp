// restclient_cpp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define _USE_WIHTTP_INTERFACE

#include "../restclient-cpp/textconv_helper.hpp"
#include "../restclient-cpp/restclient.hpp"

void response_log(RestClient::Response& response)
{
	//response utf8
	std::wstring body = textconv_helper::A2W_(response.body.c_str(), CP_UTF8);
	wprintf(L"body = %s \n", body.c_str());
	printf("code = %d \n", response.code);
	printf("cookie = %s \n", response.cookies.c_str());
	printf("%s %s \n", response.Cookie["BAIDUID"].c_str(), response.get_cookie("BAIDUID").c_str());
	printf("%s %s \n", response.headers["Location"].c_str(), response.get_header("Location").c_str());
	printf("%s %s \n", response.headers["Content-Type"].c_str(), response.get_header("Content-Type").c_str());
	printf("%s %s \n", response.headers["Content-Length"].c_str(), response.get_header("Content-Length").c_str());
}


void one_test()
{
	RestClient::Response r;
	r = RestClient::get("http://www.baidu.com/");
	response_log(r);
	r = RestClient::post("http://www.baidu.com/post", "text/json", "{\"foo\": \"bla\"}");
	response_log(r);
	r = RestClient::put("http://www.baidu.com/put", "text/json", "{\"foo\": \"bla\"}");
	response_log(r);
	r = RestClient::del("http://www.baidu.com/delete");
	response_log(r);
	r = RestClient::head("http://www.baidu.com/");
	response_log(r);
}

void get_test()
{
	RestClient::Request request;

	request.timeout = 3000;
	request.followRedirects = false;
	request.headers["User-Agent"]	= "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36";
	request.headers["Cookie"]		= "name=value;";

	RestClient::Response response = RestClient::get("http://www.baidu.com", &request);

	response_log(response);
}

void post_test()
{
	RestClient::Request request;
	request.timeout = 3000;
	request.followRedirects = false;
	request.headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36";
	request.headers["Cookie"] = "name=value;";

	RestClient::Response response = RestClient::post("http://www.baidu.com/post.php?a=123456", "text/json", "{\"foo\": \"bla\"}", &request);

	response_log(response);
}

void proxy_test()
{
	std::string strProxy = "http=115.29.2.139:80";

	RestClient::Request request;
	request.proxy.proxy = strProxy;
	request.proxy.username = "";
	request.proxy.password = "";

	request.timeout = 3000;
	request.followRedirects = false;
	request.headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36";
	request.headers["Cookie"] = "name=value;";

	RestClient::Response response = RestClient::get("http://ipip.yy.com/get_ip_info.php", &request);

	response_log(response);
}

int main()
{
	setlocale(LC_ALL, "CHS");

	one_test();
	get_test();
	post_test();
	proxy_test();

	getchar();
    return 0;
}

