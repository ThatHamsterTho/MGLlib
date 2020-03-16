#include "Core/3Drender/Camera.h"

namespace MGLlib { 

Camera::Camera(std::array<float, 3> pos, std::array<float, 3> lookat)
    : FOV(70), aspectratio(1.0f), Zplane({0.1f, 20.0f})   {
    SetUp({0, 1, 0});
    Position({pos[0], pos[1], pos[2]});
    Lookat({lookat[0], lookat[1], lookat[2]});
    UpdateCamera();
    UpdatePerspective();
}
Camera::~Camera(){}

void Camera::Position(glm::vec3 pos){
    this->pos[0] = pos.x;
    this->pos[1] = pos.y;
    this->pos[2] = pos.z;
}

void Camera::Lookat(glm::vec3 lookat){
    this->lookat[0] = lookat.x;
    this->lookat[1] = lookat.y;
    this->lookat[2] = lookat.z;
}

void Camera::SetFOV(float FOV){
    this->FOV = FOV;
}

void Camera::SetUp(glm::vec3 up){
    this->up[0] = up.x;
    this->up[1] = up.y;
    this->up[2] = up.z;
}

void Camera::UpdatePerspective(void){
    this->pers = glm::perspective(this->FOV, this->aspectratio, this->Zplane[0], this->Zplane[1]);
}

void Camera::UpdateCamera(void){
    this->view = glm::lookAt(
        glm::vec3(this->pos[0], this->pos[1], this->pos[2]),
        glm::vec3(this->lookat[0], this->lookat[1], this->lookat[2]), 
        glm::vec3(this->up[0], this->up[1], this->up[2]));
    updated = true;
}

//! @brief turns back if camera has updated since last read
bool Camera::CheckUpdated(void){
    bool t = this->updated;
    this->updated = false;
    return t;
}

void Camera::UpdateAspectRatio(float width, float height){
    if(height == 0){
        this->aspectratio = width/1.0;
    }
    else{
        this->aspectratio = width/height;
    }
}

// both values should be positive
void Camera::SetPlane(float NearPlane, float FarPlane){
    #ifdef CAMERAWARNINGS
    if(NearPlane < 0 || FarPlane < 0){
        pWARNING("Plane values are negative, this will result in a blank screen");
    }
    #endif
    this->Zplane[0] = NearPlane;
    this->Zplane[1] = FarPlane;
}

} // namespace