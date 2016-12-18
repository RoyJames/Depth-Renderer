//
// Created by David on 4/10/2016.
//

#include "GLMatrix.h"

mat2::mat2(const mat2 &m) {
    if ( *this != m ) {
        _m[0] = m._m[0];
        _m[1] = m._m[1];
    }
}

mat2 mat2::operator*(const mat2 &m) const {
    mat2  a( 0.0 );

    for ( int i = 0; i < 2; ++i ) {
        for ( int j = 0; j < 2; ++j ) {
            for ( int k = 0; k < 2; ++k ) {
                a[i][j] += _m[i][k] * m[k][j];
            }
        }
    }

    return a;
}

mat2 &mat2::operator*=(const mat2 &m) {
    mat2  a( 0.0 );

    for ( int i = 0; i < 2; ++i ) {
        for ( int j = 0; j < 2; ++j ) {
            for ( int k = 0; k < 2; ++k ) {
                a[i][j] += _m[i][k] * m[k][j];
            }
        }
    }

    return *this = a;
}
