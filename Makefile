test: tst_bld/test
	tst_bld/test

tst_bld/test: Makefile src/rfm9x.c tst_src/gtest_main.cpp tst_src/rfm9x_test.cpp tst_inc/rfm9x_test.h
	g++ -std=c++17 -o tst_bld/test src/rfm9x.c tst_src/gtest_main.cpp tst_src/rfm9x_test.cpp -Iinc -Itst_inc -lgtest
