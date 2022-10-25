#include<bits/stdc++.h>
using namespace std;
char str[20] = "hello";
int main(){
      string s(str,str + 20);
      cout << s << endl;
      istringstream is(s);

      while(is >> s)
      {
            cout << "hello " << s << "end" << endl;
      }
      return 0;
}