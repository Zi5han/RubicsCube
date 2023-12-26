#pragma once
#include "AbstractGameInterface.h"

#include <glm/ext/quaternion_float.hpp>

class TestGlm : public AbstractGameInterface {
public:
	void Initialize() override;
	void Update(double deltaTime) override;

private:
	glm::quat m_oreintaionQuaternion;
};