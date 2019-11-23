#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <xstring>


typedef unsigned int uint;
#define TO_STRING( x ) #x

//****************
// CLASS_DECLARATION
//
// This macro must be included in the declaration of any subclass of Component.
// It declares variables used in type checking.
//****************
#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const uint type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define CLASS_DEFINITION( parentclass, childclass )                                         \
const uint childclass::type = std::hash< std::string >()( TO_STRING( childclass ) ); \
bool childclass::IsClassType( const uint classType ) const {                         \
        if ( classType == childclass::type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                                                                           

class GameObject;

class Component
{
public:
	static const uint type;

public:
	Component(GameObject * gameobject);
	

	virtual void PropertiesEditor() {};
	void EnableComponent(bool value);

	virtual bool IsClassType(const uint classType) const
	{
		return classType == type;
	}

	bool IsActive();
protected:
	bool enabled = true;
	std::string name;

protected:
	virtual void OnUpdate(float dt) {};
	virtual void OnPostUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void CleanUp() {};
	bool CollapsigHeaderWithCheckbox();

public:
	//this funtion should only be call by transform component
	virtual void TransformHaveChanged() {};

public:
	//The gameobject which contains this component
	GameObject* gameobject = nullptr;


	
	friend class GameObject;
};


#endif // !COMPONENT_H_

