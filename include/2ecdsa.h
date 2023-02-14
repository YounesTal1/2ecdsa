/*
Notation follows this logic:
P1Key : Party 1's share of the key
P2Key : Party 2's share of the key
P12Key : full key combined by Party 1
P21Key : full key combined by Party 2 */


//Usage of a dummy MtA, that is one party takes two shares z_1 and z_2, and generates random a and b subject to z_1*z_2 = a+b

#include <stdlib.h>
#include "Commit.h"
#include "KeyPair.h"

#include "NIZKP.h"
#include "Tools.h"

#include "ECpoint.h"
#include "Sha256.h"
#include <gmpxx.h>
#include <string>

#include <vector>
#include "Curve.h" 

#include <ctime>

using namespace std;


void Verify(ECpoint KeyPk, vector<Fpoint> msgs, vector<Fpoint> r, vector<Fpoint> s)
{
    Fpoint hash;
    ECpoint R, R1, R2;
	Fpoint zz, vv, rr;
	

    for (vector<Fpoint>::size_type ind = 0; ind < msgs.size(); ind++)
    {


        zz = Fpoint::Invert(s[ind]);
        Fpoint hash(Sha256_char(To_str(msgs[ind].m)));
		vv = Fpoint::Multiply(zz , hash);
		R1 = ECpoint::Multiply_Gen(vv);

        R2 = ECpoint::Multiply(Fpoint::Multiply(zz , r[ind]) , KeyPk);
        R = ECpoint::Add(R1,R2);

		rr = xcoord(R);


		for (int i = 0; i < 32; i++)
    	{

        	if(r[ind].m[i] != rr.m[i])
        	{
            	cout << "Invalid signature" << endl;
            	abort();
        	}
    	}

    }

    //cout << "All signatures are valid" << endl;
}



void P1KeyGenPass1(KeyPair *P1Key, NIZKP *P1ni, Commit *cm)
{
    KeyPair Key1;
	(*P1Key) = Key1;
	NIZKP Temp(Key1.sk, Key1.Pk);

    Commit temp(Key1.Pk, Temp.proof);
	(*cm) = temp ;
	(*P1ni) =  Temp;

}


void P1KeyGenPass2(ECpoint *P12KeyPk, unsigned char* P2proof, ECpoint P1KeyPk, ECpoint P2KeyPk)
{
	Verify(P2KeyPk, P2proof);
    (*P12KeyPk) = ECpoint::Add(P1KeyPk , P2KeyPk);



}


void P1SignPass1(int n, vector<KeyPair>& P1EphKey, vector<unsigned char*>& P1proofs, vector<Fpoint>& comms, vector<string>& messages, vector<Fpoint>& rands, vector<ECpoint>& P1EphKeyPk)
{

	unsigned char* Temp;
	for(int i = 0; i<n; i++)
	{
       	KeyPair Key;

		P1EphKey.push_back(Key);
		P1EphKeyPk.push_back(Key.Pk);
        NIZKP ni(P1EphKey[i].sk, P1EphKey[i].Pk);

		Temp = new unsigned char[129];
        for (int j=0; j<129; j++)
        {
            Temp[j] = ni.proof[j];
        }
		P1proofs.push_back(Temp);
        Commit cm(i,P1EphKey[i].Pk, ni.proof);
		comms.push_back(cm.commitment);
		messages.push_back(cm.message);
		rands.push_back(cm.rd);
	}

}



void P1SignPass2(int n, vector<KeyPair> P1EphKey, vector<ECpoint>& P12EphKeyPk, vector<ECpoint>& P2EphKeyPk, vector<unsigned char*>& P2proofs, vector<Fpoint>& rcoords)
{
	ECpoint Temp;
	Fpoint tt;
	for(int i = 0; i < n; i++)
	{
		Verify(P2EphKeyPk[i], P2proofs[i]);
		Temp = ECpoint::Multiply(P1EphKey[i].sk, P2EphKeyPk[i]);
       	P12EphKeyPk.push_back(Temp);
		tt = xcoord(Temp);
		rcoords.push_back(tt);
	}

}




void P1SignPass3(int n, vector<Fpoint>& s, vector<Fpoint> s2, vector<Fpoint> a, vector<ECpoint> P12EphKeyPk, vector<KeyPair> P1EphKey)
{
		Fpoint zz, tt;
		for(int i = 0; i < n; i++)
        {

			zz = Fpoint::Invert(P1EphKey[i].sk) ;

			Fpoint aa(a[i].m);
        	tt = xcoord(P12EphKeyPk[i]);

			aa = Fpoint::Multiply(aa,tt);
			aa = Fpoint::Add(aa,s2[i]);
			
			aa = Fpoint::Multiply(aa,zz);
	  		s.push_back(aa);

	}


}





///////////////////////////////////////////////////////////////



void P2KeyGenPass1(KeyPair *P2Key, NIZKP *P2ni)
{
    KeyPair Key2;
	(*P2Key) = Key2;
	NIZKP Temp(Key2.sk, Key2.Pk);
	(*P2ni) = Temp;

}




void P2KeyGenPass2( ECpoint *P21KeyPk, KeyPair P2Key, unsigned char* P1proof, ECpoint P1KeyPk, Fpoint comm, Fpoint random, string message)
{

	Verify(random, message, comm);
	Verify(P1KeyPk, P1proof);
    (*P21KeyPk) = ECpoint::Add(P1KeyPk, P2Key.Pk);

}





void P2SignPass1(int n, vector<KeyPair>& P2EphKey, vector<unsigned char*>& P2proofs, vector<ECpoint>& P2EphKeyPk, vector<Fpoint>& P2EphKeysk)
{
	unsigned char* Temp ;

	for(int i = 0; i<n; i++)
        {

		KeyPair Key;
		P2EphKey.push_back(Key);
		P2EphKeysk.push_back(Key.sk);
		P2EphKeyPk.push_back(Key.Pk);

		NIZKP ni(P2EphKey[i].sk, P2EphKey[i].Pk);

		Temp = new unsigned char[129];
		for (int j=0; j<129; j++)
		{	
			Temp[j] = ni.proof[j];
		}
		P2proofs.push_back(Temp);
	}

}





void P2SignPass2(int n, vector<KeyPair> P2EphKey, vector<ECpoint>& P21EphKeyPk, vector<ECpoint> P1EphKeyPk, vector<unsigned char*> P1proofs, vector<Fpoint> comms, vector<string> messages, vector<Fpoint> rands)
{

        for(int i = 0; i < n; i++)
        {

			Verify(rands[i], messages[i], comms[i]);
			Verify(P1EphKeyPk[i], P1proofs[i]);
            P21EphKeyPk.push_back(ECpoint::Multiply(P2EphKey[i].sk, P1EphKeyPk[i]));
        }



}




void MtA(int n, Fpoint P1Keysk, vector<Fpoint> P2EphKeysk, vector<Fpoint>& a, vector<Fpoint>& b)
{
	

	for(int i = 0; i < n; i++)
        {
			Fpoint bb;
            bb = Fpoint::Invert(P2EphKeysk[i]);

            Fpoint aa;
            for (int j = 0; j < 32; j++)
            {

                aa.m[j] = (unsigned char)rand();
            }
        	a.push_back(aa);
			bb = Fpoint::Multiply(bb,P1Keysk);
			int ret = secp256k1_ec_seckey_negate(ctx, aa.m);
			if(ret == 0)
			{
				cout << "abort" << endl;
        		abort();
			}

			bb = Fpoint::Add(bb,aa);

        	b.push_back(bb);

		}


}






void P2SignPass3(int n, vector<Fpoint> m, vector<Fpoint>& s2, vector<Fpoint> b, vector<ECpoint> P21EphKeyPk, vector<KeyPair> P2EphKey, KeyPair P2Key)
{
		Fpoint zz, rr;
		Fpoint tmp, tmp2;
		for(int i = 0; i < n; i++)
        {

            zz = Fpoint::Invert(P2EphKey[i].sk);

			Fpoint hash(Sha256_char(To_str(m[i].m)));
            Fpoint bb(b[i].m);
			rr = xcoord(P21EphKeyPk[i]);
            tmp2 = Fpoint::Multiply(bb,rr);

            tmp = Fpoint::Multiply(P2Key.sk,rr);


            tmp = Fpoint::Add(tmp,hash);
            tmp = Fpoint::Multiply(tmp,zz);
            tmp = Fpoint::Add(tmp,tmp2);
            s2.push_back(tmp);




	}
}

