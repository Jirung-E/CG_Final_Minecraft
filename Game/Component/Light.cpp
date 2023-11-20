#include "Light.h"

using namespace std;


Light::Light(Object* object, const string& id) : ComponentImpl { object, id },
    position { }, ambient { 0.1f } {

}


void Light::update(float dt) {

}
