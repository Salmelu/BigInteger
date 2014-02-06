/* 
 * File:   BigInteger.cpp
 * Author: salmelu
 */

#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "BigInteger.hpp"

bigInt::BigInteger::BigInteger(int n) {
	if(n < 0)
	{
		this->sign = true;
		n *= -1;
	}
	else
		this->sign = false;
	
	this->nsize = 0;
	this->alloc(5);
	
	if(n >= base)
	{
		this->nsize = 2;
		this->number[1] = n/base;
		this->number[0] = n%base;
	}
	else
	{
		this->nsize = 1;
		this->number[0] = n;
	}
}

bigInt::BigInteger::BigInteger(long n) {
	if(n < 0)
	{
		this->sign = true;
		n *= -1;
	}
	else
		this->sign = false;
	
	this->nsize = 0;
	this->alloc(6);
	
	while(n > 0)
	{
		this->number[this->nsize] = n % base;
		this->nsize++;
		n /= base;
	}
}

bigInt::BigInteger::BigInteger(const std::string& s) {
	this->stringInit(s);
}

bigInt::BigInteger::BigInteger(const char* s)
{
	if(s == NULL)
	{
		this->stringInit("0");
	}
	else
	{
		std::string str(s);
		this->stringInit(str);
	}
}

bigInt::BigInteger::BigInteger(const bigInt::BigInteger &source)
{
	nsize = source.nsize;
	asize = source.asize;
	sign = source.sign;
	number = new int[asize];	
	for(int i=0;i<nsize; i++)
		number[i] = source.number[i];
}

void bigInt::BigInteger::stringInit(const std::string& s)
{
	int length = s.length();
	int start = 0;
	if(s[0] == '-') 
	{
		this->sign = true;
		start = 1;
	}
	else
		this->sign = false;
	
	this->nsize = 0;
	if((this->checkString(s)) == false || length == 0)
	{
		this->alloc(5);
	}
	else
	{
		this->alloc(length/9 + 5);
		this->nsize = length/9;
		if(length%9 != 0) this->nsize++;
		
		for(int i=0; i<this->nsize-1; i++)
		{
			this->number[i] = parseString(s.substr(length-9*(i+1), 9));
		}
		this->number[nsize-1] = parseString(s.substr(0, (length%9==0) ? 9 : length%9));
	}
}

/*
 *	Public methods for operations
 *  add, subtract, multiply, divide, modulo, compare
 */

bigInt::BigInteger bigInt::BigInteger::add(bigInt::BigInteger a) {
	/*
	 Checks signs, if negative, calls subtract instead
	 */
	if(a.sign == true)
	{
		a.sign = false;
		return this->subtract(a);
	}
	if(this->sign == true)
	{
		this->sign = false;
		this->subtract(a);
		(this->sign) ? this->sign = false : this->sign = true;
		return *this;
	}
	 /*
	  Add field after field, moving carry to more significant digits
	  */
	int length = std::max((a.nsize), this->nsize);
	if(this->asize < length + 2)
		this->alloc(asize + 5);
	
	int remain = 0;
	int i;
	for(i=0; i<a.nsize; i++)
	{
		this->number[i] += a.number[i] + remain;
		remain = this->number[i] / base;
		this->number[i] %= base;
	}
	i=a.nsize;
	/*
	 Moves carry to the higher position, until it is not null
	 */
	while(remain > 0)
	{
		this->number[i] += remain;
		remain = this->number[i] / base;
		this->number[i] %= base;
	}
	this->recount();
	
	return *this;
}

bigInt::BigInteger bigInt::BigInteger::subtract(bigInt::BigInteger s)
{
	if(s.sign == true) 
	{
		s.sign = false;
		return this->add(s);
	}
	
	int length = std::max(this->nsize, s.nsize);
	if(this->asize < length+2)
		alloc(this->asize + 5);
	
	/*
	 Checks sign, if negative, moves onto adding instead
	 */
	
	if(this->sign == true)
	{
		this->sign = false;
		this->add(s);
		this->sign = true;
	}
	else
	{
		/*
		 We are subtracting from lesser number, 
		 result is the same as subtracting the lesser from greater and changing the sign
		 */
		if(this->compare(s) == -1)
		{
			this->sign = true;
			int remain = 0;
			int i;
			for(i=0; i<this->nsize; i++)
			{
				this->number[i] = s.number[i] - this->number[i] + remain;
				remain = 0;
				if(this->number[i] < 0)
				{
					this->number[i] += base;
					remain = -1;
				}
			}
			i=this->nsize;
			while(remain < 0)
			{
				this->number[i] += remain;
				remain = 0;
				if(this->number[i] < 0)
				{
					this->number[i] += base;
					remain = -1;
				}
			}
			i = s.nsize - 1;
			while(this->number[i] == 0)
				i--;
			this->nsize = i+1;
		}
		/*
		 General subtracting
		 */
		else
		{
			int remain = 0;
			int i;
			for(i=0; i<s.nsize; i++)
			{
				this->number[i] -= s.number[i] - remain;
				remain = 0;
				if(this->number[i] < 0)
				{
					this->number[i] += base;
					remain = -1;
				}
			}
			i=s.nsize;
			while(remain < 0)
			{
				this->number[i] += remain;
				remain = 0;
				if(this->number[i] < 0)
				{
					this->number[i] += base;
					remain = -1;
				}
			}

			i = this->nsize - 1;
			while(this->number[i] == 0)
				i--;
			this->nsize = i+1;
		}
	}
	return *this;
}

bigInt::BigInteger bigInt::BigInteger::multiply(bigInt::BigInteger b)
{
	if(b == 0 || this->compare(0) == 0)
	{
		for(int i=0; i<nsize; i++)
			this->number[i] = 0;
		this->nsize=0;
		return *this;
	}
	bigInt::BigInteger r(0);
	r.alloc((b.nsize + this->nsize));
	
	/*
	 Dealing with signs
	 */
	if(this->sign == true && b.sign == true) this->sign = false;
	else if(this->sign == false && b.sign == true) this->sign = true;
	
	/*
	 Regular multiplication "digit" after digit
	 */
	int i, j=0, k;
	long p;
	while(j < b.nsize)
	{ 
		if(b.number[j] == 0) 
		{
			r.number[this->nsize+j] = 0;
			j++;
			continue;
		}
		i = k = 0;
		while(i < this->nsize)
		{
			p = (long) b.number[j] * (long) this->number[i] + r.number[i+j] + k;
			r.number[i+j] = (int) (p % base);
			k = (int) (p/base);
			i++;
		}
		r.number[this->nsize+j] = k;
		j++;
	}
	
	/*
	 Rewriting constants to update to current values
	 */
	this->asize = r.asize;
	this->nsize = b.nsize + this->nsize;
	delete [] this->number;
	this->number = r.number;
	while(this->number[this->nsize-1] == 0) this->nsize--;
	
	return *this;
}

bigInt::BigInteger bigInt::BigInteger::divide(bigInt::BigInteger d)
{
	this->division(d);
	return *this;
}

bigInt::BigInteger bigInt::BigInteger::modulo(bigInt::BigInteger d)
{
	this->division(d, true);
	return *this;
}

/*
 Internal method for dividing by 1-digit number, is called by regular division
 Takes modulo parameter signifying, if the method should return modulo or product
 */

bigInt::BigInteger bigInt::BigInteger::simpleDivision(int d, bool mod)
{
	int r = 0;
	int temp;
	bigInt::BigInteger q(0);
	q.alloc(this->nsize);
	for(int i=this->nsize-1; i>=0; i--)
	{
		q.number[i] = ((long) this->number[i] + (long) r * base ) / d;
		r = ((long) this->number[i] + (long) r * base ) % d;
	}	
	q.nsize = this->nsize;
	while(q.number[q.nsize-1] == 0)
		q.nsize--;
	if(mod)
		return BigInteger(r);
	else
		return q;	
}


/*
 Division of 2 large numbers. Using the Knuth's Algorithm D found in book 
 Art of Computer Programming, Volume 2: Seminumerical Algorithms by Donald Knuth
 */
bigInt::BigInteger bigInt::BigInteger::division(bigInt::BigInteger d, bool mod)
{
	// dealing with signs, remember if we need to change sign at the end
	bool ressign = false;
	if(this->sign == true && d.sign == false) {
		ressign = true;
		this->sign = false;
	}
	else if(this->sign == false && d.sign == true) {
		ressign = true;	
		d.sign = false;
	}
	else if(this->sign == true && d.sign == true) {
		this->sign = false;
		d.sign = false;
	}
	
	// Deal with stupid behavior, if dividing by zero, hard exiting the program
	if(this->compare(d) == -1) {
		if (mod) return *this;
		else {
			delete[] this->number;
			this->nsize = 0;
			this->alloc(5);
			this->number[0] = 0;
			return *this;
		}
	}
	if(this->compare(d) == 0) 
		if (mod) {
			delete[] this->number;
			this->nsize = 0;
			this->alloc(5);
			this->number[0] = 0;
			this->nsize = 1;
			return *this;
		}
		else {
			delete[] this->number;
			this->nsize = 1;
			this->alloc(5);
			this->number[0] = 1;
			return *this;
		}
	if(d == 0) {
		std::cerr << "Math Error: Dividing by zero!";
		exit(1);
	}

	// If the number is of size 1, moving onto simple division
	if(d.nsize == 1)
	{
		bigInt::BigInteger q = this->simpleDivision(d.number[0], mod);
		this->number = q.number;
		this->nsize = q.nsize;
		if(ressign) this->sign = true;
		return *this;
	}
	
	
	
	// Normalize numbers (D1)
	int normq = (base-1) / (d.number[d.nsize-1]*2);
	bigInt::BigInteger u(this->toString());
	bigInt::BigInteger v(d.toString());
	if(normq > 1) {
		u.multiply(normq);
		v.multiply(normq);
	}
	if(v.number[v.nsize-1] < 500000000)
	{
		u.multiply(2);
		v.multiply(2);
	}
	u.recount();
	int m = u.nsize;
	int n = v.nsize;
	long qh, rh, p;
	bigInt::BigInteger q(0);
	q.alloc(m);
	q.nsize = m-n+1;
	
	// Loop on digits of first number, dividing (D2)
	for(int j=m-n-1; j>=0; j--)
	{
		// Guessing (D3)
		qh = (((long) u.number[j+n]) *base + u.number[j+n-1]) / v.number[n-1];
		rh = (((long) u.number[j+n]) *base + u.number[j+n-1]) - qh*v.number[n-1];

		// Checking for bad behaviour (D3)
		while(qh == base || qh*v.number[n-2] > rh*base + u.number[j+n-2])
		{
			qh = qh - 1;
			rh = rh + v.number[n-1];
			if(rh >= base) break;
		}

		// Multiply and add (D4)
		bigInt::BigInteger s;
		s = v * qh;
		s.shiftLeft(j);
		u.subtract(s);

		q.number[j] = qh;
	}
	// Finishing, if modulo is desired, multiply back and return the difference
	if(!mod)
	{
		if(ressign) this->sign = true;
		this->nsize = q.nsize;
		this->asize = q.asize;
		delete[] this->number;
		this->number = q.number;
		recount();
		return *this;
	}
	else
	{
		bigInt::BigInteger r(this->toString());
		q.multiply(d);
		r.subtract(q);
		this->nsize = r.nsize;
		this->asize = r.asize;
		delete[] this->number;
		this->number = r.number;
		recount();
		return r;
	}
}

/*
 *	Comparing method, overloading comparing operators
 */

int bigInt::BigInteger::compare(bigInt::BigInteger const &second)
{
	if(this->sign == false && second.sign == true)
		return 1;
	if(this->sign == true && second.sign == false)
		return -1;
	
	if(this->nsize > second.nsize)
		return (this->sign) ? -1 : 1;
	if(this->nsize < second.nsize)
		return (this->sign) ? 1 : -1;
	
	for(int i=this->nsize-1; i>=0; i--)
	{
		if(this->number[i] > second.number[i]) 
			return (this->sign) ? -1 : 1;
		if(this->number[i] < second.number[i]) 
			return (this->sign) ? 1 : -1;
	}
	return 0;
}

bool bigInt::BigInteger::operator< (bigInt::BigInteger const &c)
{
	return ((this->compare(c)) == -1);
}

bool bigInt::BigInteger::operator> (bigInt::BigInteger const &c)
{
	return ((this->compare(c)) == 1);
}

bool bigInt::BigInteger::operator<= (bigInt::BigInteger const &c)
{
	return ((this->compare(c)) == -1 || (this->compare(c) == 0));
}

bool bigInt::BigInteger::operator>= (bigInt::BigInteger const &c)
{
	return ((this->compare(c)) == 1 || (this->compare(c) == 0));
}

bool bigInt::BigInteger::operator== (bigInt::BigInteger const &c)
{
	return ((this->compare(c)) == 0);
}

bool bigInt::BigInteger::operator!= (bigInt::BigInteger const &c)
{
	return ((this->compare(c)) != 0);
}

/*
 *	Output methods
 *	print, toString + string operator
 */

void bigInt::BigInteger::print() {
	std::cout << this->toString();
}

std::string bigInt::BigInteger::toString() {
	
	std::stringstream ss;
	if(this->sign == true)
		ss << "-";
	ss << number[nsize-1];
	for(int i=nsize-2; i>=0; i--)
	{
		ss << this->getLeadingZeros(number[i]) << number[i];
	}
	return ss.str();
}

bigInt::BigInteger::operator std::string() {
	return this->toString();
}

/*
 *	Private helpful functions
 *  alloc, recount, getLeadingZeros, checkString, parseString
 */

bool bigInt::BigInteger::alloc(int l) {
	if(l < nsize) return false;
	if(l <= 0) return false;
	int *p = new int[l];
	if(p == NULL) exit(3);
	this->asize = l;
	if(this->nsize == 0)
	{
		for(int i=0; i<l; i++)
			p[i] = 0;
		this->number = p;
	}
	else
	{
 		for(int i=0; i<nsize; i++)
		{
			p[i] = number[i];
		}
		for(int i=nsize; i<asize; i++)
			p[i] = 0;
		delete[] this->number;
		this->number = p;
	}

	return true;
}

void bigInt::BigInteger::recount() {
	if(number[this->nsize-1] == 0) {
		while(number[this->nsize-1] == 0 && this->nsize > 1) this->nsize--;
	}
	else {
		while(number[this->nsize] != 0 && this->nsize < this->asize) this->nsize++;
	}
}

std::string bigInt::BigInteger::getLeadingZeros(int i) {
	if(i < 10)
		return "00000000";
	if(i < 100)
		return "0000000";
	if(i < 1000)
		return "000000";
	if(i < 10000)
		return "00000";
	if(i < 100000)
		return "0000";
	if(i < 1000000)
		return "000";
	if(i < 10000000)
		return "00";
	if(i < 100000000)
		return "0";
	return "";
}

bool bigInt::BigInteger::checkString(std::string const &s) {
	using namespace std;
	for(int i=0; i<s.length(); i++)
	{
		if(i == 0 && s[0] == '-') continue;
		if(!isdigit(s[i])) return false;
	}
	return true;
}

int bigInt::BigInteger::parseString(std::string const &s) {
	int n;
	std::istringstream is(s);
	is >> n;
	if(n < 0)
		return ((-1)*n);
	return n;
}

void bigInt::BigInteger::shiftLeft(int len) {
	this->nsize += len;
	if(this->asize < this->nsize)
	{
		this->alloc(this->nsize + len + 1);
	}
	for(int i=this->nsize-1; i>=0; i--)
	{
		this->number[i+len] = this->number[i];
	}
	for(int i=0; i<len; i++)
		this->number[i] = 0;
}

/*
 *	Operator overloading
 *	addition, subtraction, multiplication
 */

bigInt::BigInteger bigInt::BigInteger::operator+ (bigInt::BigInteger const &a)
{
	bigInt::BigInteger c(this->toString());
	return c.add(a);
}

bigInt::BigInteger bigInt::BigInteger::operator+= (bigInt::BigInteger const &a)
{
	return this->add(a); 
}

bigInt::BigInteger bigInt::BigInteger::operator- (bigInt::BigInteger const &s)
{
	bigInt::BigInteger c(this->toString());
	return c.subtract(s);
}

bigInt::BigInteger bigInt::BigInteger::operator-= (bigInt::BigInteger const &s)
{
	return this->subtract(s); 
}

bigInt::BigInteger bigInt::BigInteger::operator* (bigInt::BigInteger const &b)
{
	bigInt::BigInteger c(this->toString());
	return c.multiply(b);
}

bigInt::BigInteger bigInt::BigInteger::operator*= (bigInt::BigInteger const &b)
{
	return this->multiply(b); 
}

bigInt::BigInteger bigInt::BigInteger::operator/ (bigInt::BigInteger const &b)
{
	bigInt::BigInteger c(this->toString());
	return c.divide(b);
}

bigInt::BigInteger bigInt::BigInteger::operator/= (bigInt::BigInteger const &b)
{
	return this->divide(b); 
}

bigInt::BigInteger bigInt::BigInteger::operator% (bigInt::BigInteger const &b)
{
	bigInt::BigInteger c(this->toString());
	return c.modulo(b);
}

bigInt::BigInteger bigInt::BigInteger::operator%= (bigInt::BigInteger const &b)
{
	return this->modulo(b); 
}
