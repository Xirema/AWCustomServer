#include<boost/multiprecision/cpp_int.hpp>
#include<iostream>
#include<iomanip>
#include<boost/random.hpp>
#include<random>

int main() {
	std::random_device rng;

	std::seed_seq seed{ rng(), rng(), rng(), rng(), rng(), rng(), rng(), rng() };
	std::mt19937_64 generator{ seed };
	using big_int = boost::multiprecision::uint256_t;
	boost::random::uniform_int_distribution<big_int> dist;
	big_int value = dist(generator);
	std::cout << "Your Secret Number is 0x" << std::hex << std::setfill('0') << std::setw(64) << value << std::endl;
}