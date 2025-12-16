// ==============================================================
//                 ORBITER MODULE: FlightData
//                  Part of the ORBITER SDK
//            Copyright (C) 2003 Martin Schweiger
//                   All rights reserved
//
// Graph.h
// Generic data graph class interface.
// Updated by Matías Saibene in Dec 2025 ==============================================================

#pragma once
#include <array>
#include <Windows.h>
#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_
#include "../../include/Orbitersdk.h"

constexpr int MAXPLOT{3};

struct GDIres{
    std::array<HFONT, 2> font;
    std::array<HPEN, 2 + MAXPLOT> pen;
    int nref;
};

class Graph{

    public:

        Graph();
        ~Graph();

        static void InitGDI();
        static void FreeGDI();

        void AppendDataXY(POINT point);
	    void Refresh(HDC hDC, int w, int h);

        std::array<POINT, 10> points;
        std::array<POINT, 10> original_points;
        void ClearPoints();

    private:

        static GDIres gdi;

        int idpt;
};

#endif //!_GRAPH_HPP_