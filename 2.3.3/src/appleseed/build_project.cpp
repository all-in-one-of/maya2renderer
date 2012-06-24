#include "prerequest_as.h"
#include "../common//prerequest_std.h"
#include <trace/trace.hpp>

static const std::string root_path = "E:/dev/Autodesk/maya/myplugin/project/liquid_/dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/sample";

void build_project(
	asf::auto_release_ptr<asr::Project> &project,
	asf::auto_release_ptr<asr::Assembly> &assembly
)
{
	CM_TRACE_FUNC("build_project()");


	// Create a color called "gray" and insert it into the assembly.
	static const float GrayReflectance[] = { 0.5f, 0.5f, 0.5f };
	assembly->colors().insert(
		asr::ColorEntityFactory::create(
		"gray",
		asr::ParamArray()
		.insert("color_space", "srgb"),
		asr::ColorValueArray(3, GrayReflectance)));

	// Create a BRDF called "diffuse_gray_brdf" and insert it into the assembly.
	assembly->bsdfs().insert(
		asr::LambertianBRDFFactory().create(
		"diffuse_gray_brdf",
		asr::ParamArray()
		.insert("reflectance", "gray")));

	// Create a physical surface shader and insert it into the assembly.
	assembly->surface_shaders().insert(
		asr::PhysicalSurfaceShaderFactory().create(
		"physical_surface_shader",
		asr::ParamArray()));

	// Create a material called "gray_material" and insert it into the assembly.
	assembly->materials().insert(
		asr::MaterialFactory::create(
		"gray_material",
		asr::ParamArray()
		.insert("surface_shader", "physical_surface_shader")
		.insert("bsdf", "diffuse_gray_brdf")));

// 	// Load the scene geometry from disk.
// 	asf::SearchPaths search_paths;
// 	search_paths.push_back(root_path + "/data");
// 	asr::MeshObjectArray objects =
// 		asr::MeshObjectReader::read(
// 		search_paths,
// 		"cube",
// 		asr::ParamArray()
// 		.insert("filename", "scene.obj"));
// 
// 	// Insert all the objects into the assembly.
// 	for (size_t i = 0; i < objects.size(); ++i)
// 	{
// 		// Insert this object into the scene.
// 		asr::MeshObject* object = objects[i];
// 		assembly->objects().insert(asf::auto_release_ptr<asr::Object>(object));
// 
// 		// Create the array of material names.
// 		asf::StringArray material_names;
// 		material_names.push_back("gray_material");
// 
// 		// Create an instance of this object and insert it into the assembly.
// 		const std::string instance_name = std::string(object->get_name()) + "_inst";
// 		assembly->object_instances().insert(
// 			asr::ObjectInstanceFactory::create(
// 			instance_name.c_str(),
// 			asr::ParamArray(),
// 			*object,
// 			asf::Transformd(asf::Matrix4d::identity()),
// 			material_names));
// 	}

	// Create a color called "light_exitance" and insert it into the assembly.
	static const float LightExitance[] = { 1.0f, 1.0f, 1.0f };
	assembly->colors().insert(
		asr::ColorEntityFactory::create(
		"light_exitance",
		asr::ParamArray()
		.insert("color_space", "srgb")
		.insert("multiplier", "30.0"),
		asr::ColorValueArray(3, LightExitance)));

	// Create a point light called "light" and insert it into the assembly.
	asf::auto_release_ptr<asr::Light> light(
		asr::PointLightFactory().create(
		"light",
		asr::ParamArray()
		.insert("exitance", "light_exitance")));
	light->set_transform(asf::Transformd(
		asf::Matrix4d::translation(asf::Vector3d(0.6, 2.0, 1.0))));
	assembly->lights().insert(light);

// 	// Create an instance of the assembly and insert it into the scene.
// 	project->get_scene()->assembly_instances().insert(
// 		asr::AssemblyInstanceFactory::create(
// 		"assembly_inst",
// 		asr::ParamArray(),
// 		*assembly,
// 		asf::Transformd(asf::Matrix4d::identity())));

// 	// Insert the assembly into the scene.
// 	project->get_scene()->assemblies().insert(assembly);

	// Create a pinhole camera with film dimensions 0.980 x 0.735 in (24.892 x 18.669 mm).
// 	asf::auto_release_ptr<asr::Camera> _camera(
// 		asr::PinholeCameraFactory().create(
// 		"camera",
// 		asr::ParamArray()
// 		.insert("film_dimensions", boost::format("%f %f") %0.024892 %0.018669)//""
// 		.insert("focal_length", "0.035")));
// 	// Bind the camera to the scene.
// 	project->get_scene()->set_camera(_camera);
// 
// 	// Place and orient the camera. By default cameras are located in (0.0, 0.0, 0.0)
// 	// and are looking toward Z- (0.0, 0.0, -1.0).
// 	project->get_scene()->get_camera()->transform_sequence().set_transform(
// 		0.0,
// 		asf::Transformd(
// 		asf::Matrix4d::rotation(asf::Vector3d(1.0, 0.0, 0.0), asf::deg_to_rad(-20.0)) *
// 		asf::Matrix4d::translation(asf::Vector3d(0.0, 0.8, 11.0))));



// 	// Create a frame and bind it to the project.
// 	project->set_frame(
// 		asr::FrameFactory::create(
// 		"beauty",
// 		asr::ParamArray()
// 		.insert("camera", project->get_scene()->get_camera()->get_name())
// 		.insert("resolution", "640 480")
// 		.insert("color_space", "srgb")));

	// Create an environment and bind it to the scene.
	project->get_scene()->set_environment(
		asr::EnvironmentFactory::create(
		"environment",
		asr::ParamArray()));

// 	// Bind the scene to the project.
// 	project->set_scene(scene);

}