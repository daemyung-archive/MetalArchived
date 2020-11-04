//
// This file is part of the "DirectX12" project
// See "LICENSE" for license information.
//

#include "camera.h"

//----------------------------------------------------------------------------------------------------------------------

auto Perspective(float fov, float aspect_ratio, float near, float far) {
    auto y = 1.0f / tanf(fov * 0.5f);
    auto x = y / aspect_ratio;
    auto z = far / (far - near);
    return simd_matrix(simd_make_float4(   x, 0.0f,      0.0f, 0.0f),
                       simd_make_float4(0.0f,    y,      0.0f, 0.0f),
                       simd_make_float4(0.0f, 0.0f,         z, 1.0f),
                       simd_make_float4(0.0f, 0.0f, z * -near, 0.0f));
}

//----------------------------------------------------------------------------------------------------------------------

auto LookAt(const simd::float3 &eye, const simd::float3 &focus, const simd::float3 &up) {
    auto Z = simd_normalize(focus - eye);
    auto X = simd_normalize(simd_cross(up, Z));
    auto Y = simd_cross(Z, X);
    auto W = simd_make_float3(-simd_dot(X, eye), -simd_dot(Y, eye), -simd_dot(Z, eye));
    return simd_matrix(simd_make_float4(X.x, Y.x, Z.x, 0.0f),
                       simd_make_float4(X.y, Y.y, Z.y, 0.0f),
                       simd_make_float4(X.z, Y.z, Z.z, 0.0f),
                       simd_make_float4(W.x, W.y, W.z, 1.0f));
}

//----------------------------------------------------------------------------------------------------------------------

Camera::Camera() {
    UpdatePosition();
    UpdateView();
    UpdateProjection();
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::ZoomBy(float amount) {
    if (abs(amount) > FLT_EPSILON) {
        if (_mode == CameraMode::kArcball) {
            _radius = std::max(_radius - amount, 1.0f);
            UpdatePosition();
            UpdateView();
        } else {
            assert(false);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::RotateBy(const simd::float2 &delta) {
    if (abs(delta.x) > FLT_EPSILON || abs(delta.y) > FLT_EPSILON) {
        if (_mode == CameraMode::kArcball) {
            _phi -= ConvertToRadians(delta.x);
            _theta = std::clamp(_theta + ConvertToRadians(delta.y), -M_PI_2, M_PI_2);
            UpdatePosition();
            UpdateView();
        } else {
            assert(false);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::SetAspectRatio(float aspect_ratio) {
    if (!NearEqual(_aspect_ratio, aspect_ratio, FLT_EPSILON)) {
        _aspect_ratio = aspect_ratio;
        UpdateProjection();
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::SetRadius(float radius) {
    if (!NearEqual(_radius, radius, FLT_EPSILON)) {
        _radius = radius;
        UpdatePosition();
        UpdateView();
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::UpdatePosition() {
    if (_mode == CameraMode::kArcball) {
        _position = {_radius * cosf(_theta) * cosf(_phi),
                     _radius * sinf(_theta),
                     _radius * cosf(_theta) * sinf(_phi)};


    } else {
        assert(false);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::UpdateProjection() {
    _projection = Perspective(_fov, _aspect_ratio, _near, _far);
}

//----------------------------------------------------------------------------------------------------------------------

void Camera::UpdateView() {
    _forward = _position - _target;
    _view = LookAt(_position, _target, simd_make_float3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------------------------------------
