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
double eAvg, eAvgMin, eAvgMax, Einit, eTemp, Emin=99999;
Vect mAvg;
StateMachineFree minState;

ofstream f("res.dat");

void walkEnergy(int deepness, PartArray *a, vector<Part*>::iterator iter){
    if (iter==a->parts.begin()){
        totalSteps=0;
        eAvg=0; eAvgMin =  99999; eAvgMax = -99999;
        mAvg = Vect(0,0,0);
    }
    if (deepness==0){
        totalSteps++;
        mAvg+=a->calcM1();
        eTemp = a->calcEnergy1FastIncremental(Einit);
        eAvg += eTemp;
        if (eTemp<Emin){
            Emin=eTemp;
            minState = *(a->state);
        }
        if (eTemp>eAvgMax) {
            eAvgMax=eTemp;
        }
        if (eTemp<eAvgMin){
            eAvgMin=eTemp;
        }
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

    cout<<"drop..."<<endl;
    PartArray *a = new PartArray(15,15,1,25);
    //a->save("sys.dat");
    //PartArray *a = new PartArray("sys.dat");
    cout<<"turn all UP..."<<endl;
    //a->setToGroundState();
    a->turnRight();
    a->state->hardReset();

    Einit = a->calcEnergy1FastIncrementalFirst();

    int stepsSum=0;
    for (int i=1;i<=a->count();i++){
        walkEnergy(i,a, a->parts.begin());
        stepsSum+=totalSteps;
        mAvg= mAvg/(double)totalSteps;
        cout<<i<<"parts, average E="<<eAvg/(double)totalSteps;
        cout<<" Mx="<<mAvg.x<<" My="<<mAvg.y<<" |M|="<<mAvg.length()/(double)totalSteps;
        cout<<" steps="<<totalSteps<<endl;
        f<<eAvg/(double)totalSteps<<"\t"
           <<eAvgMin<<"\t"
             <<eAvgMax<<"\t"
        <<a->count()-(i*2)<<"\t"<<mAvg.x<<"\t"<<mAvg.y<<"\t"<<mAvg.length()<<"\t"<<stepsSum<<endl;
    }
    cout<<endl;
    cout<<"total steps="<<stepsSum<<" 2^"<<a->count()<<"="<<pow(2,a->count())<<endl;

    cout<<"Emin="<<Emin<<endl;
    minState.draw();


    cout<<"finish";
    return 0;
}
