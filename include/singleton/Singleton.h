///
/// The helper class for singleton pattern in paper library
///

#pragma once

template <class Type> Type &PGetSingleton() {
	static Type instance;
	return instance;
}