/*
 Gauss.h - source class for Gaussian Random Number Generator

 Kacey Coley        February 18, 2015


 This class accepts a real-valued mean and standard deviation parameter
 and an integer seed value.  Using the assignment operator, a random
 number will be generated based on a Gaussian normal distribution
 function.
 The project must be compiled using C++11.
*/

#ifndef __GAUSS_H__
#define __GAUSS_H__
#include <memory>
class Gauss {
public:
	Gauss(const double mean, const double std, const int seed);
	Gauss(const Gauss &g);
	Gauss& operator=(const Gauss &g);
	void reset(const double mean, const double std, const int seed);
	~Gauss();
	double operator()();
private:
	class Impl;
	std::shared_ptr<Impl> mImpl;
};
//double gauss(const double mean, const double std, const double seed);
#endif // __GAUSS_H__
