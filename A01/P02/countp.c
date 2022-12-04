#include <stdio.h>

static int count(int n,int open,int close,int oc,int n1,int c)
{

    if(c==n && oc==0){
       
        return n1;

    }else{
            
        if(oc>=1){
            count(n,open,close+1,oc-1,n1+1,c+1);
        }else{
            count(n,open+1,close,oc+1,n1+1,c+1);
        }   
    }
    
}

int main()
{
    int i = count(5,1,0,1,0,0);
    printf("%2d",i);
    return 0;
}