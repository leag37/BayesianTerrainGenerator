/**************************************************************************************************
* Title: SmurfSingleton.h
* Authors: Gael Huber
* Description: Interface for singleton pattern.
**************************************************************************************************/
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>

template <typename T> class Singleton {
public:
	// Constructor implemented locally
	Singleton(void) {
		// Ensure the singleton does not already exist
		assert(!singleton);
		singleton = static_cast<T*> (this);
	};

	// Deconstructor implemented locally
	~Singleton(void) {
		// Ensure the singleton exists
		assert(singleton);
		singleton = 0;
	};

	// Get the singleton instance
	static T& getSingleton(void) {
		// Return the instance if it exists
		assert(singleton);
		return *singleton;
	};

	// Get the singleton pointer
	static T* getSingletonPtr() {
		return singleton;
	};

protected:
	static T* singleton;

private:
	Singleton(const Singleton<T> &);

	// Used to internally assign singleton value to class value
	Singleton& operator=(const Singleton<T> &);
};

#endif