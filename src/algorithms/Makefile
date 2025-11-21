.PHONY = all clean bin

all : bin

bin : greedy_ssp gpu_greedy_ssp partial_gpu_ssp

clean:
	rm -rf greedy_ssp gpu_greedy_ssp partial_gpu_ssp

greedy_ssp: greedy_ssp.cc
	$(CXX) -std=c++11 -Wall -O3 -fopenmp greedy_ssp.cc -o greedy_ssp

gpu_greedy_ssp: gpu_greedy_ssp.cc
	$(CXX) -std=c++11 -Wall -O3 -fno-lto -fopenmp -fstack-protector gpu_greedy_ssp.cc -o gpu_greedy_ssp

partial_gpu_ssp: partial_gpu_ssp.cc
	$(CXX) -std=c++11 -Wall -O3 -fno-lto -fopenmp -fstack-protector partial_gpu_ssp.cc -o partial_gpu_ssp
