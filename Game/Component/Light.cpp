#include "Light.h"

using namespace std;


Light::Light(Object* object, const string& id) : ComponentImpl { object, id },
    position { }, ambient { 0.1f }, c1 { 0.0f }, c2 { 0.0f } {

}


void Light::update(float dt) {

}
