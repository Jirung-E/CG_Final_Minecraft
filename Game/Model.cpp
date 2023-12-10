#pragma warning(disable: 4996)

#include "Model.h"

#include "../Util/Util.h"

using namespace std;


Vertex::Vertex(int point, int normal, int texture_coord) : point { point }, normal { normal }, texture_coord { texture_coord } {
}

Vertex::Vertex(int point, int normal) : Vertex { point, normal, point } {

}

Vertex::Vertex(int n) : Vertex { n, n, n } {

}


Model::Model(const string& file_name, float size) {
    load(file_name, size);
}

Model::Model() {

}


void Model::move(const Vector3& vector) {
    for(auto& p : points) {
        p += vector;
    }
}


void Model::load(const std::string& file_name, float size) {
    char count[128];
    int vertexnum = 0;
    int facenum = 0;
    int uvnum = 0;
    int normalnum = 0;

    FILE* path = fopen(file_name.c_str(), "r");
    //--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
    while(!feof(path)) {
        if(fscanf(path, "%s", count)) {
            if(count[0] == 'v' && count[1] == '\0')
                vertexnum++;
            else if(count[0] == 'f' && count[1] == '\0')
                facenum++;
            else if(count[0] == 'v' && count[1] == 't' && count[3] == '\0')
                uvnum++;
            else if(count[0] == 'v' && count[1] == 'n' && count[3] == '\0')
                normalnum++;
            memset(count, '\0', sizeof(count));
        }
        else {
            break;
        }
    }
    rewind(path);
    int vertIndex = 0;
    int faceIndex = 0;
    int uvIndex = 0;
    int normalIndex = 0;
    //--- 2. 메모리 할당
    points.resize(vertexnum);
    polygons.resize(facenum);
    uvs.resize(facenum);
    texture_coords.resize(uvnum);
    normals.resize(normalnum);
    char bind[128];

    //--- 3. 할당된 메모리에 각 버텍스, 페이스, uv 정보 입력
    while(!feof(path)) {
        if(fscanf(path, "%s", bind)) {
            if(bind[0] == 'v' && bind[1] == '\0') {
                if(fscanf(path, "%f %f %f\n", &points[vertIndex].x, &points[vertIndex].y, &points[vertIndex].z)) {
                    points[vertIndex] *= size;
                    vertIndex++;
                }
            }
            else if(bind[0] == 'f' && bind[1] == '\0') {
                unsigned int vertex[3], temp_uv[3], temp_normal[3];
                if(fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                    &vertex[0], &temp_uv[0], &temp_normal[0],
                    &vertex[1], &temp_uv[1], &temp_normal[1],
                    &vertex[2], &temp_uv[2], &temp_normal[2])) {
                    //uvs[faceIndex].x = temp_uv[0];
                    //uvs[faceIndex].y = temp_uv[1];
                    //uvs[faceIndex].z = temp_uv[2];
                    polygons[faceIndex].v1.point = vertex[0]-1;
                    polygons[faceIndex].v2.point = vertex[1]-1;
                    polygons[faceIndex].v3.point = vertex[2]-1;
                    polygons[faceIndex].v1.normal = temp_normal[0]-1;
                    polygons[faceIndex].v2.normal = temp_normal[1]-1;
                    polygons[faceIndex].v3.normal = temp_normal[2]-1;
                    polygons[faceIndex].v1.texture_coord = temp_uv[0]-1;
                    polygons[faceIndex].v2.texture_coord = temp_uv[1]-1;
                    polygons[faceIndex].v3.texture_coord = temp_uv[2]-1;
                    faceIndex++;
                }
            }
            else if(bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
                if(fscanf(path, "%f %f\n", &texture_coords[uvIndex].x, &texture_coords[uvIndex].y)) {
                    uvIndex++;
                }
            }
            else if(bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
                if(fscanf(path, "%f %f %f\n", &normals[normalIndex].x, &normals[normalIndex].y, &normals[normalIndex].z)) {
                    normalIndex++;
                }
            }
        }
        else {
            break;
        }
    }

    fclose(path);
}


// Stocks
const Model Model::box { "Resource/Models/Box.obj" };
const Model Model::sphere { "Resource/Models/Sphere.obj" };
