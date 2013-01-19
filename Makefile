CXXFLAGS=-Wall -pedantic

all: cpp11_template_alias cpp11_not_template_alias cpp98 

cpp11_not_template_alias: clean
	g++ $(CXXFLAGS) -std=c++0x -I./cpp11_not_template_alias -o test_aop_cpp11_not_template_alias cpp11_not_template_alias/test.cpp

cpp11_template_alias: clean
	g++ $(CXXFLAGS) -std=c++0x -I./cpp11_template_alias -o test_aop_cpp11_template_alias cpp11_template_alias/test.cpp

cpp98: clean
	g++ $(CXXFLAGS) -std=c++98 -I./cpp98 -o test_aop_cpp98 cpp98/test.cpp

clean:
	rm -f test_aop_*
