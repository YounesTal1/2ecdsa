#include "Fpoint.h"
#include <secp256k1.h>
#include <gmpxx.h>
#include "Curve.h"
#include "Tools.h"



//////////////////////////////////////////////////////////////////////////////
//Setting the context for secp156k1 and the order of the exponent group o

secp256k1_context* set_context()
{
    secp256k1_context *tmp = NULL;
    tmp = secp256k1_context_create(
        SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);


    return tmp;
}

secp256k1_context *ctx = set_context();

mpz_class o("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141", 16);

////////////////////////////////////////////////////////////////////


Fpoint::Fpoint(){}

Fpoint::Fpoint(unsigned char *m)
{

	memcpy(this->m, m, 32);

}


Fpoint& Fpoint::operator=(const Fpoint &rhs)
{
                        if (this != &rhs)
                        {

								memcpy(this->m, rhs.m, sizeof(rhs.m));
                        }
                        return *this;

}
Fpoint Fpoint::Add( const Fpoint a, const Fpoint b)
{
	unsigned char temp[32];

	memcpy(temp, a.m, 32);

	int ret = secp256k1_ec_seckey_tweak_add(ctx, temp, b.m);
	if (ret == 0)
	{
		cout << "abort" << endl;
        abort();
	}

	Fpoint c(temp);
	return c;


}

Fpoint Fpoint::Multiply(const Fpoint a, const Fpoint b)
{
	unsigned char temp[32];
	memcpy(temp, a.m, 32);
	int ret = secp256k1_ec_seckey_tweak_mul(ctx, temp, b.m);
	if(ret == 0)
	{
		cout << "abort" << endl;
        abort();
	}
	Fpoint c(temp);
	return c;
	
}	


Fpoint Fpoint::Invert(const Fpoint a)
{
 		mpz_class secret_key;
        unsigned char temp1[32], *temp2;
		size_t count;
        mpz_import(secret_key.get_mpz_t(), 32, 1, sizeof(a.m[0]), 0, 0, a.m);
        mpz_invert(secret_key.get_mpz_t(), secret_key.get_mpz_t(), o.get_mpz_t());
		mpz_export(temp1, &count, 1, sizeof(temp1[0]), 0, 0, secret_key.get_mpz_t());
		//Added as we don't want mpz to place zeros at the end of the array when the value convererted was represented in less than 32 chars
		if (count != 32)
		{
			temp2 = moveZerosToStart(temp1, count);
		}
		else
		{
			temp2 = temp1;
		}
        Fpoint b(temp2);
		return b;
}


