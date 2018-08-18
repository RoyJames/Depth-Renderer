//
// Created by David on 4/10/2016.
//

#include "GLScene.h"
#include <ctime>

GLScene::GLScene(GLOptions options) : _options(options) {
    //One for VColor, one for VPosition
    _buffers = new GLuint[3];
    _matrixLoc = 0;
    _vPositionLoc = 0;

    _posx = 0.0;
    _posy = 0.0;
    _thetaY = 0.0;
    _thetaX = 0.0;
    _thetaZ = 0.0;
    _distance = 2.0f;
    _lastx = 0;
    _lastY = 0;
    _lastz = 0;

    _numVertices = 0;

    _points = 0;

    _fovx = 70;
//    _fovy = 45.f;
    _fovy = 15.f * M_PI / 180;
//    _aspect= 4.f / 3.f;
    _aspect = 1.f;
    _zNear = 0.1f;
    _zFar = 10.f;
    _zcoeff_a = _zFar / (_zFar - _zNear);
    _zcoeff_b = _zFar * _zNear / (_zNear - _zFar);

    _depth_images_count = 0;
    _total_depth_images = _options._numCloudsToRender;

    _MVP = glm::mat4(1.0f);
}

void GLScene::constructTransformMatrix() {
    // TODO: check correctness of using precomputed viewpoints when constructing up vector
    glm::vec3 viewer = glm::vec3(0.0f, 0.0f, _distance);
    glm::vec3 center = glm::vec3(0.0f,0.0f,0.0f);
//
//    //Construct the up vector
//    glm::vec3 viewer_to_center = viewer - center;
//    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, _distance), glm::vec3(1.0f)));
//    glm::vec3 up = glm::normalize(glm::cross(right, viewer_to_center));

    if (_viewpoints.size() > 0){
        viewer = _viewpoints[_depth_images_count];
    }else{
        //Rotate the camera's position around the center
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        _thetaX = static_cast<float>(dis(gen) * 2.0 * M_PI);
        _thetaY = static_cast<float>(dis(gen) * 2.0 * M_PI);
//    const float z_angle = static_cast<float>(dis(gen) * M_PI);

        float camX = _distance * -sinf(_thetaX) * cosf(_thetaY);
        float camY = _distance * -sinf(_thetaY);
        float camZ = -_distance * cosf(_thetaX) * cosf(_thetaY);

        viewer = glm::vec3(camX, camY, camZ);
    }
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    //Update the transform matrix
    _MVP = glm::mat4(1.0f);
    _MVP *= glm::perspective(_fovy, _aspect, _zNear, _zFar);
    _MVP *= glm::lookAt(viewer, center, up);

    //Update matrix for untransforming the vector
    glm::vec3 unProjectViewer = glm::vec3(0.0f, 0.0f, _distance);

    _UnProjectMVP = glm::mat4(1.0f);
    _UnProjectMVP *= glm::perspective(_fovy, _aspect, _zNear, _zFar);
    _UnProjectMVP *= glm::lookAt(unProjectViewer, center, up);
}

GLScene::~GLScene() {
    delete [] _buffers;
    if(_points)
        delete [] _points;
}

void GLScene::load() {
    GLuint vao;

    glGenVertexArrays( 1, &vao );   // give us 1 VAO:
    glBindVertexArray( vao );       // make it active

    glGenBuffers(1, _buffers);
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);  // make it active

    loadSurfacesFromFile();
    if(_options._verbose)
        cout << "Updated surfaces" << endl;

    if(_options._verbose)
        cout << "Buffering data" << endl;

    //Set vposition attribute
    glVertexAttribPointer(_vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    size_t size = _numVertices * sizeof(glm::vec4);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER, 0, _numVertices * sizeof(glm::vec4), _points );

    loadShaders();
}

void GLScene::loadShaders() {
    GLuint program; // name of the shader program

    program = InitShader("/usr/include/depth_renderer/vshader_passthrough.glsl",
                         "/usr/include/depth_renderer/fshader_passthrough.glsl");
    glUseProgram(program);

    _vPositionLoc = (GLuint) glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(_vPositionLoc);

    //Set vposition attribute
    glVertexAttribPointer(_vPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    _matrixLoc = (GLuint) glGetUniformLocation(program, "ctm");
    glUniformMatrix4fv(_matrixLoc, 1, GL_FALSE, glm::value_ptr(_MVP));
}

void GLScene::update() {
    constructTransformMatrix();

    glUniformMatrix4fv(_matrixLoc, 1, GL_FALSE, glm::value_ptr(_MVP));
    glDrawArrays(GL_TRIANGLES, 0, _numVertices);
}

void GLScene::handleObjs(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &faces) {
    faces.resize(_faces.size());
    vertices.resize(_vertices.size());
    for(size_t i = 0; i < _faces.size(); ++i) {
        faces[i] = _faces[i];
    }

    for(size_t i = 0; i < _vertices.size(); ++i) {
        vertices[i] = _vertices[i];
    }
}

int GLScene::screenShot() {
    std::clock_t start;
    start = std::clock();
    double duration;
    // we will store the image data here
    float *pixels;
    // the thingy we use to write files        // we get the width/height of the screen into this array
    int screenStats[4];

    // get the width/height of the window
    glGetIntegerv(GL_VIEWPORT, screenStats);

    // generate an array large enough to hold the pixel data
    // (width*height*bytesPerPixel)
    pixels = new float[screenStats[2]*screenStats[3]];
    // read in the pixel data, TGA's pixels are BGR aligned
    glReadPixels(0, 0, screenStats[2], screenStats[3], GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

    std::size_t image_width( screenStats[2] );
    std::size_t image_height( screenStats[3] );

    int cloud_size = 0;
    for ( std::size_t y(0); y != image_height; ++y ) {
        for ( std::size_t x(0); x != image_width; ++x ) {
            float * rgb( pixels + (y * image_width + x) );
            bool valid_point = rgb[0] !=  1 && rgb[0] !=  0;
            if (valid_point) {
                cloud_size++;
            }
        }
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"printf: "<< duration <<'\n';

    if(cloud_size == 0) {
        std::cout << "Empty point cloud!!";
        return 0;
    }

    pcl::PointCloud<pcl::PointXYZ> cloud;
    cloud.width = (uint32_t) cloud_size;
    cloud.height   = 1;
    cloud.is_dense = false;
    cloud.points.resize (cloud.width * cloud.height);

    // float fov_x_deg = 70;
    // float fov_y_deg = 60;
    // focal_length_x = 256 / (tan(fov_x / 2))
    // focal_length_y = 212 / (tan(fov_y / 2))
    float focal_length_x = 365.605889725f;
    float focal_length_y = 367.194771204f;

    int i = 0;
    for ( std::size_t v(0); v != image_height; ++v )
    {
        for ( std::size_t u(0); u != image_width; ++u )
        {
            float * rgb( pixels + (v * image_width + u) );
            float clipped_depth_val = rgb[0];

            bool valid_point = clipped_depth_val != 1 && clipped_depth_val != 0;
            if (valid_point){
                glm::vec3 xyz = glm::unProject(glm::vec3(u, v, clipped_depth_val), _UnProjectMVP, glm::mat4(1.0f), glm::vec4(0, 0, image_width, image_height));

                cloud.points[i].x = xyz.x;
                cloud.points[i].y = xyz.y;
                cloud.points[i].z = xyz.z;
                ++i;
            }
        }
    }
    std::stringstream ss_pcd;
    ss_pcd << _options._output_dir;
    ss_pcd << _depth_images_count ;
    ss_pcd << ".pcd";

    pcl::io::savePCDFileBinary(ss_pcd.str().c_str(), cloud);

    std::stringstream ss_info;
    ss_info << _options._output_dir;
    ss_info << "info" ;
    ss_info << ".csv";

    std::stringstream ss_pose;
    ss_pose << _options._output_dir;
    ss_pose << "pose" ;
    ss_pose << ".csv";

    std::ofstream posefile;
    std::ofstream infofile;

    if (_depth_images_count == 0)
    {
        //open without append
        posefile.open(ss_pose.str().c_str());
        infofile.open(ss_info.str().c_str());
        posefile << "id, ";
        posefile << "R00, R01, R02, R10, R11, R12, R20, R21, R22, ";
        posefile << "roll, pitch, yaw" << std::endl;;

        infofile << "distance, ";
        infofile << "fx, fy, ";
        infofile << "fov_x_deg, fov_y_deg, ";
        infofile << "xdim, ydim, ";
        infofile << "aspect, zNear, zFar " << std::endl;

        infofile << _distance << ", ";
        infofile <<  focal_length_x << ", "  << focal_length_y << ", ";
        infofile << 70 << ", "  << 60 << ", ";
        infofile << 512 << ", "  << 424 << ", ";
        infofile << _aspect << ", "  << _zNear << ", " <<  _zFar << std::endl;
        infofile.close();
    }
    else{
        //Open to append
        posefile.open(ss_pose.str().c_str(), std::ios_base::app);
        infofile.open(ss_info.str().c_str(), std::ios_base::app);
    }

    // myfile << "id, "
    posefile << _depth_images_count  << ", ";
    // myfile << "R00, R01, R02,R10, R11, R12,R20, R21, R22, "
    posefile << _MVP[0][0]  << ", ";
    posefile << _MVP[0][1]  << ", ";
    posefile << _MVP[0][2]  << ", ";
    posefile << _MVP[0][3]  << ", ";
    posefile << _MVP[1][0]  << ", ";
    posefile << _MVP[1][1]  << ", ";
    posefile << _MVP[1][2]  << ", ";
    posefile << _MVP[1][3]  << ", ";
    posefile << _MVP[2][0]  << ", ";
    posefile << _MVP[2][1]  << ", ";
    posefile << _MVP[2][2]  << ", ";
    posefile << _MVP[2][3]  << ", ";
    posefile << _MVP[3][0]  << ", ";
    posefile << _MVP[3][1]  << ", ";
    posefile << _MVP[3][2]  << ", ";
    posefile << _MVP[3][3]  << ", ";
    // myfile << "roll, pitch, yaw, ";
    posefile << _thetaX << ", ";
    posefile << _thetaY  << ", ";
    posefile << 0.0f << std::endl;;

    posefile.close();

    _depth_images_count++;

    // free the memory
    delete [] pixels;
    // return success
    return 0;
}

int GLScene::saveDepthMap() {
    std::clock_t start;
    start = std::clock();
    double duration;
    // we will store the image data here
    float *pixels;
    // the thingy we use to write files        // we get the width/height of the screen into this array
    int screenStats[4];

    boost::filesystem::path ss_depth(_options._output_dir);
    std::stringstream filename;
    filename << "view" << _depth_images_count << ".depth";
    ss_depth = ss_depth / boost::filesystem::path(filename.str());
    if ( boost::filesystem::exists( ss_depth ) )
    {
        std::cout << "Skipping existing file " << ss_depth << std::endl;
        _depth_images_count++;
        return 0;
    }

    // get the width/height of the window
    glGetIntegerv(GL_VIEWPORT, screenStats);

    // generate an array large enough to hold the pixel data
    // (width*height*bytesPerPixel)
    pixels = new float[screenStats[2]*screenStats[3]];
    // read in the pixel data, TGA's pixels are BGR aligned
    glReadPixels(0, 0, screenStats[2], screenStats[3], GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

    std::size_t image_width( screenStats[2] );
    std::size_t image_height( screenStats[3] );

    int cloud_size = 0;
    for ( std::size_t y(0); y != image_height; ++y ) {
        for ( std::size_t x(0); x != image_width; ++x ) {
            float * rgb( pixels + (y * image_width + x) );
            bool valid_point = rgb[0] !=  1 && rgb[0] !=  0;
            if (valid_point) {
                cloud_size++;
            }
        }
    }

    if(cloud_size == 0) {
        cerr << "Skip empty point cloud at view " << _depth_images_count << endl;
        delete[] pixels;
        _depth_images_count++;
        return 1;
    }

    // float fov_x_deg = 70;
    // float fov_y_deg = 60;
    // focal_length_x = 256 / (tan(fov_x / 2))
    // focal_length_y = 212 / (tan(fov_y / 2))
    float focal_length_x = 365.605889725f;
    float focal_length_y = 367.194771204f;


    std::ofstream depthfile;
//    depthfile.open(ss_depth.str().c_str());
    depthfile.open(ss_depth.c_str());
    depthfile << image_height << " " << image_width << " " << cloud_size << std::endl;
    for ( std::size_t v(0); v != image_height; ++v )
    {
        for ( std::size_t u(0); u != image_width; ++u )
        {
            float * rgb( pixels + (v * image_width + u) );
            float clipped_depth_val = rgb[0];

            bool valid_point = clipped_depth_val != 1 && clipped_depth_val != 0;
            if (valid_point){
                float world_z = getLinearizedZ(clipped_depth_val);
                depthfile << v << " " << u  << " " << world_z << std::endl;
            }
        }
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    if (_options._verbose) {
        std::cout << "per view time cost: " << duration << " seconds\n";
    }
    depthfile.close();

    _depth_images_count++;

    // free the memory
    delete [] pixels;
    // return success
    return 0;
}


void GLScene::loadSurfacesFromFile() {
    GLParser parser;
    cout << "Parsing filename: " << _options._filename << endl;
    parser.parse(_options._filename, *this);

    std::vector<glm::vec3> faces;
    std::vector<glm::vec3> vertices;

    _numVertices = 0;

    if(_options._verbose) cout << "Calculating obj surface" << endl;
    handleObjs(vertices, faces);

    float max_x = 0, max_y = 0;
    float min_x = 0, min_y = 0;
    for(glm::vec3 &vertex : vertices) {
        max_x = max(max_x, vertex.x);
        max_y = max(max_y, vertex.y);
        min_x = min(min_x, vertex.x);
        min_y = min(min_y, vertex.y);
    }

    _numVertices = (GLsizei) faces.size() * 3;

    if(_points)
        delete [] _points;
    _points = new glm::vec4[_numVertices];

    if(_options._verbose)
        cout << "Loading values" << endl;

    for(size_t i = 0; i < faces.size(); i++) {
        glm::vec3 &face1 = vertices[(int) faces[i].x];
        glm::vec3 &face2 = vertices[(int) faces[i].y];
        glm::vec3 &face3 = vertices[(int) faces[i].z];

        _points[3*i+0] = glm::vec4(face1.x, face1.y, face1.z, 1.0f);
        _points[3*i+1] = glm::vec4(face2.x, face2.y, face2.z, 1.0f);
        _points[3*i+2] = glm::vec4(face3.x, face3.y, face3.z, 1.0f);
    }
    if(_options._verbose)
        cout << "Finished loading normals" << endl;

    parser.parseviews(_options._viewfile, *this);
    _total_depth_images = _viewpoints.size();
    if(_options._verbose)
        cout << "Finished loading viewpoints" << endl;
}

//ACCESSORS/MUTATORS

void GLScene::setThetaX(float thetaX) {
    _thetaX = thetaX;
}

void GLScene::setThetaY(float thetaY) {
    _thetaY = thetaY;
}

void GLScene::setThetaZ(float thetaZ) {
    _thetaZ = thetaZ;
}

void GLScene::setPosX(float posx) {
    _posx = posx;
}

void GLScene::setPosY(float posy) {
    _posy = posy;
}

void GLScene::setLastX(int lastx) {
    _lastx = lastx;
}

void GLScene::setLastZ(int lastz) {
    _lastz = lastz;
}

float GLScene::getPosX() {
    return _posx;
}

float GLScene::getPosY() {
    return _posy;
}

int GLScene::getLastX() {
    return _lastx;
}

int GLScene::getLastZ() {
    return _lastz;
}

float GLScene::getLinearizedZ(float z_buf_val) {
    // refer to https://sjbaker.org/steve/omniv/love_your_z_buffer.html
    return _zcoeff_b / (z_buf_val - _zcoeff_a);
}

float GLScene::getThetaX() {
    return _thetaX;
}

float GLScene::getThetaY() {
    return _thetaY;
}

float GLScene::getThetaZ() {
    return _thetaZ;
}

void GLScene::setLastY(int y) {
    _lastY = y;
}

void GLScene::decrementDistance() {
    if(_distance > 0.2f) {
        if(_distance < 2) {
            _distance -= 0.2;
        } else {
            --_distance;
        }
    }
}

void GLScene::incrementDistance() {
    if(_distance < 50.0f) {
        if(_distance <= 2.0f) {
            _distance += 0.2;
        } else {
            ++_distance;
        }
    }
}

int GLScene::getLastY() {
    return _lastY;
}

void GLScene::resetDistance() {
    _distance = 2;
}

void GLScene::addFace(int v1, int v2, int v3) {
    float size = _vertices.size();
    if (v1 >= 0 &&  v2 >= 0 &&  v3 >= 0 && v1 <= size && v2 <= size && v3 <= size) {
        _faces.push_back(glm::vec3(v1, v2, v3));
    } else {
        cerr << "Unable to find vertices: " << v1 << " " << v2 << " " << v3 << endl;
    }
}

void GLScene::addVertex(float p1, float p2, float p3) {
    _vertices.push_back(glm::vec3(p1, p2, p3));
}

void GLScene::addViewpoint(float p1, float p2, float p3) {
    _viewpoints.push_back(glm::vec3(p1, p2, p3));
}

bool GLScene::hasMoreSnapshots() {
    return _depth_images_count < _total_depth_images;
}
