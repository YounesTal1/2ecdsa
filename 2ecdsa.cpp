/*
Notation follows this logic:
P1Key : Party 1's share of the key
P2Key : Party 2's share of the key
P12Key : full key combined by Party 1
P21Key : full key combined by Party 2 */

#include "2ecdsa.h"




int main(int argc, char* argv[]) 
{
	int n_sig = 10;
	int n_key = 10;


    if (argc >= 3) {
		n_sig = stoi(argv[1]);
        n_key = stoi(argv[2]);
    }
	else
	{
		cout << "Not enough arguments given, using the ones by default" << endl;
	}




	srand(time(NULL));


	double time_off = 0, time_on = 0, time_gen = 0, elapsed_time;
	clock_t start, end;	
			 
	cout << "Generating " << n_key << " key(s), and signing " << n_sig << " message(s) with each key" << endl;
			
	for(int z = 0; z < n_key; z++)
	{ 
			vector<Fpoint> comms, m, s, s1, s2, a, b, P2EphKeysk, rands, rcoords, msgs;

			vector<ECpoint> P12EphKeyPk, P1EphKeyPk, P2EphKeyPk, P21EphKeyPk;
			vector<KeyPair> P1EphKey, P2EphKey; 

			vector<string>  messages;
			vector<unsigned char*> P1proofs, P2proofs;

			KeyPair P1Key, P2Key;
			NIZKP P1ni, P2ni;
			ECpoint KeyPk, P1KeyPk, P2KeyPk, P12KeyPk, P21KeyPk;
			Commit cm;
	
		
			for(int i = 0; i<n_sig; i++)
				{
					Fpoint random;

					for (int j = 0; j < 32; j++)
					{
						random.m[j] = (unsigned char)rand();
					}

					m.push_back(random);

				}




			start = clock();
			P1KeyGenPass1(&P1Key, &P1ni, &cm);
			P2KeyGenPass1(&P2Key, &P2ni);
			P1KeyGenPass2(&P12KeyPk, P2ni.proof, P1Key.Pk, P2Key.Pk);
			P2KeyGenPass2(&P21KeyPk, P2Key, P1ni.proof, P1Key.Pk, cm.commitment, cm.rd, cm.message);
			end = clock();
			elapsed_time = double(end-start)/CLOCKS_PER_SEC;
			time_gen += elapsed_time;


			start = clock();
			P1SignPass1(n_sig, P1EphKey, P1proofs, comms, messages, rands, P1EphKeyPk);
			P2SignPass1(n_sig, P2EphKey, P2proofs, P2EphKeyPk, P2EphKeysk);
			P1SignPass2(n_sig, P1EphKey, P12EphKeyPk, P2EphKeyPk, P2proofs, rcoords);
			P2SignPass2(n_sig, P2EphKey, P21EphKeyPk, P1EphKeyPk, P1proofs, comms, messages, rands);
			end = clock();
			elapsed_time = double(end-start)/CLOCKS_PER_SEC;
			time_off += elapsed_time;


			//Note not timing MtA
			MtA(n_sig, P1Key.sk, P2EphKeysk, a, b);

			start = clock();
			P2SignPass3(n_sig, m, s2, b, P21EphKeyPk, P2EphKey, P2Key);
			P1SignPass3(n_sig, s, s2, a, P12EphKeyPk, P1EphKey);
			Verify(P12KeyPk, m, rcoords, s);
			end = clock();
			elapsed_time = double(end-start)/CLOCKS_PER_SEC;

			time_on += elapsed_time;

			for (auto it = P1proofs.begin(); it != P1proofs.end(); ++it) 
			{
				delete *it;
			}

			for (auto it = P2proofs.begin(); it != P2proofs.end(); ++it) 
			{
				delete *it;
			}




	}

	
	cout << "All siganture were valid" << endl;
	cout << "Runtimes in second:" << endl;
	cout << "Average key generation time (i.e. time needed for one key generation)                     :" << time_gen/n_key << endl;
	cout << "Average signature offline time (i.e. offline time needed for one signature excluding MtA) :" << time_off/(n_key*n_sig) << endl;
	cout << "Average signature online time  (i.e. online time needed for one signature)                :" << time_on/(n_key*n_sig) << endl;



        return 0;
}
                                                                 
