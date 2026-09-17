//Michael San Pietro
//Code which uses Monte Carlo Simulation to estimate the price of a European Call Option
#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>

double MonteCarlo(int iterations, int time) 
{
	//time is time until expiry in days
	//iterations number of samples to take
	
	//convert time into years
	double T = double(time) / 365;
	//create a random generator for gaussian term Z Standard Normal with mean 0 and variance 1
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::normal_distribution<double> Z(0.0, 1.0);
	
	//Price a call with strike 120 for underlying with intial price 100 and day-by-day volatility (sd) 5
	double option_strike = 90;
	double stock_price = 100;
	double vol = 0.2;

	//1-year T-bill
	double rate = 0.0413;
	

	//iterator
	int i = 0;
	//sum of profit from options
	double sum = 0;
	while (i != iterations) 
	{
		//Z is standard normal variable with mean 0 and variance 1, use both Z and -Z as anthitetic variates to reduce variance
		double z = Z(gen);
		//produce a stock price sample at time T and add to sum
		double stock_price_T= stock_price * std::exp(((rate - (vol * vol * 0.5)) * T) + vol*sqrt(T)*z);
		//use negative Z variant as well
		double stock_price_T_i=stock_price * std::exp(((rate - (vol * vol * 0.5)) * T) + vol * sqrt(T) * -1 * z);

		//compute profits for each 
		double profit1 = std::max(stock_price_T - option_strike, 0.0);
		double profit2 = std::max(stock_price_T_i - option_strike, 0.0);
		//add both profits to the sum
		sum = sum + profit1;
		sum = sum + profit2;
		++i;
	}
	//Now compute option price e&{-rT} is discounted time value of money term, multiply iterations by two to account for the anthithetic variates
	double price = exp(-1 * rate * T) * (sum / (iterations*2));

	//return price
	return price;
}

int main() {
	
	//Run Monte Carlo Simulation and print output
	std::cout << "Computed Option Price: " << MonteCarlo(100000000, 90) << std::endl;
	std::cout << "Done";
	return 0;
}