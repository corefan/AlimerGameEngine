/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefPtr.h"

namespace Alimer
{
	/// Pointer which holds a weak reference to a RefCounted subclass. Can track destruction but does not keep the object alive.
	template <class T> class WeakPtr
	{
	public:
		/// Construct a null pointer.
		WeakPtr()
			: ptr(nullptr)
			, refCount(nullptr)
		{
		}

		/// Copy-construct.
		WeakPtr(const WeakPtr<T>& ptr_) :
			ptr(nullptr),
			refCount(nullptr)
		{
			*this = ptr_;
		}

		/// Construct from a ref pointer.
		WeakPtr(const RefPtr<T>& ptr_) :
			ptr(nullptr),
			refCount(nullptr)
		{
			*this = ptr_;
		}

		/// Construct from a raw pointer.
		WeakPtr(T* ptr_) :
			ptr(nullptr),
			refCount(nullptr)
		{
			*this = ptr_;
		}

		WeakPtr(std::nullptr_t)
			: ptr(0)
			, refCount(0)
		{
		}

		/// Destruct. Release the object reference.
		~WeakPtr()
		{
			Reset();
		}

		/// Assign another weak pointer.
		WeakPtr<T>& operator = (const WeakPtr<T>& rhs) {
			if (Get() == rhs)
				return *this;

			Reset();
			ptr = rhs.ptr;
			refCount = rhs.refCount;
			if (refCount)
				++(refCount->weakRefs);
			return *this;
		}

		/// Assign a ref pointer.
		WeakPtr<T>& operator = (const RefPtr<T>& rhs) {
			if (*this == rhs)
				return *this;

			Reset();
			ptr = rhs.Get();
			refCount = ptr ? ptr->RefCountPtr() : nullptr;
			if (refCount)
				++(refCount->weakRefs);
			return *this;
		}

		/// Assign a raw pointer.
		WeakPtr<T>& operator = (T* rhs)
		{
			if (*this == rhs)
				return *this;

			Reset();
			ptr = rhs;
			refCount = ptr ? ptr->RefCountPtr() : nullptr;
			if (refCount)
				++(refCount->weakRefs);
			return *this;
		}

		/// Release the weak object reference and reset to null.
		void Reset()
		{
			if (refCount)
			{
				--(refCount->weakRefs);
				// If expired and no more weak references, destroy the reference count
				if (refCount->expired && refCount->weakRefs == 0)
					delete refCount;
				ptr = nullptr;
				refCount = nullptr;
			}
		}

		/// Perform a static cast from a weak pointer of another type.
		template <class U> void StaticCast(const WeakPtr<U>& rhs)
		{
			*this = static_cast<T*>(rhs.ptr);
		}

		/// Perform a dynamic cast from a weak pointer of another type.
		template <class U> void DynamicCast(const WeakPtr<U>& rhs)
		{
			Reset();
			T* rhsObject = dynamic_cast<T*>(rhs.ptr);
			if (rhsObject)
				*this = rhsObject;
		}

		/// Test for equality with another weak pointer.
		bool operator == (const WeakPtr<T>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
		/// Test for equality with a ref pointer.
		bool operator == (const RefPtr<T>& rhs) const { return ptr == rhs.Get(); }
		/// Test for equality with a raw pointer.
		bool operator == (T* rhs) const { return ptr == rhs; }
		/// Test for inequality with another weak pointer.
		bool operator != (const WeakPtr<T>& rhs) const { return !(*this == rhs); }
		/// Test for inequality with a ref pointer.
		bool operator != (const RefPtr<T>& rhs) const { return !(*this == rhs); }
		/// Test for inequality with a raw pointer.
		bool operator != (T* rhs) const { return !(*this == rhs); }
		/// Point to the object.
		T* operator -> () const { T* ret = Get(); ALIMER_ASSERT(ret); return ret; }
		/// Dereference the object.
		T& operator * () const { T* ret = Get(); ALIMER_ASSERT(ret); return ret; }
		/// Convert to the object.
		operator T* () const { return Get(); }

		/// Return the object or null if it has been destroyed.
		T* Get() const
		{
			if (refCount && !refCount->expired)
				return ptr;
			else
				return nullptr;
		}

		/// Return the number of strong references.
		uint32_t Refs() const { return refCount ? refCount->refs : 0; }
		/// Return the number of weak references.
		uint32_t WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
		/// Return whether is a null pointer.
		bool IsNull() const { return ptr == nullptr; }
		/// Return whether the object has been destroyed. Returns false if is a null pointer.
		bool IsExpired() const { return refCount && refCount->expired; }

	private:
		/// %Object pointer.
		T* ptr;
		/// The object's weak reference count structure.
		RefCount* refCount;
	};

	/// Perform a static cast between weak pointers of two types.
	template <class T, class U> WeakPtr<T> StaticCast(const WeakPtr<U>& rhs)
	{
		WeakPtr<T> ret;
		ret.StaticCast(rhs);
		return ret;
	}

	/// Perform a dynamic cast between weak pointers of two types.
	template <class T, class U> WeakPtr<T> DynamicCast(const WeakPtr<U>& rhs)
	{
		WeakPtr<T> ret;
		ret.DynamicCast(rhs);
		return ret;
	}
}