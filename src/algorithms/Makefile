.PHONY = all clean bin

all : bin amdahl_greedy_ssp

bin : greedy_ssp gpu_greedy_ssp

clean:
	rm -rf greedy_ssp gpu_greedy_ssp amdahl_greedy_ssp

greedy_ssp: greedy_ssp.cc
	$(CXX) -std=c++11 -Wall -O3 -fopenmp greedy_ssp.cc -o greedy_ssp

gpu_greedy_ssp: gpu_greedy_ssp.cc
	$(CXX) -std=c++11 -Wall -O3 -fno-lto -fopenmp -fstack-protector gpu_greedy_ssp.cc -o gpu_greedy_ssp

amdahl_greedy_ssp: greedy_ssp.cc
	$(CXX) -std=c++11 -Wall -O3 -fopenmp -DAMDAHL greedy_ssp.cc -o amdahl_greedy_ssp
