#pragma once

#ifndef GUARD_Ptr_h
#define GUARD_Ptr_h

#include <cstddef>
#include <stdexcept>

template<class T> 
T* clone(const T* tp) {
	return tp->clone();
}

template <class T> 
class Ptr {
public:
	Ptr() : p(0), refptr(new std::size_t(1)) { }
	Ptr(T* t) : p(t), refptr(new std::size_t(1)) { }
	Ptr(const Ptr& h) : p(h.p), refptr(h.refptr) { ++*refptr; }
	
	Ptr& operator=(const Ptr& rhs) {
		++*rhs.refptr;
		if (--*refptr == 0) {
			delete refptr;
			delete p;
		}
		refptr = rhs.refptr;
		p = rhs.p;
		return *this;
	}

	~Ptr() {
		if (--*refptr == 0) {
			delete refptr;
			delete p;
		}
	}

	operator bool() const { return p; }

	T& operator*() const {
		if (p) return *p;
		throw std::runtime_error("unbound Ptr");
	}

	T* operator->() const {
		if (p) return p;
		throw std::runtime_error("unbound Ptr");
	}

	void make_unique() {
		if (*refptr != 1) {
			--*refptr;
			refptr = new std::size_t(1);
			p = p ? clone(p) : 0;
		}
	}

private:
	T* p;
	std::size_t* refptr;
};

#endif