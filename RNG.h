#pragma once
#include <random>
#include <iostream>

//https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
class RNG
{
private:
	std::random_device rd;
	std::mt19937 gen;

public:
	int GetRandom(int exclusiveMax)
	{
		std::uniform_int_distribution<> distr(0, exclusiveMax-1);
		return distr(gen);
	}

	RNG()
	{
		gen = std::mt19937(rd());
	}
};