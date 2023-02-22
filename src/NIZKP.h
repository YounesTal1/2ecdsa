#ifndef _NIZKP_h
#define _NIZKP_h

#include "ECpoint.h"

/*Using NIZKP of Schnorr. 
'https://crypto.stanford.edu/cs355/19sp/lec5.pdf' For details about it.
*/ 
using namespace std;

class NIZKP{

	public:
		ECpoint U;
		Fpoint c;
		Fpoint z;
		//129 as the proof is the concatenation of an elliptic curve point in the uncompressed form and two field elements
		unsigned char proof[129];

		NIZKP(Fpoint x, ECpoint Q);
		NIZKP();

		void Verify(const ECpoint Q);


};


void Verify(const ECpoint Q, const unsigned char* proof);

#endif
