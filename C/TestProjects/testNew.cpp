#include<iostream>
using namespace std;

int main ()
{
    int size;
    int * arr;
    cout<<"Input a size:"<<endl;
    cin>>size;
    if((arr=new int[size])==NULL)
    {
        cout<<"can not allocate memory."<<endl;
        return -1;
    }
    for(int i=0;i<size;i++)
    {
        arr[i]=i;
    }
    for(int i=0;i<size;i++)
    {
        cout<<arr[i]<<endl;
    }
    delete [] arr;

}
