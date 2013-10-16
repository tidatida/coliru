#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

class Failure{};

class MyClass{
    public:
    template <typename T>
    class Par{
        public:
        Par() : valid(false), content() {}
        Par(const T& c) : valid(true), content(c) {}
        T get() const {
            if(!valid)
                throw Failure();
            return content;
        }
        operator const bool() const {
    		return valid;
		}
        private:
        bool valid;
        T content;
    };
    
    MyClass(int _par1, double _par2, char _par3) : par1(_par1), par2(_par2), par3(_par3){}
    MyClass(int _par1, double _par2) : par1(_par1), par2(_par2), par3() {}
    Par<int> par1;
    Par<double> par2;
    Par<char> par3;
    
    void drawLine(){
        if(par1.get() == 1)
            par3.get();
        par2.get();
    }
    
private:
    MyClass();
};

int main() {
    MyClass mc(2, 2.);
    
    mc.drawLine();
}
