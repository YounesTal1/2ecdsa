#ifndef _Fpoint_h
#define _Fpoint_h


class Fpoint{
	public:

		unsigned char m[32];

		
		Fpoint(unsigned char *m);
		Fpoint();

		Fpoint& operator=(const Fpoint &rhs);

		//To make these constants later on
		static Fpoint Add(const Fpoint a, const Fpoint b);
		static Fpoint Multiply(const Fpoint a, const Fpoint b);
		static Fpoint Invert(const Fpoint a);
};


#endif
