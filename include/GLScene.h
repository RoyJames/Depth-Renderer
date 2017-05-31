//
// Created by David on 4/10/2016.
//

#ifndef GLRENDERSCENE_H
#define GLRENDERSCENE_H

class GLScene;

#include <vector>

#include "GLShaderLoader.h"
#include "GLOptions.h"
#include "GLParser.h"
#include "amath.h"
#include <SOIL/SOIL.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

class GLScene {
private:
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _faces;

    GLsizei _numVertices;

    GLuint *_buffers; // space for the name of one buffer object
    glm::vec4 *_points;

    float _posx, _posy, _thetaX, _thetaY, _thetaZ;
    int _lastx, _lastY, _lastz;
    float _distance;

    GLuint _matrixLoc;
    GLuint _vPositionLoc;

    float _fovx;
    float _fovy;
    float _aspect;
    float _zNear;
    float _zFar;
    glm::mat4 _rot;
    glm::vec3 _rpy;

    int _depth_images_count;
    int _total_depth_images;

    glm::mat4 _MVP, _UnProjectMVP;

    GLOptions _options;

    void constructTransformMatrix();
public:
    GLScene(GLOptions options);
    ~GLScene();

    void addFace(int v1, int v2, int v3);
    void addVertex(float p1, float p2, float p3);

    void load();
    void update();

    void loadSurfacesFromFile();

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

    float getPosX();
    float getPosY();
    float getThetaX();
    float getThetaY();
    float getThetaZ();
    int getLastX();
    int getLastY();
    int getLastZ();

    int screenShot();
    bool hasMoreSnapshots();

    void handleObjs(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &faces);

private:
    void loadShaders();
};

#endif //GLRENDERSCENE_H
