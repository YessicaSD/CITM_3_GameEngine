#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRandom.h"


GameObject::GameObject(std::string name, ComponentTransform * parent, UID uid):
	name(name)
{
	transform = new ComponentTransform(this);
	transform->SetParent(parent);
	components.push_back(transform);
	if (uid == INVALID_GAMEOBJECT_UID)
	{
		this->uid = App->random->RandomUID();
	}
	else
	{
		this->uid = uid;
	}
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		Component* aux = (*iter);
		if (aux)
		{
			delete aux;
		}
	}
	components.clear();
}

const char * GameObject::GetName()
{
	return name.c_str();
}

bool GameObject::OnStart()
{

	return true;
}

bool GameObject::OnUpdate(float dt)
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		if ((*iter)->enabled)
		{
			(*iter)->OnUpdate(dt);
		}
	}
	return true;
}

bool GameObject::OnPostUpdate()
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		if ((*iter)->enabled)
		{
			(*iter)->OnPostUpdate();
		}
	}
	return true;
}

bool GameObject::ActivateComponents()
{
	if (!active) 
	{
		for (std::vector<Component*>::iterator iter = components.begin();
			iter != components.end();
			++iter)
		{
			(*iter)->OnEnable();
		}
		active = true;
	}
	return true;
}

bool GameObject::DeactivateComponents()
{
	if (active) 
	{
		for (std::vector<Component*>::iterator iter = components.begin();
			iter != components.end();
			++iter)
		{
			(*iter)->OnDisable();
		}
		active = false;
	}
	return true;
}

void GameObject::SetActive(bool value)
{
	active = value;
}

void GameObject::OnSave(JSONFile * scene)
{
	if (transform->parent != nullptr)
	{
		JSONFile  this_gameobject;
		this_gameobject.CreateJSONFile();
		this_gameobject.SaveText("name", name.c_str());
		this_gameobject.SaveUID("UID", uid);
		this_gameobject.SaveUID("Parent UID", transform->parent->gameobject->GetUID());
		JSONFile* components_section = &this_gameobject.AddSection("Components");
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			(*iter)->OnSave(components_section);
		}

		scene->AddArrayValue(this_gameobject.GetValue());
	}
	for (std::vector<ComponentTransform*>::iterator iter = transform->children.begin(); iter != transform->children.end(); ++iter)
	{
		(*iter)->gameobject->OnSave(scene);
	}
	
	
}

void GameObject::OnLoad(JSONFile * file)
{
	JSONFile component_file;
	component_file = file->GetSection("Transform");
	if (component_file.IsValid())
	{
		transform->OnLoad(&component_file);
	}
}

UID GameObject::GetUID()
{
	return uid;
}


