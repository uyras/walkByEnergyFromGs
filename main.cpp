#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "config.h"
#include "PartArray.h"
#include <ctime>

using namespace std;


int totalSteps;
double eAvg, Emin;
Vect mAvg;
void walkEnergy(int deepness, PartArray *a, vector<Part*>::iterator iter){
    if (iter==a->parts.begin()){
        totalSteps=0;
        eAvg=0;
        mAvg = Vect(0,0,0);
    }
    if (deepness==0){
        totalSteps++;
        mAvg+=a->calcM1();
        eAvg+=a->calcEnergy1FastIncremental(Emin);
    }

    Part* temp;
    int steps=0;
    while(iter!=a->parts.end()){
        temp = (*iter);
        if (!temp->state){
            temp->rotate();
            walkEnergy(deepness-1,a,iter);
            temp->rotate();
            steps++;
        }
        iter++;
    }
}

int main(){
    config::Instance()->set2D();
    config::Instance()->srand(time(NULL));

    PartArray *a = new PartArray(15,15,1,20);
    //a->save("sys2.dat");
    //PartArray *a = new PartArray("sys.dat");
    a->setToGroundState();
    a->state->hardReset();
    Emin = a->calcEnergy1FastIncrementalFirst();
    cout<<"E="<<Emin<<endl;
    Vect m = a->calcM1();
    cout<<"Mx="<<m.x<<" My="<<m.y<<" |M|="<<m.length()<<endl;

    cout<<endl;
    int stepsSum=0;
    for (int i=1;i<=a->count();i++){
        walkEnergy(i,a, a->parts.begin());
        stepsSum+=totalSteps;
        cout<<i<<"parts, average E="<<eAvg/(double)totalSteps;
        cout<<" Mx="<<mAvg.x<<" My="<<mAvg.y<<" |M|="<<mAvg.length()/(double)totalSteps;
        cout<<" steps="<<totalSteps<<endl;
    }
    cout<<endl;
    cout<<"total steps="<<stepsSum<<" 2^"<<a->count()<<"="<<pow(2,a->count())<<endl;

    cout<<"finish";
    return 0;
}
