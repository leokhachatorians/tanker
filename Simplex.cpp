#include "Simplex.h"

Simplex::Simplex() {
    F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
    G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

    for (int i = 0; i < 512; i++) {
        perm[i] = p[i & 255];
        permMod12[i] = (short)(perm[i] % 12);
    }
}

double Simplex::dot(int g[], double x, double y) {
    double x_value = g[0] * x;
    double y_value = g[1] * y;
    return x_value + y_value;
}

int Simplex::fast_floor(double x) {
    int xi = (int) x;
    return x < xi ? xi-1 : xi;
}

double Simplex::simplex_noise(double xin, double yin) {
    double n0, n1, n2;
    int i1, j1;

    double s = (xin + yin) * F2;
    int i = fast_floor(xin + s);
    int j = fast_floor(yin + s);

    double t = (i + j) * G2;
    double X0 = i - t;
    double Y0 = j - t;
    double x0 = xin - X0;
    double y0 = yin - Y0;
    
    if (x0 > y0) {
        i1 = 1;
        j1 = 0;
    }
    else {
        i1 = 0;
        j1 = 1;
    }

    double x1 = x0 - i1 + G2;
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2;
    double y2 = y0 - 1.0 + 2.0 * G2;

    int ii = i & 255;
    int jj = j & 255;
    int gi0 = permMod12[ii + perm[jj]];
    int gi1 = permMod12[ii + i1 + perm[jj + j1]];
    int gi2 = permMod12[ii + 1 + perm[jj + 1]];

    double t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 < 0) {
        n0 = 0.0;
    }
    else {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad3[gi0], x0, y0);
    }

    double t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 < 0) {
        n1 = 0.0;
    }
    else {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
    }

    double t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 < 0) {
        n2 = 0.0;
    }
    else {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
    }
    return 45.23065f * (n0 + n1 + n2);
}

double Simplex::brownian(int iters, double x, double y, double pers, double scale, double low, double high) {
    double max_amp = 0;
    double amp = 1;
    double freq = scale;
    double noise = 0;

    for (int i = 0; i < iters; ++i) {
        noise += simplex_noise(x * freq, y * freq) * amp;
        max_amp += amp;
        amp *= pers;
        freq *= 2;
    }

    noise /= max_amp;

    noise = noise * (high - low) / 2 + (high + low) / 2;

    return noise;
}
