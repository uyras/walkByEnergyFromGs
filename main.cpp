#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "config.h"
#include "PartArray.h"
#include <ctime>
#include <stdexcept>

using namespace std;


int totalSteps;
double eAvg, eAvgMin, eAvgMax, Einit, eTemp, Emin=99999;
Vect mAvg;
StateMachineFree minState;

ofstream f("averageTrying_60_35000.dat");

int experiments = 50000;
int parts = 60;
bool showGS = false;

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

bool walkNotAllEnergy(int count, PartArray* a){
    int rotated = 0; Part* temp;
    while (rotated<count){
        int ran = config::Instance()->rand();
        int rm = config::Instance()->rand_max;
        ran = floor(double(ran)/double(rm)*double(a->count()-1));
        try {
            temp = a->parts.at(ran);
        }
        catch (const std::out_of_range& oor) {
            std::cerr << "Out of Range error: " << oor.what() << '\n';
        }


        if (!temp->state){
            temp->rotate();
            rotated++;
        }
    }
    return true;
}

int main(){
    config::Instance()->set2D();
    config::Instance()->randmode_standart();
    config::Instance()->srand(time(NULL));

    cout<<"drop..."<<endl;
    PartArray *a = new PartArray(30,30,1,parts);
    a->save("sys.dat");
    cout<<"count - "<<a->count()<<endl;
    //a->save("sys.dat");
    //PartArray *a = new PartArray("sys.dat");
    cout<<"turn all UP..."<<endl;
    //a->setToGroundState();

    //поворачиваем все вверх
    a->turnUp();
    a->state->hardReset();

    //Считаем начальную энергию
    Einit = a->calcEnergy1FastIncrementalFirst();

    for (int i=1;i<a->count();i++){
        double eAvg2=0;

        //Обходим рекурсивно несколько вариантов
        for (int j=0;j<experiments;j++){
            walkNotAllEnergy(i,a);
            //a->state->draw();
            eAvg2+=a->calcEnergy1FastIncremental(Einit);
            a->state->reset();
        }
        cout<<i<<"\t"<<eAvg2/(double)experiments<<"\t"<<endl;
        f<<i<<"\t"<<eAvg2/(double)experiments<<"\t"<<endl;
    }

    cout<<endl;

    if (showGS){
        if (a->count()<23)
            a->setToGroundState();
        else
            a->setToPTGroundState(10,5000);
        cout<<"Emin = "<<a->calcEnergy1()<<endl;
    }

    //minState.draw();


    cout<<"finish";
    return 0;
}
