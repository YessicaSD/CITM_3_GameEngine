#ifndef COMPONENT_CAMERA_H_
#define COMPONENT_CAMERA_H_
#include "Component.h"
#include "MathGeoLib/include/Math/float4.h"
class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* gameobject);
	void OnPostUpdate() override;
	void PropertiesEditor() override;
private:
	float far_, near_;

	float4 background_color;
};
#endif // !COMPONENT_CAMARA_H_

