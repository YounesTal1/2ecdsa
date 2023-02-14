#ifndef _Commit_h
#define _Commit_h

#include "ECpoint.h"
#include "Fpoint.h"
#include <string>

using namespace std;

class Commit{

	public:
		string message;
		Fpoint commitment;
		Fpoint rd;

		Commit();

		//To commit to Q||s
		Commit(ECpoint Q, unsigned char* s);
		//To commit to i||Q||s
		Commit(int i, ECpoint Q, unsigned char* s);
		
		Commit& operator=(const Commit &rhs);
		string Open();


};

void Verify(const Fpoint rd, const string message, const Fpoint commitment);

#endif
