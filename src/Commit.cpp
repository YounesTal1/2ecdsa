#include "ECpoint.h"
#include "Fpoint.h"
#include "Curve.h"
#include <stdlib.h>

#include "Sha256.h"
#include "Tools.h"
#include "Commit.h"


Commit::Commit()
{
	this->message = "";
}

Commit::Commit(ECpoint U, unsigned char* proof) {
	Fpoint random;

	//Using randomness of size 256 bits
    for (int i = 0; i < 32; i++)
    {
        random.m[i] = (unsigned char)rand();
    }
	string s = concat(Key_To_str(&U.P), To_str_proof(proof));
	

	//To hash here message + randomness

	this-> message = s;
	Fpoint temp(Sha256_char(concat(To_str(random.m),s)));
    this->commitment = temp;
	this->rd = random;
}


Commit::Commit(int i, ECpoint U, unsigned char* proof) {
	Fpoint random;
    for (int j = 0; j < 32; j++)
    {
        random.m[j] = (unsigned char)rand();
    }


	string s = concat(i, Key_To_str(&U.P), To_str_proof(proof));
	

	//To hash here message + randomness

	this-> message = s;
	Fpoint temp(Sha256_char(concat(To_str(random.m),s)));
    this->commitment = temp;
	this->rd = random;


}

Commit& Commit::operator=(const Commit &rhs)
{
                        if (this != &rhs)
                        {
                                this->message = rhs.message;
                                this->commitment = rhs.commitment;
                                this->rd = rhs.rd;
                        }
                        return *this;
}



string Commit::Open()
{
	return this->message;
}

void Verify(const Fpoint random, const string message, const Fpoint comm)
{

	Fpoint commt(Sha256_char(concat(To_str( const_cast<unsigned char*>(random.m)),message)));	
	for (int i = 0; i < 32; i++)
    {
        if(comm.m[i] != commt.m[i])
        {
            cout << "Checking committment failed" << endl;
            abort();
        }
    }
	//cout << "Commitment check passes"<< endl;
}





