/*
 Gauss.cpp - source class for Gaussian Random Number Generator

 Kacey Coley        February 18, 2015
 

 This class accepts a real-valued mean and standard deviation parameter
 and an integer seed value.  Using the assignment operator, a random 
 number will be generated based on a Gaussian normal distribution
 function.  
 The project must be compiled using C++11.
*/
#include "Gauss.h"
#include <iostream>

#include <random>
class Gauss::Impl {
public:
	Impl(const double mean, const double std, const int seed) : _mean(mean), _std(std), _seed(seed), _gen(), _d() {

		std::random_device rd;
		std::mt19937 gen(rd());
		gen.seed(seed);
		std::normal_distribution<> d(mean, std);
		_gen = gen;
		_d = d;

	}
	double _mean;
	double _std;
	int _seed;
	std::mt19937 _gen;
	std::normal_distribution<> _d;
};
Gauss::Gauss(const double mean, const double std, const int seed) : mImpl(new Impl(mean, std, seed)) {}
Gauss::Gauss(const Gauss &g) : mImpl(new Impl(g.mImpl->_mean, g.mImpl->_std, g.mImpl->_seed)) {}

Gauss& Gauss::operator=(const Gauss &other) {
	if (this != &other) {
		mImpl->_gen = other.mImpl->_gen;
		mImpl->_mean = other.mImpl->_mean;
		mImpl->_std = other.mImpl->_std;
		mImpl->_seed = other.mImpl->_seed;
		mImpl->_d.param(other.mImpl->_d.param());

	}
	return *this;
}
Gauss::~Gauss() {}
void Gauss::reset(const double mean, const double std, const int seed) {
	mImpl->_mean = mean;
	mImpl->_std = std;
	mImpl->_seed = seed;
	std::normal_distribution<> d(mean, std);

	mImpl->_d = d;
	mImpl->_d.reset();
	mImpl->_gen.seed(seed);
}
double Gauss::operator()() {
	return mImpl->_d(mImpl->_gen);
}

