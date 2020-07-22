//
//  Define.h
//  test
//
//  Created by lam on 18/6/20.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

enum PolygonMode
{
    Fill = 0,
    Line = 1,
};

#define Window_Width 1280
#define Window_Height 720

static PolygonMode g_polygonMode = PolygonMode::Fill;;