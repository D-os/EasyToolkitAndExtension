/* --------------------------------------------------------------------------
 *
 * ETK++ --- The Easy Toolkit for C++ programing
 * Copyright (C) 2004-2006, Anthony Lee, All Rights Reserved
 *
 * ETK++ library is a freeware; it may be used and distributed according to
 * the terms of The MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * File: NetAddress.cpp
 *
 * --------------------------------------------------------------------------*/

#ifndef _WIN32
#	include <netdb.h>
#	include <netinet/in.h>

#	ifdef __BEOS__
#		include <BeBuild.h>
#	endif

#	if !(defined(__BEOS__) && B_BEOS_VERSION < 0x0510)
#		include <arpa/inet.h>
#	endif
#else
#	include <winsock2.h>
#endif

#include "NetAddress.h"


ENetAddress::ENetAddress(const char *hostname, euint16 port)
	: EArchivable(), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	SetTo(hostname, port);
}


ENetAddress::ENetAddress(const char *hostname, const char *protocol, const char *service)
	: EArchivable(), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	SetTo(hostname, protocol, service);
}


ENetAddress::ENetAddress(const struct sockaddr_in &sa)
	: EArchivable(), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	SetTo(sa);
}


ENetAddress::ENetAddress(const struct in_addr addr, euint16 port)
	: EArchivable(), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	SetTo(addr, port);
}


ENetAddress::ENetAddress(euint32 addr, euint16 port)
	: EArchivable(), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	SetTo(addr, port);
}


ENetAddress::ENetAddress(const ENetAddress &from)
	: EArchivable(), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	ENetAddress::operator=(from);
}


ENetAddress::~ENetAddress()
{
}


ENetAddress::ENetAddress(EMessage *from)
	: EArchivable(from), fStatus(E_NO_INIT)
{
	bzero(&fAddr, sizeof(struct sockaddr_in));
	// TODO
}


e_status_t
ENetAddress::Archive(EMessage *into, bool deep) const
{
	if(!into) return E_ERROR;

	EArchivable::Archive(into, deep);
	into->AddString("class", "ENetAddress");

	// TODO

	return E_OK;
}


EArchivable*
ENetAddress::Instantiate(EMessage *from)
{
	if(e_validate_instantiation(from, "ENetAddress"))
		return new ENetAddress(from);
	return NULL;
}


e_status_t
ENetAddress::InitCheck() const
{
	return fStatus;
}


ENetAddress&
ENetAddress::operator=(const ENetAddress &addr)
{
	fStatus = addr.fStatus;
	memcpy(&fAddr, &addr.fAddr, sizeof(struct sockaddr_in));
	return *this;
}


e_status_t
ENetAddress::SetTo(const char *hostname, euint16 port)
{
	if(hostname == NULL) return E_ERROR;

	struct hostent *ent = gethostbyname(hostname);
	if(ent == NULL) return E_ERROR;

	e_status_t retVal = E_ERROR;

	switch(ent->h_addrtype)
	{
		case AF_INET:
			fAddr.sin_addr.s_addr = inet_addr(ent->h_addr);
			fAddr.sin_family = AF_INET;
			fAddr.sin_port = htons(port);
			retVal = fStatus = E_OK;
			break;

		default:
			ETK_DEBUG("[NET]: %s --- unknown address type.", __PRETTY_FUNCTION__);
			break;
	}

	return retVal;
}


e_status_t
ENetAddress::SetTo(const char *hostname, const char *protocol, const char *service)
{
	if(hostname == NULL) return E_ERROR;

	struct servent *ent = getservbyname(service, protocol);
	if(ent == NULL) return E_ERROR;

	return SetTo(hostname, ntohs(ent->s_port));
}


e_status_t
ENetAddress::SetTo(const struct sockaddr_in &sa)
{
	if(sa.sin_family != AF_INET)
	{
		// TODO
		return E_ERROR;
	}

	memcpy(&fAddr, &sa, sizeof(struct sockaddr_in));
	return(fStatus = E_OK);
}


e_status_t
ENetAddress::SetTo(const struct in_addr addr, euint16 port)
{
	fAddr.sin_family = AF_INET;
	fAddr.sin_port = htons(port);
	memcpy(&fAddr.sin_addr, &addr, sizeof(struct in_addr));
	return(fStatus = E_OK);
}


e_status_t
ENetAddress::SetTo(euint32 addr, euint16 port)
{
	fAddr.sin_family = AF_INET;
	fAddr.sin_port = htons(port);
	fAddr.sin_addr.s_addr = addr;
	return(fStatus = E_OK);
}


e_status_t
ENetAddress::GetAddr(char *hostname, size_t hostname_len, euint16 *port)
{
	if(fStatus != E_OK) return E_ERROR;
	if(!(hostname == NULL || hostname_len == 0))
	{
		struct hostent *ent = gethostbyaddr((const char*)&fAddr.sin_addr, sizeof(struct in_addr), AF_INET);
		if(ent == NULL) return E_ERROR;

		if(hostname_len > 1)
		{
			hostname_len = min_c(hostname_len, strlen(ent->h_name) + 1);
			memcpy(hostname, ent->h_name, hostname_len - 1);
		}

		*(hostname + hostname_len - 1) = 0;
	}
	if(port) *port = ntohs(fAddr.sin_port);
	return E_OK;
}


e_status_t
ENetAddress::GetAddr(struct sockaddr_in &sa)
{
	if(fStatus != E_OK) return E_ERROR;
	memcpy(&sa, &fAddr, sizeof(struct sockaddr_in));
	return E_OK;
}


e_status_t
ENetAddress::GetAddr(struct in_addr &addr, euint16 *port)
{
	if(fStatus != E_OK) return E_ERROR;
	memcpy(&addr, &fAddr.sin_addr, sizeof(struct in_addr));
	if(port) *port = ntohs(fAddr.sin_port);
	return E_OK;
}

