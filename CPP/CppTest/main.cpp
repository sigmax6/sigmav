#include "FindPatSimple.h"
#include "FindPatKMP.h"
#include <iostream>
int main (int argc, char const* argv[])
{
   string str1("This is a test of simple pattern find function");
   string str2("patternt");
   int res=FindPatKMP(str1,str2,2);
   if(res!=-1)
   {
       cout<<"Pattern find succeed! in "<<str1<<" pos:"<<res<<endl;
   }
   else
   {
       cout<<"Pattern find failed!"<<endl;
   }
   return 0;
}
