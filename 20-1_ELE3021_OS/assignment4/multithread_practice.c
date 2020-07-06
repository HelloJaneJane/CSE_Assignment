#include <stdio.h>
#include <stdlib.h>

#define ARGUMENT_NUMBER 20
#define ll long long

ll result=0;

void* ThreadFunc(void* n){
    ll number = *((ll*)n);
    printf("number = %lld\n", number);
    for(ll i=0; i<25000000; i++){
        result += number;
    }
    return 0;
}

int main(){
    ll argument[ARGUMENT_NUMBER];

    for(ll i=0; i<ARGUMENT_NUMBER; i++){
        argument[i]=i;
    }
    for(ll i=0; i<ARGUMENT_NUMBER; i++){
        ThreadFunc((void*)&argument[i]);
    }
    
    printf("result = %lld\n", result);
    return 0;
}