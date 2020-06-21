#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRandom.h"
#include "ModuleScene.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"

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
		(*iter) = nullptr;
		if (aux != nullptr)
		{
			aux->CleanUp();
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

void GameObject::OnSave(JSONFile * json_scene)
{
	JSONFile  json_gameobject;
	json_gameobject.CreateJSONFile();
	json_gameobject.SaveText("name", name.c_str());
	json_gameobject.SaveUID("UID", uid);
	if (transform->parent != App->scene->root_gameobject->transform)
	{
		json_gameobject.SaveUID("Parent UID", transform->parent->gameobject->GetUID());
	}
	else
	{
		json_gameobject.SaveUID("Parent UID", INVALID_GAMEOBJECT_UID);
	}
	JSONFile* components_section = &json_gameobject.AddSection("Components");
	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
	{
		(*iter)->OnSave(components_section);
	}
	json_scene->AddArrayValue(json_gameobject.GetValue());

	for (std::vector<ComponentTransform*>::iterator iter = transform->children.begin(); iter != transform->children.end(); ++iter)
	{
		(*iter)->gameobject->OnSave(json_scene);
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
	component_file = file->GetSection("Camera");
	if (component_file.IsValid())
	{
		ComponentCamera* component_camera = CreateComponent<ComponentCamera>();
		component_camera->OnLoad(&component_file);
	}
	component_file = file->GetSection("Mesh");
	if (component_file.IsValid())
	{
		ComponentMesh* component_mesh = CreateComponent<ComponentMesh>();
		component_mesh->OnLoad(&component_file);
	}
	component_file = file->GetSection("Material");
	if (component_file.IsValid())
	{
		//INFO: Component mesh creates a component material, that's why we use GetComponent instead of CreateComponent
		ComponentMaterial * component_material = GetComponent<ComponentMaterial>();
		component_material->OnLoad(&component_file);
	}
}

UID GameObject::GetUID()
{
	return uid;
}


