//
//  fileHelper.cpp
//  model_loading
//
//  Created by lam on 18/10/15.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "FileHelper.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

std::string getStringFromFile(const std::string fullPath)
{
    std::string str;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(fullPath);
        std::stringstream strStream;
        strStream << file.rdbuf();
        str = strStream.str();
        file.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "err : getStringFromFile fail" << std::endl;
    }
    return str;
}