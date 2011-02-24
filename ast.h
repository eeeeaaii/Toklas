#pragma once
#include "symboltable.h"


// forward declarations


//////////////////// general

class ast
{
	protected:
		string indent(int lvl);
		string debugoptype(int optype);
	public:
		ast()
			{ }
		virtual void debug(int ind)
			{ }
		virtual void semanticCheck()
			{ }
		virtual void createTypes()
			{ }
		virtual registerid generateCode(operation* &p)
			{ return 0; }
		virtual ~ast()
			{ }
};

template <class T>
class astnodelist
{
	private:
		typename std::vector<T> contents;
		typename std::vector<T>::iterator it;

	public:

		void add(T newx)
			{ contents.push_back(newx); }

		void begin()
			{ it = contents.begin(); }

		bool atEnd()
			{ return it == contents.end(); }

		T getCurrent()
			{ return (*it); }

		void gotoNext()
			{ it++; }

		void debug(int ind);

		virtual ~astnodelist();
};

template <class T>
void astnodelist<T>::debug(int ind)
{
	typename std::vector<T>::iterator i;
	for (i = contents.begin() ; i != contents.end() ; i++) {
		ast* p = *i;
		p->debug(ind);
	}
}

template <class T>
astnodelist<T>::~astnodelist()
{
	typename std::vector<T>::iterator i;
	for (i = contents.begin() ; i != contents.end() ; i++) {
		ast* p = *i;
		delete p; // virtual destructor will ensure the correct stuff gets done
	}
}
