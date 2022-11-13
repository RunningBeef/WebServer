#include<bits/stdc++.h>
using namespace std;

class A{
public:
~A()
{
      cout << "Destroy ++" << endl;
}
public: int x;
};
void func(A & a)
{
      A &b = a;
}

int main()
{
      A a;
      func(a);
      return 0;
}