#include "GLParser.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

using namespace std;

GLParser::GLParser() {
}

//http://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool hasEnding (std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

void GLParser::parse(const char *file, GLScene &scene) {
	if(hasEnding(file, ".obj")) {
		parseObj(file, scene);
	} else {
		cerr << "File has incorrect ending: " << file << endl;
		exit(1);
	}
}



// void GLParser::parseObj(const char *file, GLScene &scene) {
// 	ifstream in(file);
// 	if(!in.good()) {
// 		cerr << "File " << file << " not found" << endl;
// 		exit(1);
// 	}

// 	char buffer[1025];
// 	string cmd;

// 	for(int line = 1; in.good(); line++) {
// 		in.getline(buffer, 1024);
// 		buffer[in.gcount()] = 0;
// 		cmd = "";

// 		istringstream iss(buffer);
// 		iss >> cmd;
// 		if (cmd[0] == '#' || cmd.empty()) {
// 			continue;
// 		}
// 		else if(cmd == "v") {
// 			float p1, p2, p3;
// 			iss >> p1 >> p2 >> p3;
// 			scene.addVertex(p1, p2, p3);
// 		}
// 		else if(cmd == "f") {
// 			int v1i, v2i, v3i;
// 			iss >> v1i >> v2i >> v3i;
// 			scene.addFace(v1i - 1, v2i - 1, v3i - 1);
// 		}
// 		else if (cmd != "#") {
// 			//cout << "Parser error: invalid command (" << buffer << ") at line " << line << endl;
// 		}
// 	}

// 	in.close();
// }

void GLParser::parseObj(const char *file, GLScene &scene) {

	//std::string inputfile = "cornell_box.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file);

	if (!err.empty()) { // `err` may contain warning message.
	  std::cerr << err << std::endl;
	}

	if (!ret) {
	  exit(1);
	}

	for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
	      float x = attrib.vertices[3*v+0];
	      float y = attrib.vertices[3*v+1];
	      float z = attrib.vertices[3*v+2];
		  scene.addVertex(x, y, z);
	}

	for (size_t i = 0; i < shapes.size(); i++) {
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
	          tinyobj::index_t i0 = shapes[i].mesh.indices[3*f+0];
	          tinyobj::index_t i1 = shapes[i].mesh.indices[3*f+1];
	          tinyobj::index_t i2 = shapes[i].mesh.indices[3*f+2];
	          // printf("  idx[%ld] = %d/%d/%d, %d/%d/%d, %d/%d/%d. mat_id = %d\n", f,
	          //   i0.vertex_index, i0.normal_index, i0.texcoord_index,
	          //   i1.vertex_index, i1.normal_index, i1.texcoord_index,
	          //   i2.vertex_index, i2.normal_index, i2.texcoord_index,
	          //   shapes[i].mesh.material_ids[f]);
	          scene.addFace(i0.vertex_index, i1.vertex_index, i2.vertex_index);
		}

	}

}
