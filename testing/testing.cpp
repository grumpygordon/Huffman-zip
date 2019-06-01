#include <iomanip>
#include <iostream>
#include <fstream>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <vector>
#include <functional>
#include "testing.h"
#include "../utility/utility.h"

char *input = (char *) ("input.txt");
char *tmp = (char *) ("compressed.txt");
char *res = (char *) ("decompressed.txt");

size_t test_num = 0, ac = 0;
double all_time = 0;

void pre_verdict(bool answ) {
    if (answ) {
        std::cout << "[    OK]\n";
		ac++;
    } else {
        std::cerr << "[FAILED] Test #" << test_num << "\n";
    }
}

void large_verdict(bool answ, double ftime) {
    all_time += ftime;
    if (answ) {
		ac++;
        std::ifstream f1(input, std::ios::binary | std::ios::ate);
        std::ifstream f2(tmp, std::ios::binary | std::ios::ate);

        double cur = f2.tellg() * 1.0 / 1024;
        double prev = f1.tellg() * 1.0 / 1024;

        std::cerr << "[    OK] in " << std::fixed << ftime / CLOCKS_PER_SEC << " sec. ";
        std::cerr << cur << " Kb / " << prev << " Kb\n";
    } else {
        std::cerr << "[FAILED] Test #" << test_num << "\n";
        std::cerr << "Passed " << test_num - ac - 1 << " large tests in \n" << std::fixed
                  << all_time / CLOCKS_PER_SEC
                  << " sec.\n";
    }
}

bool check_test(bool small = false) {
    bool answ = true;

    auto start_time = clock();

    encode(input, tmp);
    decode(tmp, res);

    double test_time = (clock() - start_time);
    std::ifstream fin(res);
    std::ifstream finp(input);

    char c1, c2;
    while (fin.get(c1) && answ) {
        if (finp.get(c2)) {
            answ = (c1 == c2);
        } else {
            answ = false;
        }
    }

    if (!fin.get(c1) && finp.get(c2)) {
        answ = false;
    }

    if (small) {
        pre_verdict(answ);
    } else {
        large_verdict(answ, test_time);
    }

    return answ;
}

void run_test(const std::function<void(std::ofstream &)> &fun, bool small = false) {
    if (ac != test_num++ && !small) return;
    std::ofstream fout(input);
    fun(fout);
    fout.close();
    std::cerr << "[RUN   ] Test #" << test_num << "\n";
	check_test(small);
    std::remove(input);
    std::remove(tmp);
    std::remove(res);
}

void test() {
    std::cerr << std::setprecision(2);
    auto stime = clock();

    run_test([](std::ofstream &fout) {}, true);
	run_test([](std::ofstream &fout) { fout << "a"; }, true);
	run_test([](std::ofstream &fout) { fout << "a"; }, true);
	run_test([](std::ofstream &fout) { fout << "aaa"; }, true);
	run_test([](std::ofstream &fout) { fout << "abacabadabacaba"; }, true);
	run_test([](std::ofstream &fout) { for (size_t i = 0; i < 26; i++) fout << char('a' + i); }, true);
	run_test([](std::ofstream &fout) { for (size_t i = 0; i < 256; i++) fout << char(i); }, true);
	run_test([](std::ofstream &fout) { for (size_t i = 0; i < 400; i++) fout << 'a'; }, true);

    if (ac != test_num) {
        std::cerr << "Program failed pre-tests.\nPassed " << ac << " tests.\n";
        return;
    } else {
        std::cerr << "Pre-tests are passed.\nRun large tests:\n";
    }

	run_test([](std::ofstream &fout) { for (size_t i = 0; i < 40000; i++) fout << 'a'; });
	run_test([](std::ofstream &fout) {
		 for (size_t i = 0; i < 150; i++) {
			std::vector<char> g(26);
			std::iota(g.begin(), g.end(), 'a');
			random_shuffle(g.begin(), g.end());
			for (char j : g)
				fout << j;
        }
    });

	run_test([](std::ofstream &fout) {
        for (size_t i = 0; i < 10000; i++) {
            fout << "abbbbcccccccccccccccc";
        }
    });
	run_test([](std::ofstream &fout) {
        for (size_t i = 0; i < 20; i++) {
            for (size_t j = 0; j <= (size_t) (1 << i); j++) {
                fout << (char) (i);
            }
        }
    });
	run_test([](std::ofstream &fout) {
        for (size_t i = 0; i < 1000; i++) {
            fout << (char) (rand() % 256);
        }
    });
	run_test([](std::ofstream &fout) {
        for (size_t i = 0; i < 10000; i++) {
            fout << (char) (rand() % 256);
        }
    });
	run_test([](std::ofstream &fout) {
        for (size_t i = 0; i < 100000; i++) {
            fout << (char) (rand() % 256);
        }
    });
	run_test([](std::ofstream &fout) {
        for (size_t i = 0; i < 1000000; i++) {
            fout << (char) (rand() % 256);
        }
    });
    std::cerr << "All " << test_num << " tests passed.\n";
    std::cerr << "Compressor worked (summary): " << all_time / CLOCKS_PER_SEC << " seconds\n";
    std::cerr << "Tests checked in " << std::fixed
              << (double) (clock() - stime) / CLOCKS_PER_SEC << " seconds\n";
}
