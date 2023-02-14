#ifndef _ECpoint_h
#define _ECpoint_h

#include <secp256k1.h>
#include "Fpoint.h"

using namespace std;
class ECpoint{
	public:

		secp256k1_pubkey P;

		
		ECpoint(secp256k1_pubkey pub);
		ECpoint();

		ECpoint& operator=(const ECpoint &rhs);

		static ECpoint Add(const ECpoint Q, const ECpoint R);
		static ECpoint Multiply(const Fpoint a, const ECpoint Q);
		static ECpoint Multiply_Gen(const Fpoint a);
};

Fpoint xcoord(const ECpoint Q);
#endif
