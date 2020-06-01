#include <iostream>
#include <vector>
using namespace std;

int main() {
    // CLOOK
    int s, m, n;
    scanf("%d", &s);
    scanf("%d", &m);
    scanf("%d", &n);
    int request[n];
    vector<int> order;
    for (int i = 0; i < n; i++) {
        scanf("%d", &request[i]);
    }
    int count = 0;
    for(int i=0; i<n; i++){
        if(request[i] == s){
            order.push_back(s);
            request[i] = -1;
        }
    }
    sort(request, request + n);
    order.push_back(s);
    int begin = 0;
    int mark = 0;
    for(int i=0; i<n; i++){
        if(request[i]>0){
            mark = i;
            break;
        }
    }
    for (int i = mark; i < n; i++) {
        if (request[i] > s) {
            begin = i - 1;
            break;
        }
    }
    for (int i = begin; i >= mark; i--) {
        order.push_back(request[i]);
    }
    for (int i = n - 1; i > begin + 1; i--) {
        order.push_back(request[i]);
    }
    order.push_back(request[begin + 1]);
    int distance = 0;
    int last = -1;
    for(int i=0; i<order.size(); i++){
        if(i==0){
            last = order[i];
        }else{
            distance += abs(order[i]-last);
            last = order[i];
        }
        if(i!=order.size()-1){
            printf("%d ", order[i]);
        }
        else{
            printf("%d\n", order[i]);
        }
    }
    printf("%d\n", distance);
    return 0;
}