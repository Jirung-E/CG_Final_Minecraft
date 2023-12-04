#include "Model.h"

#include "../Util/Util.h"

using namespace std;


Vertex::Vertex(int point, int normal, int texture_coord) : point { point }, normal { normal }, texture_coord { texture_coord } {
}

Vertex::Vertex(int point, int normal) : Vertex { point, normal, point } {

}

Vertex::Vertex(int n) : Vertex { n, n, n } {

}


Model::Model() {

}


void Model::move(const Vector3& vector) {
    for(auto& p : points) {
        p += vector;
    }
}
