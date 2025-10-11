#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <semaphore>
using namespace std;

class ZeroEvenOdd {
    private:
      int num;
      int turn;
      counting_semaphore<1> semEven;
      counting_semaphore<1> semOdd;
      counting_semaphore<1> semZero;
      


    public :
    
    ZeroEvenOdd(int n) : semEven(0), semOdd(0), semZero(1), num(n), turn(0) {
    }
    
    void printEven() {

       for(int i=2;i<=num;i+=2) {
         semEven.acquire();
         cout<<i<<endl;
         semZero.release();
       }
    }
    
    void printOdd() {

        for(int i=1;i<=num;i+=2) {
         semOdd.acquire();
         cout<<i<<endl;
         semZero.release();
       }
    }

    void printZero() {

        for(int i=0;i<num;i++) {
            semZero.acquire();
            cout<<0<<endl;
            turn = 1-turn;
            if(turn) {
                semOdd.release();
            }
            else {
                semEven.release();
            }
        }
    }
};

int main() {
    ZeroEvenOdd obj(9);

    vector<thread>threads;
    threads.emplace_back(&ZeroEvenOdd::printOdd,&obj);
    threads.emplace_back(&ZeroEvenOdd::printEven,&obj);
    threads.emplace_back(&ZeroEvenOdd::printZero,&obj);

    for(thread& t : threads) {
        t.join();
    }
    return 0;
}