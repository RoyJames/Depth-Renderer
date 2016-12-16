
#include "GLBezierSurface.h"

void GLBezierSurface::eval_bez(std::vector<vec3> &Q, float u, vec3 &vertex, vec3 &norm, bool isu) {
    int dim = isu ? _udeg : _vdeg;
    if(dim == 2) {
        norm = Q[1] - Q[0];
    }
    for(int k = 1; k < dim; ++k) {
        for(int i = 0; i < dim - k; ++i) {
            Q[i] = (1.0f-u) * Q[i] + u * Q[i+1];
        }
        if(dim == k+2) {
            norm = Q[1] - Q[0];
        }
    }
    vertex = Q[0];
}

void GLBezierSurface::initialize_Q(std::vector<vec3> &Q, bool isu, int index) {
    if(isu) {
        Q.resize((size_t) _udeg);
        for(int i = 0; i < _udeg; ++i)
            Q[i] = (*this)[index][i];
    } else {
        Q.resize((size_t) _vdeg);
        for(int i = 0; i < _vdeg; ++i)
            Q[i] = (*this)[i][index];
    }
}

void GLBezierSurface::eval_surface(float u, float v, vec3 &vertex, vec3 &norm) {

    std::vector<vec3> q((size_t) _vdeg);
    std::vector<vec3> Q((size_t) _udeg);

    //First pass
    for(int i = 0; i < _vdeg; ++i) {
        initialize_Q(Q, true, i);
        vec3 ignored;
        eval_bez(Q, u, q[i], ignored, true);
    }

    //Second pass
    vec3 vtangent;
    eval_bez(q, v, vertex, vtangent, false);

    //First pass
    q.resize((size_t) _udeg);
    for(int i = 0; i < _udeg; ++i) {
        initialize_Q(Q, false, i);
        vec3 ignored;
        eval_bez(Q, v, q[i], ignored, false);
    }

    //Second pass
    vec3 utangent;
    vec3 ignored;
    eval_bez(q, u, ignored, utangent, true);

//    cout << "(u,v) " << "(" << u << "," << v << "), utangent" << utangent << ", vtangent" << vtangent << ", cross" << cross(utangent, vtangent) << endl;
    norm = normalize(cross(utangent, vtangent));
}

void GLBezierSurface::getTriangles(int resolution, std::vector<vec3>& faces, std::vector<vec3> &vertices, std::vector<vec3> &norms, std::vector<vec2> &uvs) {
	int udim = max(2, _udeg + resolution);
    int vdim = max(2, _vdeg + resolution);

    for(int ru = 0; ru <= udim-1; ++ru) {
        float u = 1.0f * ru / (udim-1);
        for(int rv = 0; rv <= vdim-1; ++rv) {
            float v = 1.0f * rv / (vdim-1);
            vec3 vertex(0,0,0), norm(0,0,0);

            eval_surface(u, v, vertex, norm);
            vertices.push_back(vertex);
            norms.push_back(norm);
            uvs.push_back(vec2(u, v));
        }
    }

    for(int ru = 0; ru < udim - 1; ++ru) {
        for(int rv = 0; rv < vdim - 1; ++rv) {
            vec3 face1, face2;
            //face1
            face1.x = ru      * vdim + rv;
            face1.y = ru      * vdim + (rv+1);
            face1.z = (ru+1)  * vdim + (rv+1);
            //face2
            face2.x = (ru+1)  * vdim + (rv+1);
            face2.y = (ru+1)  * vdim + rv;
            face2.z = ru      * vdim + rv;

            faces.push_back(face1);
            faces.push_back(face2);
        }
    }
}