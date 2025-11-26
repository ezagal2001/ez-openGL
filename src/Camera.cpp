// got this error message to define this macro in order to use the
// <glm/gtx/rotate_vector.hpp> file.
#define GLM_ENABLE_EXPERIMENTAL

#include <Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

// Default constructor
Camera::Camera()
{
    // mEye represents the camera's position,
    // assume, we are placed at the origin
    mEye = glm::vec3(0.0f, 0.0f, 0.0f);

    // assume we are looking out into the world, -z-direction
    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    // assume the camera's? "up" vector starts off aligned with the
    // primitve's y-direction
    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::setProjectionMatrix(float fovy, float aspect, float near,
                                float far)
{
    mProjectionMatrix = glm::perspective(fovy, aspect, near, far); 
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return mProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const 
{   
    // remember the mEye represents the camera's position
    return glm::lookAt(mEye, mEye + mViewDirection, mUpVector); 
}

void Camera::MouseLook(int mouseX, int mouseY)
{

    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);
    static bool firstLook = true;
    if (firstLook) {
        mOldMousePosition = currentMouse;
        firstLook = false;
    }
    
    float aFactor = 0.6;
    glm::vec3 upDown = glm::cross(mViewDirection, mUpVector);
    //glm::vec2 mouseDelta = mOldMousePosition - currentMouse;
    glm::vec2 mouseDelta = currentMouse - mOldMousePosition;
    mViewDirection = glm::rotate(mViewDirection, -aFactor * glm::radians(mouseDelta.x), mUpVector);
    mViewDirection = glm::rotate(mViewDirection, -aFactor * glm::radians(mouseDelta.y), upDown);
    mOldMousePosition = currentMouse; 
}

void Camera::MoveUp(float speed)
{
    mEye += mUpVector * speed;
}

void Camera::MoveDown(float speed)
{
    mEye += -mUpVector * speed;
}

void Camera::MoveForward(float speed)
{
    mEye += mViewDirection * speed;
}

void Camera::MoveBackward(float speed)
{
    mEye -= mViewDirection * speed;
}

void Camera::MoveLeft(float speed)
{
    glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
    mEye -= rightVector * speed;
}

void Camera::MoveRight(float speed)
{
    glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
    mEye += rightVector * speed;
}




