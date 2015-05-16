just manual for compile here:
wget https://github.com/ysle11/otskok-impuls/blob/master/calculator.h
wget https://github.com/ysle11/otskok-impuls/blob/master/calculator.cpp
g++ -msse -mtune=pentium3 -mfpmath=sse -mno-mmx calculator.cpp -o calculator
now man how to using it:
./calculator -a opt mode1 -p 1440
it take 2-4 hours for calculation
next step - get results for opening orders
./calculator -a test mode1 -p 1440
the results look like http://bogsuv.hj.cx/
