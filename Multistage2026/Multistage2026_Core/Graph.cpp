#include "Graph.hpp"
#include <array>
#include <format>
#include <string>

// ==============================================================
//                 ORBITER MODULE: FlightData
//                  Part of the ORBITER SDK
//            Copyright (C) 2003 Martin Schweiger
//                   All rights reserved
//
// Graph.cpp
// Generic data graph class implementation.
// ==============================================================

#define STRICT

static std::array<COLORREF, MAXPLOT> plotcol{
    0x0000ff, 0xff0000, 0x00ff00
};

Graph::Graph(){
    
    idpt = 0;

    for(unsigned int i = 0; i < 10; i++){
        points.at(i).x = 0;
        points.at(i).y = 0;
    }

    original_points = {};

}

Graph::~Graph(){

}

void Graph::InitGDI(){

    if(!gdi.nref){
        gdi.font.at(0) = CreateFont(-10, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 49, "Arial");
		gdi.font.at(1) = CreateFont (-10, 0, 900, 900, 400, 0, 0, 0, 0, 3, 2, 1, 49, "Arial");
		gdi.pen.at(0)  = CreatePen (PS_SOLID, 2, 0x0000FF);
		gdi.pen.at(1)  = CreatePen (PS_SOLID, 0, 0xD0D0D0);
    }

    for(int i = 0; i < MAXPLOT; i++){
        gdi.pen.at(2 + 1) = CreatePen(PS_SOLID, 1, plotcol.at(i));
    }

    gdi.nref++;
}

void Graph::FreeGDI(){

    gdi.nref--;

    if(!gdi.nref){
        DWORD i;
        for(i = 0; i < 2; i++){
            DeleteObject(gdi.font.at(i));
        }

        for(i = 0; i < 2 + MAXPLOT; i++){
            DeleteObject(gdi.pen.at(i));
        }

    }

}

void Graph::AppendDataXY(POINT point){

    if(idpt < 10){
        points.at(idpt) = point;
        idpt++;
    }

}

void Graph::ClearPoints(){

    for(UINT i = 0; i < 10; i++){

        points.at(i).x = 0;
        points.at(i).y = 0;

        original_points.at(i).x = 0;
        original_points.at(i).y = 0;
    }

    idpt = 0;
}

void Graph::Refresh(HDC hDC, int w, int h){

    int x0 = 0, x1 = w;
    int y0 = h, y1 = 0;

    double topmargin = 15;
    double bottommargin = 15;
    double rightmargin = 15;

    HFONT pfont = (HFONT)SelectObject(hDC, gdi.font.at(0));

    for(UINT i = 0; i < 10; i++){
        if(points.at(i).x < 0){
            points.at(i).x = 0;
        }

        if(points.at(i).y < 0){
            points.at(i).y = 0;
        }

        original_points.at(i) = points.at(i);
    }

    for(UINT idrem=idpt;idrem<10;idrem++){
			points.at(idrem).x = points.at(idpt).x;
			points.at(idrem).y = 0;
    }

    // draw data

    SelectObject (hDC, gdi.pen[0]);
	
	float MaxX = 0;

	for(UINT i = 0; i < 10; i++){
        if(points.at(i).x > MaxX){
            MaxX = points.at(i).x;
        }
    }
	
    double cnvx = (w - rightmargin) / (double)MaxX;
			
    float MaxY = 0;

	for(UINT i = 0; i < 10; i++){
		if(points.at(i).y > MaxY){
            MaxY=points.at(i).y;
        }
	}
	
    double cnvy = (h - topmargin - bottommargin) / (double)100;
			
	for(UINT i = 0; i < 10; i++){
	    points.at(i).x *= cnvx;
		points.at(i).y *= cnvy;
    }
			
    for(UINT i = 0; i < 10; i++){				
		points.at(i).y = (h) - points.at(i).y - bottommargin;
	}
			
	Polyline(hDC, points.data(), idpt);

	for(UINT i=0;i<idpt;i++){
        std::string buff = std::format("t:{} P:{}",original_points.at(i).x,original_points.at(i).y);

		TextOut(hDC, points.at(i).x,points.at(i).y - 15, buff.c_str(), static_cast<int>(buff.size()));
	}

    //Draw axes
    SelectObject(hDC, GetStockObject(BLACK_PEN));
    MoveToEx(hDC, x0, y1, nullptr);
    LineTo(hDC, x0, y0);
    MoveToEx(hDC, x0, y0 - bottommargin, nullptr);
    LineTo(hDC, x1, y0 - bottommargin);

    SelectObject(hDC, pfont);

}

GDIres Graph::gdi = {0,0,0};