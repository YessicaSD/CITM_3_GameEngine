#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"

#include "ComponentCamera.h"
#include "glew\include\GL\glew.h"
#include "PanelScene.h"
#include "PanelGame.h"
#include <gl\GL.h>

#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleImportModel.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

#include "RaycastHit.h"
#include "ComponentMaterial.h"
#include "ResourceModel.h"
#include "ComponentAnimator.h"

#include "imGuizmo/ImGuizmo.h"
#include "Event.h"
#include <map>
#include "parson\parson.h"

ModuleScene::ModuleScene(const char *name, bool start_enabled) : Module(start_enabled, name)
{
	current_scene_name = "new_scene";
}

ModuleScene::~ModuleScene()
{
}

// Load assets
bool ModuleScene::Start(JSONFile *config)
{
	bool ret = true;

	root_gameobject = new GameObject("Root", nullptr);
	CreateOctree();
	
	LoadStreetScene();

	GameObject* object_camera = new GameObject("Main camera", root_gameobject->transform);
	game_camera = object_camera->CreateComponent<ComponentCamera>();

	object_camera = new GameObject("Camera", root_gameobject->transform);
	object_camera->CreateComponent<ComponentCamera>();

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	DeleteGameObject(root_gameobject);
	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	if (!App->gui->panel_scene->is_being_used)
	{
		if (character != nullptr)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN)
			{
				attack->loop = false;
				character_animator->ChangeCurrentNode(attack);
			}
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN)
			{
				run->loop = true;
				character_animator->ChangeCurrentNode(run);
			}
		}
		
		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::TRANSLATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_E))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::ROTATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::SCALE;
		}
	}

	//TODO: Turn into a shortcut
	if (App->input->GetKey(SDL_SCANCODE_DELETE))
	{
		ComponentTransform *selected_object = App->gui->GetSelectedTransform();
		if (selected_object != nullptr)
		{
			Event new_event(Event::DELETE_OBJECT);
			new_event.object.ptr = selected_object->gameobject;
			App->AddEvent(new_event);
			App->AddEvent(Event(Event::UPDATE_OCTREE));
			App->gui->SetSelectedGameObjec(nullptr);
		}
	}

	GameObjectUpdateRecursive(dt, root_gameobject->transform);

	return UPDATE_CONTINUE;
}

void ModuleScene::GameObjectUpdateRecursive(float dt, ComponentTransform *transform)
{
	transform->gameobject->OnUpdate(dt);
	for (std::vector<ComponentTransform *>::iterator iter = transform->children.begin();
		 iter != transform->children.end();
		 ++iter)
	{
		GameObjectUpdateRecursive(dt, (*iter));
	}
}

void ModuleScene::GameObjectPostUpdateRecursive(ComponentTransform *transform)
{
	transform->gameobject->OnPostUpdate();
	for (std::vector<ComponentTransform *>::iterator iter = transform->children.begin();
		 iter != transform->children.end();
		 ++iter)
	{
		GameObjectPostUpdateRecursive((*iter));
	}
}
bool Compare(RaycastHit &a, RaycastHit &b)
{
	return a.distance < b.distance;
}

bool ModuleScene::IntersectRay(LineSegment *ray, RaycastHit &hit)
{
	this->ray = (*ray);

	std::map<float, ComponentTransform *> out_objects;
	octree.CollectIntersections(out_objects, (*ray));
	GetIntersectBoxNonStatics(root_gameobject->transform, ray, out_objects);

	return TestWithTriangles(ray, out_objects, hit);
}

void ModuleScene::RecreateOctree()
{
	CreateOctree();
	LoadStaticObjects();
}

void ModuleScene::CreateOctree()
{
	const float3 center(0.0f, 0.0f, 0.0f);
	const float3 size(OCTREE_X_SIZE, OCTREE_Y_SIZE, OCTREE_Z_SIZE);
	AABB boundary;
	boundary.SetFromCenterAndSize(center, size);
	octree.SetLimits(boundary);
}

void ModuleScene::GetStaticObjects(std::vector<ComponentTransform *> &static_objects)
{
	root_gameobject->transform->GetStaticObjects(static_objects);
}

void ModuleScene::SetMainCamera(ComponentCamera *main_camera)
{
	if (main_camera != nullptr)
	{
		game_camera = main_camera;
		App->renderer3D->game_fbo.camera = game_camera;
	}
}

void ModuleScene::DeleteGameObject(GameObject *gameobject)
{
	if (gameobject)
	{
		gameobject->transform->DeleteChildren();
		if (gameobject->transform->parent)
		{
			for (std::vector<ComponentTransform *>::iterator iter = gameobject->transform->parent->children.begin(); iter != gameobject->transform->parent->children.end(); ++iter)
			{
				if ((*iter) == gameobject->transform)
				{
					gameobject->transform->parent->children.erase(iter);
					break;
				}
			}
		}
		if (gameobject == character)
		{
			character = nullptr;
		}
		delete gameobject;
	}
}

void ModuleScene::GetIntersectBoxNonStatics(ComponentTransform *object, LineSegment *ray, std::map<float, ComponentTransform *> &out_objects)
{
	if (object->enabled && !object->is_static)
	{
		if (object->Intersect(*ray))
		{

			float near_hit, far_hit;
			if (ray->Intersects(object->bounding_box.GetOBB(), near_hit, far_hit))
			{

				out_objects[near_hit] = object;
			}
		}
		for (auto iter = object->children.begin(); iter != object->children.end(); ++iter)
		{
			GetIntersectBoxNonStatics((*iter), ray, out_objects);
		}
	}
}
bool ModuleScene::TestWithTriangles(LineSegment *ray, std::map<float, ComponentTransform *> &out_objects, RaycastHit &hit_out)
{

	for (std::map<float, ComponentTransform *>::iterator iter = out_objects.begin(); iter != out_objects.end(); ++iter)
	{
		ComponentMesh *mesh = (*iter).second->gameobject->GetComponent<ComponentMesh>();
		if (mesh)
		{
			RaycastHit hit;
			if (mesh->Intersect(ray, hit))
			{
				hit_out = hit;
				return true;
			}
		}
	}
	return false;
}

void ModuleScene::LoadStaticObjects()
{
	std::vector<ComponentTransform *> static_objects;
	root_gameobject->transform->GetStaticObjects(static_objects);
	for (std::vector<ComponentTransform *>::iterator iter = static_objects.begin(); iter < static_objects.end(); ++iter)
	{
		octree.Insert((*iter));
	}
}

void ModuleScene::DrawObjects(ComponentCamera *camera)
{
	if (camera->frustum_culling)
	{
		DrawWithFrustrum(camera);
	}
	else
	{
		GameObjectPostUpdateRecursive(root_gameobject->transform);
	}
}

void ModuleScene::DrawWithFrustrum(ComponentCamera *camera)
{
	std::vector<ComponentTransform *> objects;
	octree.CollectIntersections(objects, camera->GetFrustrum());
	root_gameobject->transform->GetIntersectNonStatics(objects, camera->GetFrustrum());
	for (std::vector<ComponentTransform *>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->gameobject->OnPostUpdate();
	}
}

void ModuleScene::SaveScene()
{
	current_scene.CreateJSONFileArray();
	for (auto iter = root_gameobject->transform->children.begin();
		 iter != root_gameobject->transform->children.end();
		 ++iter)
	{
		(*iter)->gameobject->OnSave(&current_scene);
	}
	current_scene.SaveFile(std::string(ASSETS_FOLDER) + current_scene_name.c_str() + "." + SCENE_EXTENSION);
	current_scene.CloseFile();
	LOG("Saved scene");
}

void ModuleScene::LoadScene(const char *scene_path)
{
	std::string scene_name_;
	std::string extension_;
	App->file_system->SplitFilePath(scene_path, nullptr, &scene_name_, &extension_);
	size_t pos_extension = scene_name_.find(extension_);

	current_scene_name = scene_name_.substr(0, pos_extension - 1);
	current_scene.LoadFile(scene_path);
	if (json_value_get_type(current_scene.GetValue()) != JSONArray)
	{
		LOG("Could not load this scene");
		return;
	}
	current_scene.LoadArray();
	std::map<UID, GameObject *> new_gameobjects;
	std::map<UID, UID> parent_index;
	int number_of_objects = current_scene.GetNumberOfElement();
	root_gameobject->transform->DeleteChildren();

	//INFO: Load all gameobjects into a map
	for (int i = 0; i < number_of_objects; ++i)
	{
		JSONFile current_object(current_scene.GetObjectArray(i));
		GameObject *gameobject_ptr = new GameObject(std::string(current_object.LoadText("name")), nullptr, current_object.LoadUID("UID"));
		gameobject_ptr->OnLoad(&current_object.GetSection("Components"));
		new_gameobjects[gameobject_ptr->uid] = gameobject_ptr;
		parent_index[gameobject_ptr->uid] = current_object.LoadUID("Parent UID");
	}

	//INFO: Parent them
	for (auto iter = new_gameobjects.begin(); iter != new_gameobjects.end(); ++iter)
	{
		GameObject *current_gameobject = (*iter).second;
		UID parent_uid = parent_index[(*iter).first];
		if (parent_uid != INVALID_GAMEOBJECT_UID)
		{
			GameObject *parent_gameobject = new_gameobjects[parent_uid];
			current_gameobject->transform->SetParent(parent_gameobject->transform);
		}
		else
		{
			current_gameobject->transform->SetParent(root_gameobject->transform);
		}
	}
}

update_status ModuleScene::PostUpdate()
{
	App->renderer3D->scene_fbo.StartRender(App->gui->panel_scene->current_viewport_size);

	glLineWidth(5);
	glColor4f(0, 0, 255, 1);
	octree.Draw();

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	//p.wire = false;
	p.Render();
	if (App->camera->see_frustrum_culling)
	{
		DrawObjects(game_camera);
	}
	else
	{
		DrawObjects(App->camera->scene_camera);
	}

	glLineWidth(10);
	glColor4f(255, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(ray.a.x, ray.a.y, ray.a.z);
	glVertex3f(ray.b.x, ray.b.y, ray.b.z);
	glEnd();
	App->renderer3D->scene_fbo.EndRender();

	App->renderer3D->game_fbo.StartRender(App->gui->panel_game->current_viewport_size);
	if (game_camera != nullptr)
	{
		DrawObjects(game_camera);
	}
	App->renderer3D->game_fbo.EndRender();
	return UPDATE_CONTINUE;
}

void ModuleScene::EventRequest(const Event &event)
{
	if (event.type == Event::UPDATE_OCTREE)
	{
		RecreateOctree();
	}
	else if (event.type == Event::DROPPED_MODEL_TO_SCENE)
	{
		App->import_model->CreateGameObjectFromModel(event.drop_model_data.model, App->scene->root_gameobject->transform);
	}
	else if (event.type == Event::SAVE_SCENE)
	{
		SaveScene();
	}
	else if (event.type == Event::LOAD_SCENE)
	{
		LoadScene(event.path);
	}
	else if (event.type == Event::DELETE_OBJECT)
	{
		DeleteGameObject(event.object.ptr);
	}
}

const char *ModuleScene::GetComponentType(const uint type)
{
	if (type == ComponentTransform::type)
	{
		return "Transform";
	}
	else if (type == ComponentMesh::type)
	{
		return "Mesh";
	}
	else if (type == ComponentMaterial::type)
	{
		return "Material";
	}
	else if (type == ComponentCamera::type)
	{
		return "Camera";
	}
}

uint ModuleScene::GetComponentType(const char *type)
{
	if (strcmp(type, "Transform") == 0)
	{
		return ComponentTransform::type;
	}
	else if (strcmp(type, "Mesh") == 0)
	{
		return ComponentMesh::type;
	}
	else if (strcmp(type, "Material") == 0)
	{
		return ComponentMaterial::type;
	}
	else if (strcmp(type, "Camera") == 0)
	{
		return ComponentCamera::type;
	}
}

void ModuleScene::LoadStreetScene()
{
	//Street scene
	const char * street_path = "Assets/street/Street environment_V01.FBX";
	JSONFile street_meta;
	street_meta.LoadFile(std::string(street_path) + "." + META_EXTENSION);
	UID street_uid = street_meta.LoadUID("resourceUID");
	ResourceModel * street = (ResourceModel*)App->resource_manager->GetResource(street_uid);
	if (street != nullptr)
	{
		GameObject* go_street = App->import_model->CreateGameObjectFromModel(street, root_gameobject->transform);
		go_street->transform->SetScale(float3(3.f, 3.f, 3.f));
	}
	else
	{
		LOG((std::string(street_path) + " not found, make sure it's on the assets folder.").c_str());
	}

	//Character + Idle Animation
	const char * character_idle_path = "Assets/Skeleton@Idle.FBX";
	JSONFile character_idle_meta;
	character_idle_meta.LoadFile(std::string(character_idle_path) + "." + META_EXTENSION);
	UID character_idle_uid = character_idle_meta.LoadUID("resourceUID");
	ResourceModel * character_idle = (ResourceModel*)App->resource_manager->GetResource(character_idle_uid);
	if (character_idle != nullptr)
	{
		character = App->import_model->CreateGameObjectFromModel(character_idle, root_gameobject->transform);
		character->transform->SetScale(float3(1.f, 1.f, 1.f));
		character_animator = character->GetComponent<ComponentAnimator>();
		idle = character_animator->GetCurrentNode();
	}
	else
	{
		LOG((std::string(character_idle_path) + " not found, make sure it's on the assets folder.").c_str());
	}

	//Attack
	const char * character_attack_path = "Assets/Skeleton@Attack.FBX";
	JSONFile character_attack_meta;
	character_attack_meta.LoadFile(std::string(character_attack_path) + "." + META_EXTENSION);
	UID character_attack_uid = character_attack_meta.LoadUID("resourceUID");
	ResourceModel * character_attack = (ResourceModel*)App->resource_manager->GetResource(character_attack_uid);
	if (character_attack != nullptr)
	{
		character_attack->StartUsingResource();
		for (auto iter = character_attack->animations_uid.begin(); iter != character_attack->animations_uid.end(); ++iter)
		{
			attack = character_animator->AddClip((ResourceAnimation*)App->resource_manager->GetResource((*iter)));
		}
	}
	else
	{
		LOG((std::string(character_attack_path) + " not found, make sure it's on the assets folder.").c_str());
	}

	//Walk
	const char * character_walk_path = "Assets/Skeleton@Run.FBX";
	JSONFile charater_walk_meta;
	charater_walk_meta.LoadFile(std::string(character_walk_path) + "." + META_EXTENSION);
	UID character_walk_uid = charater_walk_meta.LoadUID("resourceUID");
	ResourceModel * character_walk = (ResourceModel*)App->resource_manager->GetResource(character_walk_uid);
	if (character_walk != nullptr)
	{
		character_walk->StartUsingResource();
		for (auto iter = character_walk->animations_uid.begin(); iter != character_walk->animations_uid.end(); ++iter)
		{
			run = character->GetComponent<ComponentAnimator>()->AddClip((ResourceAnimation*)App->resource_manager->GetResource((*iter)));
		}
	}
	else
	{
		LOG((std::string(character_walk_path) + " not found, make sure it's on the assets folder.").c_str());
	}
}
