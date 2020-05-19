all: verifier
verifier: verifier.cc
	g++ --std=c++17 verifier.cc -o verifier -lstdc++fs
clean:
	rm verifier
