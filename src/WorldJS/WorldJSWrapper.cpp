//
// Created by YuzukiTsuru on 2021/2/11.
//

#include "WorldJSWrapper.h"

#include <cmath>

void WorldJSWrapper::DisplayInformationVal(emscripten::val x) {
    std::cout << "File information" << std::endl;
    std::cout << "Sampling : " << x["fs"].as<int>() << " Hz " << x["nbit"].as<int>() << " Bit" << std::endl;
    std::cout << "Length " << x["x_length"].as<int>() << "[sample]" << std::endl;
    std::cout << "Length " << static_cast<double>(x["x_length"].as<int>()) / x["fs"].as<int>() << " [sec]" << std::endl;
}

emscripten::val WorldJSWrapper::GetInformationVal(const emscripten::val &x) {
    std::string info = "File information<br>Sampling : "
                       + std::to_string(x["fs"].as<int>())
                       + " Hz "
                       + std::to_string(x["nbit"].as<int>())
                       + " Bit"
                       + "<br>Length "
                       + std::to_string(x["x_length"].as<int>())
                       + "[sample]"
                       + "<br>Length " +
                       std::to_string(static_cast<double>(x["x_length"].as<int>()) / x["fs"].as<int>())
                       + " [sec]";
    return emscripten::val(info);
}

emscripten::val WorldJSWrapper::W2World(const std::string &fileName) {
    // init return val
    // Get File Name
    const char *f = fileName.c_str();
    int fs, nbit;
    // GetAudioLength for read
    int x_length = GetAudioLength(f);

    if (x_length == 0) {
        emscripten_log(E01);
        EM_TERM();
    }

    auto x = new double[x_length];
    // Use tools/audioio.cpp wavread function
    wavread(f, &fs, &nbit, x);

    Wav2World wav2World(x, x_length, fs);

    emscripten::val ret = emscripten::val::object();
    ret.set("x", Get1XArray<double>(x, x_length));
    ret.set("fs", fs);
    ret.set("nbit", nbit);
    ret.set("x_length", x_length);
    ret.set("f0", Get1XArray<double>(wav2World.GetF0(), wav2World.GetF0Length()));
    ret.set("time_axis", Get1XArray<double>(wav2World.GetTimeAxis(), wav2World.GetF0Length()));
    ret.set("fft_size", wav2World.GetFFTSize());
    int specl = wav2World.GetFFTSize() / 2 + 1;
    ret.set("spectral", Get2XArray<double>(wav2World.GetSP(), wav2World.GetF0Length(), specl));
    ret.set("aperiodicity", Get2XArray<double>(wav2World.GetAP(), wav2World.GetF0Length(), specl));
    return ret;
}

EMSCRIPTEN_KEEPALIVE emscripten::val WorldJSWrapper::F0ToCent(emscripten::val f0) {
    int x_length;
    auto x = GetPtrFrom1XArray<double>(std::move(f0), &x_length);
    if (x_length == 0) {
        emscripten_log(E01);
        EM_TERM();
    }

    auto cent = new double[x_length];
    for (int i = 0; i < x_length; ++i) {
        cent[i] = 1200. * std::log2(x[i] / 440.);
    }

    auto ret = Get1XArray<double>(cent, x_length);

    delete[] x;
    delete[] cent;

    return ret;
}

EMSCRIPTEN_KEEPALIVE emscripten::val WorldJSWrapper::CentToF0(emscripten::val cent) {
    int x_length;
    auto x = GetPtrFrom1XArray<double>(std::move(cent), &x_length);
    if (x_length == 0) {
        emscripten_log(E01);
        EM_TERM();
    }

    auto f0 = new double[x_length];
    for (int i = 0; i < x_length; ++i) {
        f0[i] = 440. * std::pow(2, x[i] / 1200.);
    }

    auto ret = Get1XArray<double>(f0, x_length);

    delete[] x;
    delete[] f0;

    return ret;
}
