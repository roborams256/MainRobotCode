//
//  ParameterStorage.cpp
//  FRCStorage
//
//  Created by Mitchell Kahn on 2/20/12.
//  Copyright (c) 2012 AppDelegates, LLC. All rights reserved.
//

#include "ParameterStorage.h"
#include <iostream>
#include <fstream>


ParameterStorage::ParameterStorage(){
    
    
};



int ParameterStorage::GetParameterAsInt(char  *parameterName){
    
    
    char *strVal;
    
    strVal = (char *)malloc(100);
    
    std::ifstream myfile (parameterName);
    
    if (myfile.is_open())
    {
        
        myfile >> strVal;
        myfile.close();
    }
    else
        strVal = "-9999";
    
    return atoi(strVal);

};

float ParameterStorage::GetParameterAsFloat(char  *parameterName){
    
    
    char *strVal;
    
    strVal = (char *)malloc(100);
    
    std::ifstream myfile (parameterName);
    
    if (myfile.is_open())
    {
        
        myfile >> strVal;
        myfile.close();
    }
    else
        strVal = "-9999.9999";
    
    return atof(strVal);
    
};



void ParameterStorage::SaveParameterAsInt(char  *parameterName, int param){
    
    char strVal[100];
    sprintf(strVal, "%d",param);
    
    std::ofstream ofile (parameterName);
    if (ofile.is_open())
    {
        
        ofile << strVal;
    }
    
    ofile.close();
    
};


void ParameterStorage::SaveParameterAsFloat(char  *parameterName, float param){
    
    char strVal[100];
    sprintf(strVal, "%1.5f",param);
    
    std::ofstream ofile (parameterName);
    if (ofile.is_open())
    {
        
        ofile << strVal;
    }
    
    ofile.close();
    
};


