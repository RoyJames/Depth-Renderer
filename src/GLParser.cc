#include "GLParser.h"

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
	if(hasEnding(file, ".txt")) {
		scene.setBezier();
		parseBezier(file, scene);
	} else if(hasEnding(file, ".obj")) {
		scene.setObj();
		parseObj(file, scene);
	} else {
		cerr << "File has incorrect ending: " << file << endl;
		exit(1);
	}
}

void GLParser::parseBezier(const char *file, GLScene &scene) {
	ifstream in(file);
	if(!in.good()) {
		cerr << "File " << file << " not found" << endl;
		exit(1);
	}

	char buffer[1025];
	string cmd;
	int num_beziers = -1;
	int u_deg = -1, v_deg = -1;

	in.getline(buffer, 1024);
	buffer[in.gcount()] = 0;
	istringstream iss1(buffer);

	iss1 >> num_beziers;
	if(num_beziers <= 0) {
		cerr << "Invalid number of bezier surfaces." << endl;
		exit(1);
	}

	for(int i = 0; i < num_beziers; ++i) {
		in.getline(buffer, 1024);
		buffer[in.gcount()] = 0;
		istringstream iss(buffer);

		iss >> u_deg >> v_deg;
		if(u_deg <= 0 || v_deg <= 0) {
			cerr << "u_deg/v_deg must be greater than 0" << endl;
			exit(1);
		}

		GLBezierSurface cur(u_deg, v_deg);

		for(int v = 0; v < (v_deg + 1); ++v) {
			in.getline(buffer, 1024);
			buffer[in.gcount()] = 0;
			istringstream iss2(buffer);

			for(int u = 0; u < (u_deg + 1); ++u) {
				float x1, x2, x3;
				iss2 >> x1 >> x2 >> x3;
				cur[v][u].x = x1;
				cur[v][u].y = x2;
				cur[v][u].z = x3;
			}
		}

		scene.addSurface(cur);
	}

	in.close();
}

void GLParser::parseObj(const char *file, GLScene &scene) {
	ifstream in(file);
	if(!in.good()) {
		cerr << "File " << file << " not found" << endl;
		exit(1);
	}

	char buffer[1025];
	string cmd;

	for(int line = 1; in.good(); line++) {
		in.getline(buffer, 1024);
		buffer[in.gcount()] = 0;
		cmd = "";

		istringstream iss(buffer);
		iss >> cmd;
		if (cmd[0] == '#' || cmd.empty()) {
			continue;
		}
		else if(cmd == "v") {
			float p1, p2, p3;
			iss >> p1 >> p2 >> p3;
			scene.addVertex(p1, p2, p3);
		}
		else if(cmd == "f") {
			int v1i, v2i, v3i;
			iss >> v1i >> v2i >> v3i;
			scene.addFace(v1i - 1, v2i - 1, v3i - 1);
		}
		else if (cmd != "#") {
			cout << "Parser error: invalid command (" << buffer << ") at line " << line << endl;
		}
	}

	in.close();
}
