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
 * File: MessageFilter.h
 * Description: Filter message before ELooper::DispatchMessage
 * Warning: ignore E_QUIT_REQUESTED & _QUIT_
 *
 * --------------------------------------------------------------------------*/

#ifndef __ETK_MESSAGE_FILTER_H__
#define __ETK_MESSAGE_FILTER_H__

#include <etk/app/Handler.h>

typedef enum e_filter_result {
	E_SKIP_MESSAGE,
	E_DISPATCH_MESSAGE
} e_filter_result;

typedef enum e_message_delivery {
	E_ANY_DELIVERY,
	E_DROPPED_DELIVERY,
	E_PROGRAMMED_DELIVERY
} e_message_delivery;

typedef enum e_message_source {
	E_ANY_SOURCE,
	E_REMOTE_SOURCE,
	E_LOCAL_SOURCE
} e_message_source;

#ifdef __cplusplus /* Just for C++ */

class EMessage;
class EMessageFilter;

typedef e_filter_result (*e_filter_hook)(EMessage *message, EHandler **target, EMessageFilter *filter);


class _IMPEXP_ETK EMessageFilter {
public:
	EMessageFilter(e_message_delivery delivery, e_message_source source,
		       euint32 command, e_filter_hook filter = NULL);
	EMessageFilter(e_message_delivery delivery, e_message_source source,
		       e_filter_hook filter = NULL);
	EMessageFilter(euint32 command, e_filter_hook filter = NULL);
	EMessageFilter(const EMessageFilter &filter);
	EMessageFilter(const EMessageFilter *filter);
	virtual ~EMessageFilter();

	EMessageFilter &operator=(const EMessageFilter &from);

	virtual e_filter_result		Filter(EMessage *message, EHandler **target);

	e_message_delivery		MessageDelivery() const;
	e_message_source		MessageSource() const;
	euint32				Command() const;
	bool				FiltersAnyCommand() const;
	ELooper				*Looper() const;

private:
	friend class ELooper;
	friend class EHandler;

	euint32 fCommand;
	bool fFiltersAny;
	e_message_delivery fDelivery;
	e_message_source fSource;
	e_filter_hook fFilterHook;

	ELooper *fLooper;
	EHandler *fHandler;

	e_filter_result doFilter(EMessage *message, EHandler **target);
};

#endif /* __cplusplus */

#endif /* __ETK_MESSAGE_FILTER_H__ */

