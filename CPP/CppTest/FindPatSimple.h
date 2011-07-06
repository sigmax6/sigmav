#include <string>
#include <assert.h>
using namespace std;
int FindPatSimple(string S,string P,int startIndex)
{
    int lastIndex=S.size()-P.size();
    if(lastIndex<startIndex)
    {
        return -1;
    }
    else
    {
        int i=startIndex;
        int j=0;
        while(i<S.size()&&j<P.size())
        {
            if(S[i]==P[j])
            {
                i++;
                j++;
            }
            else
            {
                i=i-j+1;
                j=0;         //back tracking
            }
        }
        if(j>=P.size())
        {
            return i-j;
        }
        else
        {
            return -1;
        }
    }
}
