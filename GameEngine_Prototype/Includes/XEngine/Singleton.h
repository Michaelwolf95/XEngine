#pragma once
// Singleton Base Class
// Based on the following StackOverflow discussions:
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289#1008289
//https://stackoverflow.com/questions/41328038/singleton-template-as-base-class-in-c
// 
template<typename T>
class Singleton
{
public:
	static T& getInstance() // Type& is a REFERENCE; an alias for the variable.
	{
		static T instance;
		return instance;
	}
protected:
	Singleton() {}
	~Singleton() {}
public:
	Singleton(Singleton const &) = delete;
	Singleton& operator=(Singleton const &) = delete;
};
