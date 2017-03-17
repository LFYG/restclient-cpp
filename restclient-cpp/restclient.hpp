/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef _WINNET_RESTCLIENT_HPP_INCLUDED_
#define _WINNET_RESTCLIENT_HPP_INCLUDED_

#include <string>
#include <map>

#include "winnet_helper.hpp"

using namespace winnet;

namespace RestClient
{
	typedef struct {
		int code;
		std::string		body;
		HeaderFields	headers;
		std::string		cookies;
		Cookies			Cookie;
		std::string get_header(const std::string& name)
		{
			return this->headers[name];
		};
		std::string get_cookie(const std::string& name)
		{
			return this->Cookie[name];
		};
	} Response;

	typedef struct{
		int timeout;
		bool followRedirects = true;
		HeaderFields headers;
	} Request;

	Request default_request = {3000,true,
		{
			std::make_pair("User-Agent","winnet http client v1.0")
		} 
	};

	Response get(const std::string& url,Request* request = nullptr) {
		Response ret;

		CWinNet http;

		if (http.Open(url, HTTP_METHOD_GET))
		{
			if (request == nullptr)
				request = &default_request;

			if (request->timeout > 0)
				http.SetTimeout(request->timeout);

			http.FollowRedirects(request->followRedirects);

			if (!request->headers.empty())
			{
				for (auto &kv : request->headers)
					http.SetRequestHeader(kv.first.c_str(), kv.second.c_str());
			}

			if (http.Send())
			{
				std::string result;
				std::vector<BYTE> response = http.GetResponseBody();
				result.insert(result.begin(), response.begin(), response.end());
				ret.body = result;
				ret.code = http.GetStatusCode();
				ret.cookies = http.GetCookieStr();
				ret.Cookie = http.GetCookies();
				ret.headers = http.GetHeaderFields();
			}	
		}
		return ret;
	}

	Response post(const std::string& url, const std::string& content_type, const std::string& data, Request* request = nullptr) {
		
		Response ret;

		CWinNet http;

		if (http.Open(url, HTTP_METHOD_POST))
		{
			if (request == nullptr)
				request = &default_request;

			http.FollowRedirects(request->followRedirects);

			http.SetRequestHeader("Content-Type", content_type);

			if (request->timeout > 0)
				http.SetTimeout(request->timeout);

			if (!request->headers.empty())
			{
				for (auto &kv : request->headers)
					http.SetRequestHeader(kv.first.c_str(), kv.second.c_str());
			}

			if (http.Send((LPVOID)data.c_str(), data.length()))
			{
				std::string result;
				std::vector<BYTE> response = http.GetResponseBody();
				result.insert(result.begin(), response.begin(), response.end());
				ret.body = result;
				ret.code = http.GetStatusCode();
				ret.cookies = http.GetCookieStr();
				ret.Cookie = http.GetCookies();
				ret.headers = http.GetHeaderFields();
			}
		}
		return ret;
	}

	Response put(const std::string& url, const std::string& content_type, const std::string& data, Request* request = nullptr) {
		
		Response ret;
		
		CWinNet http;
		
		if (http.Open(url, HTTP_METHOD_PUT))
		{
			if (request == nullptr)
				request = &default_request;

			http.FollowRedirects(request->followRedirects);

			http.SetRequestHeader("Content-Type", content_type);

			if (request->timeout > 0)
				http.SetTimeout(request->timeout);

			if (!request->headers.empty())
			{
				for (auto &kv : request->headers)
					http.SetRequestHeader(kv.first.c_str(), kv.second.c_str());
			}

			if (http.Send((LPVOID)data.c_str(), data.length()))
			{
				std::string result;
				std::vector<BYTE> response = http.GetResponseBody();
				result.insert(result.begin(), response.begin(), response.end());
				ret.body = result;
				ret.code = http.GetStatusCode();
				ret.cookies = http.GetCookieStr();
				ret.Cookie = http.GetCookies();
				ret.headers = http.GetHeaderFields();
			}
		}
		return ret;
	}

	Response del(const std::string& url, Request* request = nullptr) {
		Response ret;

		CWinNet http;

		if (http.Open(url, HTTP_METHOD_DELETE))
		{
			if (request == nullptr)
				request = &default_request;

			if (request->timeout > 0)
				http.SetTimeout(request->timeout);

			http.FollowRedirects(request->followRedirects);

			if (!request->headers.empty())
			{
				for (auto &kv : request->headers)
					http.SetRequestHeader(kv.first.c_str(), kv.second.c_str());
			}

			if (http.Send())
			{
				std::string result;
				std::vector<BYTE> response = http.GetResponseBody();
				result.insert(result.begin(), response.begin(), response.end());
				ret.body = result;
				ret.code = http.GetStatusCode();
				ret.cookies = http.GetCookieStr();
				ret.Cookie = http.GetCookies();
				ret.headers = http.GetHeaderFields();
			}
		}
		return ret;
	}

	Response head(const std::string& url, Request* request = nullptr) {
		Response ret;

		CWinNet http;

		if (http.Open(url, HTTP_METHOD_HEAD))
		{
			if (request == nullptr)
				request = &default_request;

			if (request->timeout > 0)
				http.SetTimeout(request->timeout);

			http.FollowRedirects(request->followRedirects);

			if (!request->headers.empty())
			{
				for (auto &kv : request->headers)
					http.SetRequestHeader(kv.first.c_str(), kv.second.c_str());
			}

			if (http.Send())
			{
				std::string result;
				std::vector<BYTE> response = http.GetResponseBody();
				result.insert(result.begin(), response.begin(), response.end());
				ret.body = result;
				ret.code = http.GetStatusCode();
				ret.cookies = http.GetCookieStr();
				ret.Cookie = http.GetCookies();
				ret.headers = http.GetHeaderFields();
			}
		}
		return ret;
	}
}



#endif // _WINNET_RESTCLIENT_HPP_INCLUDED_
