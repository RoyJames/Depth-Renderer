//
// Created by David on 4/10/2016.
//

#ifndef GLRENDERSCENE_H
#define GLRENDERSCENE_H
#include <GL/gl3w.h>

#define timeInc 10

class GLScene;

#include "GLShaderLoader.h"
#include <vector>
#include "GLTriangle.h"
#include "GLVector.h"
#include "GLMatrix.h"
#include "GLOptions.h"
#include "GLParser.h"
#include <SOIL/SOIL.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>


typedef vec4  point4;
typedef vec4  color4;
typedef vec3  face;
typedef vec3  vertex;

class GLScene {
private:
    std::vector<vertex> _vertices;
    std::vector<face> _faces;

    size_t _numVertices;

    GLuint *_buffers; // space for the name of one buffer object
    point4 *_points;

    float _posx, _posy, _thetaX, _thetaY, _thetaZ;
    int _lastx, _lastY, _lastz, _distance;

    GLint _matrixLoc, _viewerLoc, _textureLoc;
    GLint _vNormLoc, _vPositionLoc;

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

    void constructTransformMatrix();
public:
    GLScene(GLOptions options);
    ~GLScene();

    void addFace(int v1, int v2, int v3);
    void addVertex(float p1, float p2, float p3);
    void loadFromFile();

    void load();
    void reload();
    void update();

    void updateSurfaces();
    void reloadSurfaces();

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

    void assignPointsFromSurfaces();

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
    bool hasMoreSnapshots();

    void handleObjs(std::vector<vec3> &vertices, std::vector<vec3> &faces);
    void renderOffScreen();
};

#endif //GLRENDERSCENE_H
