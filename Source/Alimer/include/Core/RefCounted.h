/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
	/**
	* Reference count structure.
	* Used in both intrusive and non-intrusive reference counting.
	*/
	struct ALIMER_API RefCount
	{
		/// Construct with zero refcounts.
		RefCount()
			: refs(0)
			, weakRefs(0)
			, expired(false)
		{
		}

		/// Reference count. If below zero, the object has been destroyed.
		uint32_t refs;
		/// Weak reference count.
		uint32_t weakRefs;
		/// Expired flag. The object is no longer safe to access after this is set true.
		bool expired;
	};

	/**
	* Base class for objects that require lifecycle management.
	* These are noncopyable and non-assignable.
	*/
	class ALIMER_API RefCounted
	{
	public:
		/**
		* Constructor.
		*/
		RefCounted();

		/**
		* Destructor.
		*/
		virtual ~RefCounted();

		/**
		* Increments the reference count of this object.
		*
		* Call ReleaseReference to to decrement the reference count.
		*/
		void AddReference();

		/**
		* Decrements the reference count of this object and delete self if no more references.
		*
		* Call AddReference to to increment the reference count.
		*/
		void ReleaseReference();

		/**
		* Returns the current reference count of this object.
		*
		* @return This object's reference count.
		*/
		uint32_t Refs() const { return _refCount ? _refCount->refs : 0; }

		/**
		* Returns the current weak reference count of this object.
		*
		* @return This object's reference count.
		*/
		uint32_t WeakRefs()  const { return _refCount ? _refCount->weakRefs : 0; }

		/**
		* Return pointer to the reference count structure. Allocate if not allocated yet.
		*
		* @return This object's RefCount structure.
		*/
		RefCount* RefCountPtr();

	private:
		DISALLOW_COPY_AND_ASSIGN(RefCounted);

		/// Pointer to the reference count structure.
		RefCount* _refCount = nullptr;
	};
}