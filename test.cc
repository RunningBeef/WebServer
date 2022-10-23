#include<bits/stdc++.h>
using namespace std;
enum class test
{
      KHello = 0,
      KWorld,
      KWhat,
      KWhy,
      KL,
      KA,
      KB,
      KC
};
int main(){
      test x = test::KC;
      cout << sizeof(x) << endl;
      return 0;
}