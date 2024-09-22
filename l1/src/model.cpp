#include "model.hpp"

#include "matriciesop.hpp"


Model::Model(sf::ConvexShape& shape) : shape(shape), initialShapeColor(shape.getFillColor()) {
    const int pointCount = this->shape.getPointCount();
    for (int i = 0; i < pointCount; ++i) {
        const sf::Vector2f& point = this->shape.getPoint(i);
        this->shapePoints.push_back(point);
        this->shapeCenter += point / (float) pointCount;
    }

    this->shiftToCenterMatrix[0][2] = this->shapeCenter.x;
    this->shiftToCenterMatrix[1][2] = this->shapeCenter.y;

    this->shiftFromCenterMatrix[0][2] = -this->shapeCenter.x;
    this->shiftFromCenterMatrix[1][2] = -this->shapeCenter.y;
}

void Model::update(const sf::Vector2f& shiftDelta, const float& rotationAngleDelta, const float& scaleFactorDelta) {
    this->shift += shiftDelta;
    updateShiftMatrix();
    this->rotationAngle += rotationAngleDelta;
    updateRotationMatrix();
    this->scaleFactor += scaleFactorDelta;
    updateScaleMatrix();

    modelMatrix = shiftFromCenterMatrix * rotationMatrix * scaleMatrix * shiftToCenterMatrix * shiftMatrix;

    for (uint64_t i = 0; i < shapePoints.size(); ++i) {
        sf::Vector2f realCoordinates = multiplyHomogenous(shapePoints[i], modelMatrix);
        this->shape.setPoint(i, realCoordinates);
    }

    this->shapeColor.r = this->initialShapeColor.r + (int) ((this->shift.x + this->shift.y) / 10.) % 255;
    this->shapeColor.g = this->initialShapeColor.g + (int) (this->scaleFactor * 50.) % 255;
    this->shapeColor.b = this->initialShapeColor.b + (int) (this->rotationAngle * 20.) % 255;

    this->shape.setFillColor(this->shapeColor);
}

void Model::updateShiftMatrix() {
    this->shiftMatrix[0][2] = shift.x;
    this->shiftMatrix[1][2] = shift.y;
}

void Model::updateRotationMatrix() {
    float cosine = cos(this->rotationAngle);
    float sinus = sin(this->rotationAngle);

    this->rotationMatrix[0][0] = cosine;
    this->rotationMatrix[0][1] = -sinus;
    this->rotationMatrix[1][0] = sinus;
    this->rotationMatrix[1][1] = cosine;
}

void Model::updateScaleMatrix() {
    this->scaleMatrix[0][0] = this->scaleFactor;
    this->scaleMatrix[1][1] = this->scaleFactor;
}    