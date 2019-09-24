#include <iostream>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
int main(){
    stack<int> s;
    char input[100000];
    char *p;
    int value = 0;
    int top1, top2;
    int answer = 0;
    cin.getline(input, 100000);
    p = strtok(input, " ");
    while(p != NULL){
        if(!strcmp(p, "+") || !strcmp(p, "-") || !strcmp(p, "*")){
            top1 = s.top();
            s.pop();
            top2 = s.top();
            s.pop();
            if(!strcmp(p, "+")){answer = top1+top2; s.push(answer);}
            else if(!strcmp(p, "-")){answer = top2-top1; s.push(answer);}
            else if(!strcmp(p, "*")){answer = top1*top2; s.push(answer);}
        }
        else{
            value = atoi(p);
            s.push(value);
        }
        p = strtok(NULL, " ");
    }
    printf("%d\n", s.top());
    return 0;
}