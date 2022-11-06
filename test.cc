#include<bits/stdc++.h>
using namespace std;
char str[20] = "hello";
int main(){
      shared_ptr<int> t = make_shared<int>(10);
      t.reset();
      if(t)
      {
            cout << "not null" << endl;
      }else cout << "null" << endl;
      return 0;
}