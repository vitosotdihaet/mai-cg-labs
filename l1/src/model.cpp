#include "model.hpp"

#include "matriciesop.hpp"


Model::Model(sf::ConvexShape& shape) : shape(shape), initialShapeColor(shape.getFillColor()) {
    const int pointCount = this->shape.getPointCount();
    for (int i = 0; i < pointCount; ++i) {
        const sf::Vector2f& point = this->shape.getPoint(i);
        this->shapePoints.push_back(point);
        this->shapeCenter += point / (float) pointCount;
    }

    this->shiftFromCenterMatrix[0][2] = -this->shapeCenter.x;
    this->shiftFromCenterMatrix[1][2] = -this->shapeCenter.y;

    this->shiftToCenterMatrix[0][2] = this->shapeCenter.x;
    this->shiftToCenterMatrix[1][2] = this->shapeCenter.y;
}

void Model::update(const sf::Vector2f& shiftDelta, const float& rotationAngleDelta, const float& scaleFactorDelta) {
    this->shift += shiftDelta;
    this->rotationAngle += rotationAngleDelta;
    this->scaleFactor += scaleFactorDelta;

#ifdef MANUAL_MATRIX_MULTIPLICATION
    // TODO: think about it...
    float s = this->scaleFactor;
    float x = this->shift.x;
    float y = this->shift.y;
    float cx = this->shapeCenter.x;
    float cy = this->shapeCenter.y;
    float cosRotation = cos(this->rotationAngle);
    float sinRotation = sin(this->rotationAngle);

    this->modelMatrix[0][0] = s * cosRotation;
    this->modelMatrix[0][1] = -s * sinRotation;
    this->modelMatrix[0][2] = x * s * cosRotation - y * s * sinRotation + s * cx * cosRotation - s * cy * sinRotation - cx;

    this->modelMatrix[1][0] = s * sinRotation;
    this->modelMatrix[1][1] = s * cosRotation;
    this->modelMatrix[1][2] = x * s * sinRotation + y * s * cosRotation + s * cx * sinRotation - s * cy * cosRotation - cy;

    this->modelMatrix[2][2] = 1;

    printMatrix(modelMatrix);
#else
    updateShiftMatrix();
    updateRotationMatrix();
    updateScaleMatrix();

    this->modelMatrix = this->shiftFromCenterMatrix * this->rotationMatrix * this->scaleMatrix * this->shiftToCenterMatrix * this->shiftMatrix;
#endif


    for (uint64_t i = 0; i < shapePoints.size(); ++i) {
        sf::Vector2f realCoordinates = multiplyHomogenous(shapePoints[i], this->modelMatrix);
        this->shape.setPoint(i, realCoordinates);
    }

    if (this->changingColor) {
        this->shapeColor.r = this->initialShapeColor.r + (int) ((this->shift.x + this->shift.y) / 10.) % 255;
        this->shapeColor.g = this->initialShapeColor.g + (int) (this->scaleFactor * 50.) % 255;
        this->shapeColor.b = this->initialShapeColor.b + (int) (this->rotationAngle * 20.) % 255;
    } else {
        this->shapeColor = this->initialShapeColor;
    }

    this->shape.setFillColor(this->shapeColor);
}

void Model::updateShiftMatrix() noexcept {
    this->shiftMatrix[0][2] = shift.x;
    this->shiftMatrix[1][2] = shift.y;
}

void Model::updateRotationMatrix() noexcept {
    float cosine = cos(this->rotationAngle);
    float sine = sin(this->rotationAngle);

    this->rotationMatrix[0][0] = cosine;
    this->rotationMatrix[0][1] = -sine;
    this->rotationMatrix[1][0] = sine;
    this->rotationMatrix[1][1] = cosine;
}

void Model::updateScaleMatrix() noexcept {
    this->scaleMatrix[0][0] = this->scaleFactor;
    this->scaleMatrix[1][1] = this->scaleFactor;
}    