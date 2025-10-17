#pragma once
#include <DirectXMath.h>
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include "glm/gtx/euler_angles.hpp"

struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    [[nodiscard]] DirectX::XMMATRIX GetViewMatrix() const {
        const DirectX::XMVECTOR eye = XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&position));
        const DirectX::XMVECTOR forward = ForwardDX();
        const DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
        return DirectX::XMMatrixLookToLH(eye, forward, up);
    }

    [[nodiscard]] DirectX::XMMATRIX GetMatrixDX() const {
        const glm::mat4 glmMat = GetMatrix();
        return DirectX::XMMATRIX(reinterpret_cast<const float*>(&glmMat));
    }

    [[nodiscard]] glm::mat4 GetMatrix() const {
        glm::mat4 matrix = translate(glm::mat4(1.0f), position);
        matrix = rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
        matrix = rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
        matrix = rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
        matrix = glm::scale(matrix, scale);
        return matrix;
    }

    [[nodiscard]] DirectX::XMVECTOR ForwardDX() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        const glm::vec3 forward = normalize(glm::vec3(rotationMatrix * glm::vec4(0, 0, 1, 0)));
        return DirectX::XMVectorSet(forward.x, forward.y, forward.z, 0.0f);
    }

    [[nodiscard]] glm::vec3 Forward() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return normalize(glm::vec3(rotationMatrix * glm::vec4(0, 0, 1, 0)));
    }

    [[nodiscard]] glm::vec3 Backward() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return normalize(glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 0)));
    }

    [[nodiscard]] glm::vec3 Right() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return normalize(glm::vec3(rotationMatrix * glm::vec4(1, 0, 0, 0)));
    }

    [[nodiscard]] glm::vec3 Left() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return normalize(glm::vec3(rotationMatrix * glm::vec4(-1, 0, 0, 0)));
    }

    [[nodiscard]] glm::vec3 Up() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return normalize(glm::vec3(rotationMatrix * glm::vec4(0, 1, 0, 0)));
    }

    [[nodiscard]] glm::vec3 Down() const {
        const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return normalize(glm::vec3(rotationMatrix * glm::vec4(0, -1, 0, 0)));
    }
};
