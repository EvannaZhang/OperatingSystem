#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>
using namespace std;

struct node{
    int valid = 0;
    int value;
};

int fifo(int len, int num){
    int hit = 0;
    list<int> fifo;
    for(int i=0; i<num; i++){
        int now;
        scanf("%d ", &now);
        list<int>::iterator iter;
        iter = find(fifo.begin(), fifo.end(), now);
        if(iter == fifo.end()){
            //miss
            fifo.push_back(now);
            if(fifo.size() > len)
                fifo.pop_front();
        }else{
            hit++;
        }
    }
    return hit;
}

int lru(int len, int num){
    int hit = 0;
    unordered_map<int, int> values;
    for(int i=0; i<num; i++){
        int now;
        scanf("%d ", &now);
        unordered_map<int, int>::iterator iter;
        iter = values.find(now);
        if(iter == values.end()){
            //miss
            if(values.size()==len){
                //full
                int min = num+1;
                unordered_map<int, int>::iterator save;
                unordered_map<int, int>::iterator iterator;
                for(iterator = values.begin(); iterator!=values.end(); iterator++){
                    if(iterator->second < min){
                        min = iterator->second;
                        save = iterator;
                    }
                }
                values.erase(save);
                values.insert(pair<int, int>(now, i));
            }else{
                values.insert(pair<int, int>(now, i));
            }
        }else{
            hit++;
            values[now] = i;
        }
    }
    return hit;
}


int min(int len, int num){
    int hit = 0;
    unordered_map<int, list<int> > all;
    unordered_set<int> values;
    int page[num];
    unordered_map<int, list<int> >::iterator m_iter;
    for(int i=0; i<num; i++){
        scanf("%d ", &page[i]);
        all[page[i]].push_back(i);
    }
    for (m_iter = all.begin(); m_iter != all.end(); m_iter++) {
        m_iter->second.push_back(num + 10);
    }
    for(int i=0; i<num; i++){
        int now = page[i];
        unordered_set<int>::iterator iter;
        iter = values.find(now);
        if(iter == values.end()){
            //miss
            if(values.size()>=len){
                //full
                unordered_set<int>::iterator toerase;
                int far = -1;
                unordered_set<int>::iterator iterator;
                for(iterator = values.begin(); iterator!=values.end(); iterator++){
                    auto posi = all.find(*iterator);
                    auto listiter = posi->second.begin(); //iterator的list的首位
                    while(listiter!=posi->second.end()){
                        if(*listiter>i) {
                            if (*listiter > far) {
                                far = *listiter;
                                toerase = iterator;
                            }
                            break;
                        }else{
                            auto newli = listiter;
                            listiter++;
                            posi->second.erase(newli);
                        }
                    }
                }
                values.erase(toerase);
                values.insert(page[i]);
            }else{
                values.insert(now);
            }
        }else{
            hit++;
        }
    }

    return hit;
}


int clock(int len, int num){
    int hit = 0;
    int size = 0;
    list<node> values;
    list<node>::iterator hand;
    list<node>::iterator iter;
    for(int i=0; i<len; i++){
        node tmp;
        values.push_back(tmp);
    }
    hand = values.begin();
    for (int i = 0; i < num; i++) {
        int now;
        scanf("%d ", &now);
        bool finished = false;
        for (iter = values.begin(); iter != values.end(); iter++) {
            if (iter->value == now) {
                iter->valid = 1;
                hit++;
                finished = true;
                break;
            }
        }
        if(!finished) {
            if (size < len) {
                hand->valid = 1;
                hand->value = now;
                hand++;
                size++;
                if(size == len) hand = values.begin();
            } else {
                //要 replace
                bool cont = true;
                while (cont) {
                    if (hand->valid == 0) { //replacement
                        hand->value = now;
                        hand->valid = 1;
                        hand++;
                        if (hand == values.end()) {
                            hand = values.begin();
                        }
                        cont = false;
                    } else {
                        hand->valid = 0;
                        hand++;
                        if (hand == values.end()) {
                            hand = values.begin();
                        }
                    }
                }
            }
        }
    }
    return hit;
}


int secondchance(int len, int num){
    int hit = 0;
    list<int> fifo;
    list<int> lastneed;
    unordered_map<int, int> values;
    list<int>::iterator iter;
    unordered_map<int, int>::iterator lru_iter;
    for(int i=0; i<num; i++){
        int now;
        scanf("%d ", &now);
        bool finished = false;
        iter = find(fifo.begin(), fifo.end(), now);
        if(iter != fifo.end()){
            // found in fifo
            finished = true;
            hit++;
        }
        int tomove = -1;
        int position = -1;
        if(!finished){
            lru_iter = values.find(now);
            if(lru_iter != values.end()){
                // found in lru
                // move to fifo, "tomove" to lru
                hit++;
                values[now] = i;
                values.erase(lru_iter);
                tomove = *fifo.begin();
                position = *lastneed.begin();
                fifo.pop_front();
                lastneed.pop_front();
                fifo.push_back(now);
                lastneed.push_back(i);
                finished = true;
            }
        }
        if(!finished){
            if(fifo.size()<len/2) {
                // not hit, fifo small
                fifo.push_back(now);
                lastneed.push_back(i);
                finished = true;
            }
        }
        if(!finished){
            // not hit, fifo large
            tomove = *fifo.begin();
            position = *lastneed.begin();
            fifo.pop_front();
            lastneed.pop_front();
            lastneed.push_back(i);
            fifo.push_back(now);
        }
        if(tomove != -1){
            // move "tomove" to lru
            if(values.size() < len - len/2){
                values.insert(pair<int, int>(tomove, position));
            }else{
                int min = num+1;
                unordered_map<int, int>::iterator save;
                unordered_map<int, int>::iterator iterator;
                for(iterator = values.begin(); iterator!=values.end(); iterator++){
                    if(iterator->second < min){
                        min = iterator->second;
                        save = iterator;
                    }
                }
                values.erase(save);
                values.insert(pair<int, int>(tomove, position));
            }
        }
    }

    return hit;
}


int main() {
    int size, algo, number;
    scanf("%d\n", &size);
    scanf("%d\n", &algo);
    scanf("%d\n", &number);
    int ans = 0;
    switch(algo){
        case 0: ans = fifo(size, number); break;
        case 1: ans = lru(size, number); break;
        case 2: ans = min(size, number); break;
        case 3: ans = clock(size, number); break;
        case 4: ans = secondchance(size, number); break;
        default: ans = -1;
    }
    float percent = 100*(double)ans/(double)number;
    ans = ceil(percent*100);
    printf("%s%.2f%s", "Hit ratio = ", (double)ans/(double)100, "%\n");
    return 0;
}

