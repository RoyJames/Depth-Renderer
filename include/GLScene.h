//
// Created by David on 4/10/2016.
//

#ifndef GLRENDERSCENE_H
#define GLRENDERSCENE_H

#define timeInc 10

class GLScene;

#include "GLBezierSurface.h"
#include <vector>
#include "GLTriangle.h"
#include "GLVector.h"
#include "GLMatrix.h"
#include "GLOptions.h"
#include "GLParser.h"
#include "GLShaderLoader.h"
#include <SOIL/SOIL.h>

typedef vec4  point4;
typedef vec4  color4;
typedef vec3  face;
typedef vec3  vertex;

class GLScene {
private:
    std::vector<vertex> _vertices;
    std::vector<face> _faces;
    std::vector<GLBezierSurface> _surfaces;

    size_t _numVertices;

    GLuint *_buffers; // space for the name of one buffer object
    point4 *_points;
    vec4 *_norms;
    vec2 *_uvs;

    float _posx, _posy, _thetaX, _thetaY, _thetaZ;
    int _lastx, _lastY, _lastz, _distance;

    GLint _matrixLoc, _viewerLoc, _worldtimeLoc, _textureLoc;
    GLint _vNormLoc, _vPositionLoc, _uvLoc;
    int _worldTime;
    GLint _checkerboardResolutionLoc, _isCheckerboardLoc;

    int _resolution;

    GLfloat _checkerboardResolution;
    GLboolean _isCheckerboard;

    point4 _lightPos;
    color4 _lightAmb, _lightDiff, _lightSpec;
    color4 _materialAmb, _materialDiff, _materialSpec;

    float _materialShininess;

    float _fovx;
    float _fovy;
    float _aspect;
    float _zNear;
    float _zFar;
    mat4 _rot;
    vec3 _rpy;

    int _depth_images_count;
    int _total_depth_images;

    mat4 _ctm;
    vec4 _viewer;

    GLOptions _options;

    bool _isBezier;
    bool _isObj;

    void constructTransformMatrix();
public:
    GLScene(GLOptions options);
    ~GLScene();

    void addSurface(GLBezierSurface &bsurface);
    void addFace(int v1, int v2, int v3);
    void addVertex(float p1, float p2, float p3);
    void loadFromFile();

    void load();
    void reload();
    void update();

    void updateSurfaces();
    void reloadSurfaces();
    void reloadTexture();

    void loadShaders();

    void setThetaX(float thetaX);
    void setThetaY(float thetaY);
    void setThetaZ(float thetaZ);
    void setPosX(float posx);
    void setPosY(float posy);
    void setLastX(int lastx);
    void setLastZ(int lastz);
    void setLastY(int y);
    void decrementDistance();
    void incrementDistance();
    void resetDistance();

    void increaseResolution();
    void decreaseResolution();
    void resetResolution();
    void assignPointsFromSurfaces();

    void toggleCheckerboard();
    void increaseCheckerboardResolution();
    void decreaseCheckerboardResolution();

    float getPosX();
    float getPosY();
    float getThetaX();
    float getThetaY();
    float getThetaZ();
    int getLastX();
    int getLastY();
    int getLastZ();

    void incrementTime();
    int screenShot(int const num);

    void setBezier();
    void setObj();
    void handleObjs(std::vector<vec3> &vertices, std::vector<vec3> &norms, std::vector<vec3> &faces);
    void handleBeziers(std::vector<vec3> &vertices, std::vector<vec3> &norms, std::vector<vec3> &faces, std::vector<vec2> &uvs);
};


//////////////////////////////////////////////////
    // Grab the OpenGL screen and save it as a .tga //
    // Copyright (C) Marius Andra 2001              //
    // http://cone3d.gz.ee  EMAIL: cone3d@hot.ee    //
    //////////////////////////////////////////////////
    // (modified by me a little)


#endif //GLRENDERSCENE_H
