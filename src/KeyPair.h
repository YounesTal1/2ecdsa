#ifndef _KeyPair_h
#define _KeyPair_h


#include "ECpoint.h"
#include "Fpoint.h"

class KeyPair{
	public:
		Fpoint sk;
		ECpoint Pk;


		KeyPair();


		KeyPair& operator=(const KeyPair &rhs);

};


#endif
