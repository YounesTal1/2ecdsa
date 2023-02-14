#include <stdlib.h>
#include "ECpoint.h"
#include "KeyPair.h"

using namespace std;

KeyPair::KeyPair()
{
	Fpoint random;

    for (int i = 0; i < 32; i++)
    {
        random.m[i] = (unsigned char)rand();
    }
	
	this->sk = random;
	this->Pk = ECpoint::Multiply_Gen(random);

}



KeyPair& KeyPair::operator=(const KeyPair &rhs)
{
                        if (this != &rhs)
                        {

                                this->sk = rhs.sk;
                                this->Pk = rhs.Pk;
                        }
                        return *this;
}

