/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef _WINNET_HPP_INCLUDED_
#define _WINNET_HPP_INCLUDED_

#include <string>
#include <tchar.h>
#include <vector>
#include <map>
#include <regex>

#include <windows.h>
#include <strsafe.h>
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")

namespace winnet
{
	/// <summary>
	/// Common HTTP methods.
	/// </summary>		
	const std::string HTTP_METHOD_GET		= "GET";
	const std::string HTTP_METHOD_POST		= "POST";
	const std::string HTTP_METHOD_PUT		= "PUT";
	const std::string HTTP_METHOD_HEAD		= "HEAD";
	const std::string HTTP_METHOD_DELETE	= "DELETE";

	typedef std::map<std::string, std::string> HeaderFields;
	typedef std::map<std::string, std::string> Cookies;

	class CWinNet
	{
	private:

		HINTERNET			m_hSession;
		HINTERNET			m_hConnect;
		HINTERNET			m_hRequest;

		std::string			m_strHost;
		std::string			m_strPath;
		std::string			m_strExt;

		INTERNET_SCHEME		m_nScheme;
		INTERNET_PORT		m_nPort;

		bool				m_followRedirects;

		HeaderFields		m_RequestHeaders;

		std::string			m_request_method;

		Cookies ParseCookies(const std::string& cookies)
		{
			Cookies result;
			
			std::string cookies_pattern("([\\S]+?)=([^;|^\\r|^\\n]+)");
			std::regex cookies_regex(cookies_pattern);
			std::smatch results;

			auto cookies_begin = std::sregex_iterator(cookies.begin(), cookies.end(), cookies_regex);
			auto cookies_end = std::sregex_iterator();

			for (auto i = cookies_begin; i != cookies_end; ++i)
			{
				std::smatch match = *i;
				if (match.size() == 3)
					result[match[1].str()] = match[2].str();
			}

			return result;
		}

	public:
		CWinNet(void) : m_hSession(NULL)
			, m_hConnect(NULL)
			, m_hRequest(NULL)
			, m_followRedirects(FALSE)
		{

		}

		~CWinNet()
		{
			if (m_hRequest) InternetCloseHandle(m_hRequest);
			if (m_hConnect) InternetCloseHandle(m_hConnect);
			if (m_hSession) InternetCloseHandle(m_hSession);
		}

		BOOL Open(const std::string& Url, const std::string& method)
		{
			m_hSession = InternetOpenA("winnet http client v1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
			if (!m_hSession) return FALSE;

			m_request_method = method;

			// Initialize the URL_COMPONENTS structure.
			URL_COMPONENTSA urlComp;
			ZeroMemory(&urlComp, sizeof(urlComp));
			urlComp.dwStructSize = sizeof(urlComp);

			// The case where lpszHostName is NULL and dwHostNameLength is non-0 means
			// to return pointers into the passed in URL along with lengths. The 
			// specific non-zero value is irrelevant
			urlComp.dwHostNameLength = 1;
			urlComp.dwUserNameLength = 1;
			urlComp.dwPasswordLength = 1;
			urlComp.dwUrlPathLength = 1;
			urlComp.dwExtraInfoLength = 1;

			BOOL bRet	= InternetCrackUrlA(Url.c_str(), 0, 0, &urlComp);

			m_strHost	= std::string(urlComp.lpszHostName, urlComp.dwHostNameLength);
			m_strPath	= std::string(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);
			m_strExt	= std::string(urlComp.lpszExtraInfo, urlComp.dwExtraInfoLength);
			m_nScheme	= urlComp.nScheme;
			m_nPort		= urlComp.nPort;

			m_strPath.append(m_strExt);

			return bRet;
		}

		void SetTimeout(DWORD dwTimeOut)
		{
			InternetSetOptionA(m_hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));
			InternetSetOptionA(m_hSession, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));
			InternetSetOptionA(m_hSession, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeOut, sizeof(dwTimeOut));
		}

		void SetRequestHeader(const std::string& key, const std::string& value)
		{
			m_RequestHeaders[key] = value;
		}

		void SetHeaders(HeaderFields headers)
		{
			m_RequestHeaders = headers;
		}

		void SetUserAgent(const std::string& userAgent) {
			SetRequestHeader("User-Agent", userAgent);
		}

		void SetCookie(const std::string& cookies)
		{
			SetRequestHeader("Cookie", cookies);
		}

		void SetReferer(const std::string& referer)
		{
			SetRequestHeader("Referer", referer);
		}

		void FollowRedirects(bool follow)
		{
			m_followRedirects = follow;
		}

		BOOL Send(LPVOID lpPostData = NULL, DWORD dwsize = 0)
		{
			m_hConnect = InternetConnectA(m_hSession, m_strHost.c_str(), m_nPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

			if (!m_hConnect) return FALSE;

			//强制请求最新文件,不使用缓存,不使用本地Cookies
			DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_COOKIE_THIRD_PARTY | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_CACHE_WRITE;

			//忽略因服务器的证书主机名与请求的主机名不匹配所导致的错误,忽略由已失效的服务器证书导致的错误
			dwFlags = dwFlags | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;

			if (m_nScheme == INTERNET_SCHEME_HTTPS)
				dwFlags = dwFlags | INTERNET_FLAG_SECURE;//SSL

			if (!m_followRedirects)
				dwFlags = dwFlags | INTERNET_FLAG_NO_AUTO_REDIRECT;//禁止重定向

			m_hRequest = HttpOpenRequestA(m_hConnect, m_request_method.c_str(), m_strPath.c_str(), "HTTP/1.1", NULL, NULL, dwFlags, NULL);

			if (!m_hRequest) return FALSE;

			//启用gzip自动解压,可以加快速度
			DWORD dwGzip = 1;
			InternetSetOptionW(m_hRequest, INTERNET_OPTION_HTTP_DECODING, &dwGzip, sizeof(dwGzip));

			std::string strHeaders;
			for (HeaderFields::iterator iter = m_RequestHeaders.begin(); iter != m_RequestHeaders.end(); iter++)
			{
				strHeaders.append(iter->first);
				strHeaders.append(": ");
				strHeaders.append(iter->second);
				strHeaders.append("\n");
			}

			m_RequestHeaders.clear();

			return HttpSendRequestA(m_hRequest, strHeaders.c_str(), strHeaders.size(), lpPostData, dwsize);
		}

		std::vector<BYTE> GetResponseBody()
		{
			DWORD dwReadSize = 0;
			DWORD dwDownloaded = 0;

			std::vector<BYTE> list;

			do
			{
				// Check for available data.
				dwReadSize = 0;
				if (!InternetQueryDataAvailable(m_hRequest, &dwReadSize, 0, 0))
				{
					printf("Error %u in InternetQueryDataAvailable.\n", GetLastError());
					break;
				}

				// No more available data.
				if (!dwReadSize)
					break;

				// Allocate space for the buffer.
				//pszOutBuffer = new char[dwSize + 1];
				BYTE* lpReceivedData = new BYTE[dwReadSize];
				if (!lpReceivedData)
				{
					printf("Out of memory\n");
					break;
				}
				else
				{
					// Read the Data.
					ZeroMemory(lpReceivedData, dwReadSize);

					if (!InternetReadFile(m_hRequest, lpReceivedData, dwReadSize, &dwDownloaded)) {
						printf("Error %u in InternetReadFile.\n", GetLastError());
					}
					else {
						for (size_t i = 0; i < dwReadSize; i++)
							list.push_back(lpReceivedData[i]);
					}

					// Free the memory allocated to the buffer.
					delete[] lpReceivedData;

					// This condition should never be reached since WinHttpQueryDataAvailable
					// reported that there are bits to read.
					if (!dwDownloaded)
						break;
				}
			} while (dwReadSize > 0);
			return list;
		}

		std::string GetResponseHeaders()
		{
			return GetResponseHeaderValue(HTTP_QUERY_RAW_HEADERS_CRLF);
		}

		std::string GetResponseHeaderValue(const std::string& name)
		{
			std::string	result;

			DWORD			dwSize			= name.length() + 1;
			LPVOID			lpOutBuffer		= new char[dwSize];

			StringCchPrintfA((LPSTR)lpOutBuffer, dwSize, name.c_str());

		retry:

			if (!HttpQueryInfoA(m_hRequest, HTTP_QUERY_CUSTOM, (LPVOID)lpOutBuffer, &dwSize, NULL))
			{
				// Check for an insufficient buffer.
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					// Allocate the necessary buffer.
					delete[] lpOutBuffer;
					lpOutBuffer = NULL;

					lpOutBuffer = new char[dwSize];

					// Rewrite the header name in the buffer.
					StringCchPrintfA((LPSTR)lpOutBuffer, dwSize, name.c_str());

					// Retry the call.
					goto retry;
				}
				else
				{
					// Error handling code.
					delete[] lpOutBuffer;
					lpOutBuffer = NULL;
				}
			}

			if (lpOutBuffer)
				result.assign(reinterpret_cast<LPCSTR>(lpOutBuffer));

			return result;
		}

		std::string GetResponseHeaderValue(int dwInfoLevel, DWORD dwIndex = NULL)
		{
			std::string		result;
			LPVOID			lpOutBuffer = NULL;
			DWORD			dwSize = 0;

		retry:

			if (!HttpQueryInfoA(m_hRequest, dwInfoLevel, (LPVOID)lpOutBuffer, &dwSize, &dwIndex))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					// Allocate the necessary buffer.
					lpOutBuffer = new char[dwSize];
					// Retry the call.
					goto retry;
				}
			}

			if (lpOutBuffer)
				result.assign(reinterpret_cast<LPCSTR>(lpOutBuffer));

			if (lpOutBuffer)
				delete[] lpOutBuffer;

			return result;
		}

		HeaderFields GetHeaderFields()
		{
			std::string strHeaders = GetResponseHeaders();
			
			HeaderFields Headers;

			std::string pattern("(.+?): ([^;|^\\r|^\\n]+)");
			std::regex headers_regex(pattern);
			std::smatch results;

			auto _begin = std::sregex_iterator(strHeaders.begin(), strHeaders.end(), headers_regex);
			auto _end = std::sregex_iterator();
			
			for (auto i = _begin; i != _end; ++i)
			{
				std::smatch match = *i;
				if (match.size() == 3)
					Headers[match[1].str()] = match[2].str();
			}

			Headers["Set-Cookies"] = GetCookieStr();

			return Headers;
		}

		std::string GetLocal()
		{
			return GetResponseHeaderValue(HTTP_QUERY_LOCATION);
		}

		DWORD GetStatusCode()
		{
			DWORD dwStatusCode;
			DWORD dwSize = sizeof(DWORD);
			HttpQueryInfoA(m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwSize, NULL);
			return dwStatusCode;
		}

		std::string GetCookieStr()
		{
			Cookies result = GetCookies();

			std::string cookies;
			for (auto &kv : result)
			{
				if (kv.second == "-" || kv.second == "''")
					continue;
				if (cookies.empty())
					cookies.append(kv.first).append("=").append(kv.second);
				else
					cookies.append("; ").append(kv.first).append("=").append(kv.second);
			}

			return cookies;
		}

		Cookies GetCookies()
		{
			Cookies result;

			std::string strHeaders = GetResponseHeaders();
			std::string cookies_pattern("Set-Cookie: ([^\\r|^\\n]+)");

			std::regex cookies_regex(cookies_pattern);
			std::smatch results;

			auto cookies_begin = std::sregex_iterator(strHeaders.begin(), strHeaders.end(), cookies_regex);
			auto cookies_end = std::sregex_iterator();

			for (auto i = cookies_begin; i != cookies_end; ++i)
			{
				std::smatch match = *i;
				Cookies cookies = ParseCookies(match.str());
				for (auto &kv : cookies)
					result[kv.first] = kv.second;
			}

			return result;
		}

	};
}

#endif // _WINNET_HPP_INCLUDED_
