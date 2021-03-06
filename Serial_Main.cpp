// Iurii Kondrakov
// Serial_Main.cpp
// 2021.12.06
#include <iostream>
#include <chrono>

//Exclusive Scan
template <typename T, typename C>
int excl_scan(
	const T* in,                // source data
	T* out,                     // output data
	int size,                   // size of data sets
	C combine,                  // combine operation
	T initial                   // initial value
) {

	if (size > 0) {
		for (int i = 0; i < size - 1; i++) {
			out[i] = initial;
			initial = combine(initial, in[i]);
		}
		out[size - 1] = initial;
	}
	return 1; // 1 thread
}

// report system time
void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(span);
	std::cout << msg << " - took - " <<
		ms.count() << " microseconds" << std::endl;
}

int main(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << argv[0] << ": invalid number of arguments\n";
		std::cerr << "Usage: " << argv[0] << "\n";
		std::cerr << "Usage: " << argv[0] << "  power_of_2\n";
		return 1;
	}
	std::cout << "Serial Prefix Scan" << std::endl;

	// initial values for testing
	const int N = 9;
	const int in_[N]{ 3, 1, 7, 0, 1, 4, 5, 9, 2 };

	// command line arguments - none for testing, 1 for large arrays
	int n, nt{ 1 };
	if (argc == 1) {
		n = N;
	}
	else {
		n = 1 << std::atoi(argv[1]);
		if (n < N) n = N;
	}
	int* in = new int[n];
	int* out = new int[n];

	// initialize
	for (int i = 0; i < N; i++)
		in[i] = in_[i];
	for (int i = N; i < n; i++)
		in[i] = 1;
	auto add = [](int a, int b) { return a + b; };

	std::chrono::steady_clock::time_point ts, te;

	// Exclusive Prefix Scan
	ts = std::chrono::steady_clock::now();
	nt = excl_scan<int, decltype(add)>(in, out, n, add, (int)(0));
	te = std::chrono::steady_clock::now();

	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << out[i] << ' ';
	std::cout << out[n - 1] << std::endl;
	reportTime("Exclusive Scan", te - ts);


	delete[] in;
	delete[] out;
}
