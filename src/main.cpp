/*************************************************************************************************/
// 2D Vertex Model for Plant Morphogenesis
// Original Author: Yasuhiro Inoue (inoue.yasuhiro.4n@kyoto-u.ac.jp), for animal morphogenesis simulation
// Modified by: Zining Wang (wangzining16@mails.ucas.ac.cn), for plant morphogenesis simulation based on cell division patterns
// Reference: Kinoshita, A., Naito, M., Wang, Z., Inoue, Y., Mochizuki, A., & Tsukaya, H. (2022). Position of meristems and the angles of the cell division plane regulate the uniqueness of lateral organ shape. Development, 149(23), dev199773.
/*********************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <vector>
#include <random>
#include <time.h>
#include <cstring>
#include <assert.h>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>

#include "../include/class.h"
#include "../include/division.h"
#include "../include/vec.h"
#include "../include/parameter.h"
#include "../include/force.h"
#include "../include/geo2dv.h"
#include "../include/IOV.h"
#include "../include/wangSystem.h"
#include "../include/wangMath.h"


#include "../include/2dvOrgan.h"
#include "../include/2dvCell.h"
#include "../include/2dvLine.h"
#include "../include/2dvVertex.h"
#include "../include/2dvInitial.h"

//mode = "ArabidopsisPetal" or "SharpApex" or "ConcaveApex"

using namespace std;

int main(){

parameter::read_mode(modeFile);


//recording time
vector<time_t> initial_time, terminal_time;
time_t initial_time_first = wangSystem::initial_time();
initial_time.push_back (initial_time_first);

cout<<"******************| Mode Selection| ********************************"<<endl;
cout<<"Mode "<<mode<<endl;
cout<<"********************************************************************"<<endl;



//initialization
//parameter::read_and_record();
parameter::read(parameterInputFile);
parameter::record();

Organ *p_g = new Organ;
readV::read_organ_txt(p_g,0);
organ_geo::epidermal_identity(p_g);

force::forceShapeInitiation(p_g,200000);
division::cell_time_initialization(p_g);

//simulation
cout<<"*********************| Simulation Start |***************************"<<endl;
for(int step=0; step<step_end; step++){
    
    p_g->step=step;
    force::calcForceMotion(p_g);

    //check cell division
    if(step%T_division_check==0){
        cout<<"At time step "<<step<<endl;
        //cout<<"Start geometrics calculation ---- ";
        geo::calcGeometrics(p_g);
        //cout<<"Output ---- ";
        output::geo(p_g);
        //cout<<"Finished "<<endl;

        //cout<<"Start division event judgement ---- ";
        division::Global(p_g);
        //cout<<"Division events output ---- ";
        output::division(p_g);
        //cout<<"Finished "<<endl;

        cout<<"********************************************************************"<<endl;
    }

    //cell time add
    if(step%T_cell_time==0){
        division::cellTimeAdd(p_g,1);
    }

    //output VTK
    if(step%T_vtkoutput==0){
        output::VTK(p_g);            
        if(termination::checkEnd(p_g)==1){
                    goto End_One_Simulation1;
        }
    }
}
End_One_Simulation1:;
    cout<<"********************************************************************"<<endl;

//record termination time
time_t terminal_time_last = wangSystem::terminal_time();
terminal_time.push_back(terminal_time_last);

output::simulation_log(initial_time,terminal_time);

return 0;
}