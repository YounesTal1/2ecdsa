#include "ECpoint.h"
#include <secp256k1.h>
#include "Fpoint.h"
#include "Curve.h"
#include <iostream>

using namespace std;


ECpoint::ECpoint(){}


ECpoint::ECpoint(secp256k1_pubkey P)
{
	this->P = P;
}


ECpoint& ECpoint::operator=(const ECpoint &rhs)
{
                        if (this != &rhs)
                        {

                                this->P = rhs.P;
                        }
                        return *this;

}
ECpoint ECpoint::Add(const ECpoint Q, const ECpoint R)
{
	secp256k1_pubkey* pubkeys[2];
	secp256k1_pubkey  result;
    size_t num_pubkeys = 2;



    pubkeys[0] =  const_cast<secp256k1_pubkey*> (&Q.P);
    pubkeys[1] =  const_cast<secp256k1_pubkey*> (&R.P);
    if (secp256k1_ec_pubkey_combine(ctx, &result, pubkeys, num_pubkeys) != 1) {
        printf("Failed to add points\n");
    }
	ECpoint Res(result);
	return Res;


}

ECpoint ECpoint::Multiply(const Fpoint a, const ECpoint Q)
{
	int ret = secp256k1_ec_pubkey_tweak_mul(ctx, const_cast<secp256k1_pubkey*>(&Q.P), a.m);
	if (ret == 0)
    {
        cout << "abort" << endl;
        abort();
    }

	ECpoint Res(Q.P);
	return Res;
	
}	


ECpoint ECpoint::Multiply_Gen(const Fpoint a)
{

	ECpoint Temp;

    int ret = secp256k1_ec_pubkey_create(ctx, &(Temp.P), a.m);
    if (ret == 0)
    {
        cout << "abort" << endl;
        abort();
    }

	return Temp;
	
}	

Fpoint xcoord(const ECpoint Q)
{
	//65 as we are using points in the non-compressed form
    size_t pub_str_len = 65;
    unsigned char* pub_str = NULL;
    pub_str = new unsigned char[pub_str_len];


    secp256k1_ec_pubkey_serialize(ctx, pub_str, &pub_str_len, const_cast<secp256k1_pubkey*>(&Q.P), SECP256K1_EC_UNCOMPRESSED);
    unsigned char ttemp[32];
    for(int j= 0; j < 32; j++)
    {
    	ttemp[j] = pub_str[j+1];
    }
	
    Fpoint rr(ttemp);
	
	return rr;
}
