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

	template <class T>
	class RefPtr {
	public:
		ALIMER_FORCE_INLINE RefPtr() : ptr_(nullptr) {
		}

		ALIMER_FORCE_INLINE RefPtr(std::nullptr_t) : ptr_(nullptr) {
		}

		ALIMER_FORCE_INLINE RefPtr(T* ptr) : ptr_(ptr) {
			InternalAddRef();
		}

		inline RefPtr(const RefPtr<T>& r) : ptr_(r.ptr_) {
			InternalAddRef();
		}

		/** Takes-over the object from another pointer. */
		inline RefPtr(RefPtr&& other) : ptr_(other.ptr_) {
			other.ptr_ = nullptr;
		}

		/**
		* Copies another pointer.
		* This will increment the object's reference-count (if it is non-null).
		*/
		template <class U>
		inline RefPtr(const RefPtr<U>& other) : ptr_(static_cast <T*> (other.Get())) {
			InternalAddRef();
		}

		~RefPtr() {
			Reset();
		}

		operator T*() const { return ptr_; }
		T* operator->() const { return ptr_; }

		explicit operator bool() const {
			return (this->Get() != 0);
		}

		RefPtr<T>& operator=(T* rhs) {
			if (Get() == rhs)
				return *this;

			Reset();
			ptr_ = rhs;
			if (ptr_) {
				ptr_->AddReference();
			}

			return *this;
		}

		RefPtr<T>& operator=(const RefPtr<T>& rhs) {
			if (*this == rhs)
				return *this;

			Reset();
			ptr_ = rhs.ptr_;
			if (ptr_) {
				ptr_->AddReference();
			}

			return *this;
		}

		/**
		* Release the object reference and reset to null.
		* Destroy the object if was the last reference.
		*/
		void Reset()
		{
			if (ptr_)
			{
				ptr_->ReleaseReference();
				ptr_ = nullptr;
			}
		}

		/**
		* Changes this pointer to point at a different object.
		* The reference count of the old object is decremented, and it might be
		* deleted if it hits zero. The new object's count is incremented.
		*/
		template <class U>
		RefPtr& operator= (const RefPtr<U>& other) {
			return operator= (static_cast <T*> (other.Get()));
		}

		RefPtr& operator= (RefPtr&& other) {
			std::swap(ptr_, other.ptr_);
			return *this;
		}

		/// Perform a static cast from a ref pointer of another type.
		template <class U> void StaticCast(const RefPtr<U>& rhs) {
			InternalRelease();
			ptr_ = static_cast<T*>(rhs.Get());
			InternalAddRef();
		}

		/// Perform a dynamic cast from a ref pointer of another type.
		template <class U> void DynamicCast(const RefPtr<U>& rhs) {
			InternalRelease();
			ptr_ = dynamic_cast<T*>(rhs.Get());
			InternalAddRef();
		}

		void Swap(T** pp) {
			T* p = ptr_;
			ptr_ = *pp;
			*pp = p;
		}

		void Swap(RefPtr<T>& r) {
			Swap(&r.ptr_); 
		}

		ALIMER_FORCE_INLINE void Attach(T* other) {
			if (ptr_ != nullptr) {
				ptr_->ReleaseReference();
				// Attaching to the same object only works if duplicate references are being coalesced. Otherwise
				// re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
				ALIMER_ASSERT(ptr_ != other);
			}

			ptr_ = other;
		}

		T* Detach() {
			T* ptr = ptr_;
			ptr_ = nullptr;
			return ptr;
		}

		/// Return the object.
		T* Get() const { return ptr_; }

		/// Return the number of strong references.
		uint32_t Refs() const {
			return ptr_ ? ptr_->Refs() : 0;
		}

		/// Return the number of weak references.
		uint32_t WeakRefs() const {
			return ptr_ ? ptr_->WeakRefs() : 0;
		}

		/// Return whether is a null pointer.
		bool IsNull() const { return ptr_ == nullptr; }

		T* const* GetAddressOf() const {
			return &ptr_;
		}

		T** GetAddressOf() {
			return &ptr_;
		}

		T** ReleaseAndGetAddressOf() {
			InternalRelease();
			return &ptr_;
		}

	private:
		/// Add a reference to the object pointed to.
		void InternalAddRef() const {
			if (ptr_ != nullptr) {
				ptr_->AddReference();
			}
		}

		/// Release the object reference and delete it if necessary.
		void InternalRelease() {
			T* temp = ptr_;

			if (temp != nullptr) {
				ptr_ = nullptr;
				temp->ReleaseReference();
			}
		}

		T* ptr_;
	};

	/// Perform a static cast from one ref pointer type to another.
	template <class T, class U> RefPtr<T> StaticCast(const RefPtr<U>& ptr)
	{
		RefPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a dynamic cast from one weak pointer type to another.
	template <class T, class U> RefPtr<T> DynamicCast(const RefPtr<U>& ptr)
	{
		RefPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}
}