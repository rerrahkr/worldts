//
// Created by YuzukiTsuru on 2021/2/11.
//

#ifndef WORLDJS_WORLDJSWRAPPER_H
#define WORLDJS_WORLDJSWRAPPER_H

#include <iostream>
#include <emscripten.h>

#include "WorldJS.h"
#include "Converter.h"

class WorldJSWrapper {
public:
    static EMSCRIPTEN_KEEPALIVE void DisplayInformationVal(emscripten::val x);

    static EMSCRIPTEN_KEEPALIVE emscripten::val GetInformationVal(const emscripten::val &x);

    static EMSCRIPTEN_KEEPALIVE emscripten::val W2World(const std::string &fileName);

    static EMSCRIPTEN_KEEPALIVE emscripten::val F0ToCent(emscripten::val f0);

    static EMSCRIPTEN_KEEPALIVE emscripten::val CentToF0(emscripten::val cent);
};


#endif //WORLDJS_WORLDJSWRAPPER_H
