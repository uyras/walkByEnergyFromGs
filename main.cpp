#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "config.h"
#include "PartArray.h"
#include <ctime>

using namespace std;

int main(){
    config::Instance()->set2D();
    config::Instance()->srand(time(NULL));

    //PartArray *a = new PartArray(10,10,10,20);
    //a->save("sys.dat");
    PartArray *a = new PartArray("sys.dat");
    a->setToGroundState();
    double Emin = a->calcEnergy1();
    cout<<"E="<<Emin<<endl;
    Vect m = a->calcM1();
    cout<<"Mx="<<m.x<<" My="<<m.y<<" |M|="<<m.length()<<endl;
    vector<Part*>::iterator iter = a->parts.begin(), iter2;
    Part* temp, *temp2; double Etemp=0; double eTotal = 0;
    while(iter!=a->parts.end()){
        temp = (*iter);
        temp->rotate();
        Etemp = a->calcEnergy1();
        eTotal += Etemp;
        cout<<"Eold="<<Emin<<" Enew="<<Etemp<<" diff="<<Emin-Etemp<<endl;
        temp->rotate();
        iter++;
    }

    cout<<"average E="<<eTotal/(double)a->count()<<endl<<endl;

    eTotal=0;
    iter=a->parts.begin();
    while(iter!=a->parts.end()){
        temp = (*iter);
        temp->rotate();
        iter2 = a->parts.begin();
        while(iter2!=a->parts.end()){
            temp2 = (*iter2);
            temp2->rotate();
            Etemp = a->calcEnergy1();
            eTotal += Etemp;
            //cout<<"Eold="<<Emin<<" Enew="<<Etemp<<" diff="<<Emin-Etemp<<endl;
            temp2->rotate();
            iter2++;
        }
        temp->rotate();
        iter++;
    }
    cout<<"average E="<<eTotal/(double)(a->count()*a->count())<<endl<<endl;

    cout<<"finish";
    return 0;
}
