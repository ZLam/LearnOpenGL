//
//  RandomHelper.cpp
//  model_loading
//
//  Created by lam on 18/10/16.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "RandomHelper.hpp"

std::mt19937& RandomHelper::getEngine() {
    static std::random_device seed_gen;
    static std::mt19937 engine(seed_gen());
    return engine;
}