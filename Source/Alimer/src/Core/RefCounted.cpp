/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Core/RefCounted.h"

namespace Alimer
{
	RefCounted::RefCounted()
	{

	}

	RefCounted::~RefCounted()
	{
		if (_refCount)
		{
			ALIMER_ASSERT(_refCount->refs == 0);
			if (_refCount->weakRefs == 0)
			{
				delete _refCount;
			}
			else
			{
				_refCount->expired = true;
			}
		}
	}

	void RefCounted::AddReference()
	{
		if (!_refCount)
			_refCount = new RefCount();

		++(_refCount->refs);
	}

	void RefCounted::ReleaseReference()
	{
		ALIMER_ASSERT(_refCount && _refCount->refs > 0);
		--(_refCount->refs);
		if (_refCount->refs == 0) {
			delete this;
		}
	}

	RefCount* RefCounted::RefCountPtr()
	{
		if (!_refCount)
			_refCount = new RefCount();

		return _refCount;
	}
}