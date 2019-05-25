#include <iomanip>
#include "compress_testing.h"
#include "utility/utility.h"
#include <numeric>
char *input = (char *) ("input.txt");
char *tmp = (char *) ("compressed.txt");
char *res = (char *) ("decompressed.txt");

size_t test_num = 0, pre_ac = 0;
double all_time = 0;

void start_test() {
    std::fstream fs;
    fs.open(input, std::ios::out);
}

void finish_test() {
    std::remove(input);
    std::remove(tmp);
    std::remove(res);
}

void pre_verdict(bool answ) {
    if (answ) {
        std::cout << "[    OK]\n";
        pre_ac++;
    } else {
        std::cerr << "[FAILED] Test #" << test_num << "\n";
    }
}

void large_verdict(bool answ, double ftime) {
    all_time += ftime;
    if (answ) {
        std::ifstream f1(input, std::ios::binary | std::ios::ate);
        std::ifstream f2(tmp, std::ios::binary | std::ios::ate);

        double cur = f2.tellg() * 1.0 / 1024;
        double prev = f1.tellg() * 1.0 / 1024;

        std::cerr << "[    OK] in " << std::fixed << ftime / CLOCKS_PER_SEC << " sec. ";
        std::cerr << cur << " Kb / " << prev << " Kb\n";
    } else {
        std::cerr << "[FAILED] Test #" << test_num << "\n";
        std::cerr << "Passed " << test_num - pre_ac - 1 << " large tests in \n" << std::fixed
                  << all_time / CLOCKS_PER_SEC
                  << " sec.\n";
    }
}

bool check_test(bool small = false) {
    test_num++;
    bool answ = true;

    auto start_time = clock();

    compress(input, tmp);
    decompress(tmp, res);

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

    std::cerr << "[RUN   ] Test #" << test_num << "\n";
    if (small) {
        pre_verdict(answ);
    } else {
        large_verdict(answ, test_time);
    }

    finish_test();
    return answ;
}

void test() {
    std::cerr << std::setprecision(2);
    auto stime = clock();

    {
        // empty file
        start_test();
        check_test(true);
    }

    {
        start_test();

        std::ofstream fout(input);
        fout << "a";
        fout.close();

        check_test(true);
    }

    {
        start_test();

        std::ofstream fout(input);
        fout << "aaa";
        fout.close();

        check_test(true);
    }

    {
        start_test();

        std::ofstream fout(input);
        for (int i = 0; i < 26; i++)
			fout << char('a' + i);
        fout.close();

        check_test(true);
    }

    {
        start_test();

        std::ofstream fout(input);
		for (int i = 0; i < 256; i++)
			fout << char(i);
		fout.close();

        check_test(true);
    }

    {
        start_test();

        std::ofstream fout(input);
        fout << "abacabadabacaba";
        fout.close();

        check_test(true);
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 400; i++)
            fout << 'a';
        fout.close();

        check_test(true);
    }

    if (pre_ac != test_num) {
        std::cerr << "Program failed pre-tests.\nPassed " << pre_ac << " tests.\n";
        return;
    } else {
        std::cerr << "Pre-tests are passed.\nRun large tests:\n";
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 40000; i++)
            fout << 'a';
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 150; i++) {
			std::vector<char> g(26);
			std::iota(g.begin(), g.end(), 'a');
			random_shuffle(g.begin(), g.end());
			for (char j : g)
				fout << j;
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 10000; i++) {
            fout << "abbbbcccccccccccccccc";
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 20; i++) {
            for (size_t j = 0; j <= (size_t) (1 << i); j++) {
                fout << (char) (i);
            }
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 128; i++) {
            fout << (char) (i);
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 1000; i++) {
            fout << (char) (rand() % 128);
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 10000; i++) {
            fout << (char) (rand() % 128);
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    {
        start_test();

        std::ofstream fout(input);
        for (size_t i = 0; i < 1000000; i++) {
            fout << (char) (rand() % 128);
        }
        fout.close();

        if (!check_test()) {
            return;
        }
    }

    std::cerr << "All " << test_num << " tests passed.\n";
    std::cerr << "Compressor worked (summary): " << all_time / CLOCKS_PER_SEC << " seconds\n";
    std::cerr << "Tests checked in " << std::fixed
              << (double) (clock() - stime) / CLOCKS_PER_SEC << " seconds\n";
}
