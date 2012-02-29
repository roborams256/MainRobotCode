//
//  ParameterStorage.h
//  FRCStorage
//
//  Created by Mitchell Kahn on 2/20/12.
//  Copyright (c) 2012 AppDelegates, LLC. All rights reserved.
//

#include <stdio.h>



class ParameterStorage {
    
private:
    
    
    
    
public:
    
    ParameterStorage();
    
    bool fileIsCool;
    
    //TODO should be Template?
    
    float GetParameterAsFloat(char *parameterName);

    int GetParameterAsInt(char *parameterName);
    
    void SaveParameterAsInt(char *parameterName, int value);
    void SaveParameterAsFloat(char *parameterName, float value);
   
};
