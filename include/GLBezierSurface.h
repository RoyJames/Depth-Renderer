//With help from: http://stackoverflow.com/questions/6969881/operator-overload

#ifndef GL_BEZIER_SURFACE
#define GL_BEZIER_SURFACE

#include "amath.h"
#include "GLVector.h"
#include <iostream>
#include <vector>

using namespace std;


class GLBezierSurface {
public:
    GLBezierSurface(int u_deg, int v_deg) {
        _udeg = u_deg+1;
        _vdeg = v_deg+1;

        _points = std::vector<vec3>((size_t) _vdeg * _udeg);
        for(int v = 0; v < _vdeg; ++v) {
            for(int u = 0; u < _udeg; ++u) {
                _points[v*_udeg + u] = vec3(0,0,0);
            }
        }
    }

    class Proxy {
    public:
        Proxy(std::vector<vec3>* _array, int _udeg, int _vindex) : _array(_array), _udeg(_udeg), _vindex(_vindex) { }

        vec3 &operator[](int u) {
            if(u >= 0 && u < _udeg)
                return (*_array)[_vindex * _udeg + u];
            else
                cerr << "Invalid access at lower: " << _udeg << " " << u << endl;
            return (*_array)[_vindex * _udeg + u];
        }
    private:
        std::vector<vec3>* _array;
        int _udeg;
        int _vindex;
    };

    Proxy operator[](int v) {
        if(v >= 0 && v < _vdeg)
            return Proxy(&_points, _udeg, v);
        else
            cerr << "Invalid access at top: " << _vdeg << " " << v << endl; //TODO MAKE THIS BETTER
        return Proxy(&_points, _udeg, v);
    }

    void getTriangles(int resolution, std::vector<vec3>& faces, std::vector<vec3> &vertices, std::vector<vec3> &norms, std::vector<vec2> &uvs);

private:
    std::vector<vec3> _points;
    int _udeg;
    int _vdeg;

    void eval_bez(std::vector<vec3> &Q, float u, vec3 &vertex, vec3 &norm, bool isu);
    void eval_surface(float u, float v, vec3 &vertex, vec3 &norm);
    void initialize_Q(std::vector<vec3> &Q, bool isu, int index);
};

#endif //GL_BEZIER_SURFACE