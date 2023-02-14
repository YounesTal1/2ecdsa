#include <stdlib.h>
#include "ECpoint.h"
#include "Curve.h"

#include "Sha256.h"

#include "NIZKP.h"
#include "Tools.h"
#include <secp256k1.h>
#include <string>

using namespace std;


NIZKP::NIZKP(){}

NIZKP::NIZKP(Fpoint x, ECpoint Q)
{
	Fpoint random;

    for (int i = 0; i < 32; i++) 
	{
    	random.m[i] = (unsigned char)rand();
    }

    size_t pub_str_len = 65;
	unsigned char pub_str[pub_str_len];



	this->U = ECpoint::Multiply_Gen(random);
    secp256k1_ec_pubkey_serialize(ctx, pub_str, &pub_str_len, &(this->U.P), SECP256K1_EC_UNCOMPRESSED);


	string concatenation = concat(Key_To_str(&Q.P), Key_To_str(&(this->U.P)));

	Fpoint c(Sha256_char(concatenation));
    this->c = c;

	Fpoint z;
	z = Fpoint::Multiply(c , x);
	z = Fpoint::Add(random ,z);

    this->z = z;

	unsigned char *temp = concat_char(To_char(&(this->U.P)), this->c.m,  this->z.m);
	std::memcpy(this->proof, temp, 129);
	

}

void NIZKP::Verify(const ECpoint Q)
{
	string concatenation;
    concatenation = concat(Key_To_str( const_cast<secp256k1_pubkey*>(&Q.P)), Key_To_str(&(this->U.P)));
	Fpoint c(Sha256_char(concatenation));


	for (int i = 0; i < 32; i++) 
    {
		if(this->c.m[i] != c.m[i])
		{
			cout << "first check in NIZKP fails" << endl;
			abort();
		}
    }

	ECpoint Temp1, Temp2;
	Temp1 =  ECpoint::Multiply_Gen(z);


	Temp2 = ECpoint::Multiply(this->c, Q);
	Temp2 = ECpoint::Add(this->U, Temp2);

		string TT1 =  Key_To_str(&Temp1.P) ;
		string TT2 =  Key_To_str(&Temp2.P) ;


		if (TT1 != TT2)
		{
			cout << "Second check in NIZKP fails" << endl;
			abort();
		}

		cout << "Check passed for NIZKP" << endl;

}

void Verify(const ECpoint Q, const unsigned char* proof)
{

	Fpoint c,z;
	secp256k1_pubkey UP ;

	unsigned char input[65];
	
	for(int i = 0	; i < 65; i++)
	{
		input[i] = proof[i];
	}
	for(int i = 0	; i < 32; i++)
	{

		c.m[i] = proof[i+65];
		z.m[i] = proof[i+97];
	}


	if (!secp256k1_ec_pubkey_parse(ctx, &UP, input, 65))
	{
    	cout << "Fail parsing" << endl;
		abort();
	}

	ECpoint U(UP);
	Fpoint cc(Sha256_char(concat(Key_To_str( const_cast<secp256k1_pubkey*>(&Q.P)), Key_To_str(&UP))));
	for (int i = 0; i < 32; i++) 
    {
		if(c.m[i] != cc.m[i])
		{
			cout << "first check in NIZKP fails" << endl;
			abort();
		}
    }

	ECpoint Temp1, Temp2;
	Temp1 = ECpoint::Multiply_Gen(z);
	Temp2 = ECpoint::Multiply(c, Q);
	Temp2 = ECpoint::Add(U, Temp2);

	for (int i = 0; i < 32; i++) 
	{
		if(Key_To_str(&Temp1.P) != Key_To_str(&Temp2.P))
		{
			cout << "Second check in NIZKP fails" << endl;
			abort();
		}
		//cout << "Check passed for NIZKP" << endl;

    }

}


