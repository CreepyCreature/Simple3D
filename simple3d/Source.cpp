#include "simple3d.h"


/*
 *	Compile this for x86 (i.e. 32bit) unless you link the x86_64 
 *	version of the used libraries.
 *
 *	Limitation: If a textured model that has no Specular Maps
 *	is drawn with the default Lighting Shader the resulting shading is
 *	undefined (due to sampling with an empty Specular sampler).
 *
**/


int main() 
{
	int width = 1280;
	int height = 720;
	Window window(width, height);
	window.SetCursorMode(Window::CURSOR_MODE::CURSOR_DISABLED);
	Renderer renderer;
	renderer.Init();

	ShaderProgram program;
	program.LoadFromFile(lighting_vs_path, ShaderProgram::ShaderStage::VERTEX_SHADER);
	program.LoadFromFile(lighting_fs_path, ShaderProgram::ShaderStage::FRAGMENT_SHADER);
	program.Link();

	ShaderProgram basicprogram;
	basicprogram.LoadFromFile(basic_textured_vs_path, ShaderProgram::ShaderStage::VERTEX_SHADER);
	basicprogram.LoadFromFile(basic_textured_fs_path, ShaderProgram::ShaderStage::FRAGMENT_SHADER);
	basicprogram.Link();

	ShaderProgram lightsource;
	lightsource.LoadFromFile(basic_vs_path, ShaderProgram::ShaderStage::VERTEX_SHADER);
	lightsource.LoadFromFile(basic_fs_path, ShaderProgram::ShaderStage::FRAGMENT_SHADER);
	lightsource.Link();
	
	std::vector<glm::vec3> vcube = {
		glm::vec3(-0.5f, -0.5f,	 -0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	-0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	-0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	-0.5f)	  ,
		glm::vec3(-0.5f,  0.5f,	 -0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	 -0.5f)	  ,

		glm::vec3(-0.5f, -0.5f,	  0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	 0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	 0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	 0.5f)	  ,
		glm::vec3(-0.5f,  0.5f,	  0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	  0.5f)	  ,

		glm::vec3(-0.5f,  0.5f,	  0.5f)	  ,
		glm::vec3(-0.5f,  0.5f,	 -0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	 -0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	 -0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	  0.5f)	  ,
		glm::vec3(-0.5f,  0.5f,	  0.5f)	  ,

		glm::vec3(0.5f,  0.5f,	 0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	-0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	-0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	-0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	 0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	 0.5f)	  ,

		glm::vec3(-0.5f, -0.5f,	 -0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	-0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	 0.5f)	  ,
		glm::vec3(0.5f, -0.5f,	 0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	  0.5f)	  ,
		glm::vec3(-0.5f, -0.5f,	 -0.5f)	  ,

		glm::vec3(-0.5f,  0.5f,	 -0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	-0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	 0.5f)	  ,
		glm::vec3(0.5f,  0.5f,	 0.5f)	  ,
		glm::vec3(-0.5f,  0.5f,	  0.5f)	  ,
		glm::vec3(-0.5f,  0.5f,	 -0.5f)	  ,
	};
	std::vector<glm::vec3> vpos = {
		glm::vec3(-0.5f,  0.5f, -0.5f)	,
		glm::vec3(-0.5f, -0.5f, -0.5f)	,
		glm::vec3( 0.5f, -0.5f, -0.5f)	,
		glm::vec3( 0.5f,  0.5f, -0.5f)
	};
	std::vector<glm::vec3> vcols = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
	};
	std::vector<GLuint> vindices = {
		0, 1, 2,
		2, 3, 0
	};

	//Mesh mesh(vpos, vcols, vindices);
	Mesh mesh(vcube, vcube);
	std::vector<Mesh> meshes;
	meshes.push_back(mesh);
	Model point(meshes);
	point.Scale(glm::vec3(0.2));

	Model model("../_models/sponza_obj/sponza.obj");
	model.Scale(glm::vec3(0.1));

	Camera camera(&window);
	JoystickManager* joy = JoystickManager::Instance();
	//std::vector<char> joybuttons = (*joy).GetJoystickButtons(JoystickManager::JOYSTICK_1);

	DirectionalLight dirlight;
	DirectionalLight otherdirlight;
	PointLight plight, olight, alight;
	SpotLight spot, flashlight;
	
	dirlight.SetDirection(0.3f, -0.9f, -0.1f);

	plight.SetAllColor(0.6f, 0.0f, 0.0f);
	olight.SetAllColor(0.0f, 0.6f, 0.0f);
	alight.SetAllColor(0.0f, 0.0f, 0.6f);

	otherdirlight.SetDirection(-2.2f, -1.0f, 0.3f);
	otherdirlight.SetAmbient(0.125f, 0.1f, 0.075f);
	otherdirlight.SetDiffuse(0.5f, 0.4f, 0.3f);
	otherdirlight.SetSpecular(0.7f, 0.7f, 0.7f);

	dirlight.	SetActive(false);
	
	LightingManager lighting;
	lighting.AddLight(&dirlight);
	lighting.AddLight(&otherdirlight);
	lighting.AddLight(&plight);
	lighting.AddLight(&olight);
	lighting.AddLight(&alight);
	lighting.AddLight(&spot);
	lighting.AddLight(&flashlight);

	GLfloat t = Time();	
	while (!window.ShouldClose() && !window.KeyPressed(KEY_ESCAPE))
	{		
		camera.Update();
		camera.UpdateShaders(program);
		camera.UpdateShaders(basicprogram);
		camera.UpdateShaders(lightsource);

		dirlight.SetDirection(
			(glm::sin(glm::radians(t) * 10) + 1) / 2,
			(glm::cos(glm::radians(t) * 10) + 1) / 2,
			0.0f
		);
		if (window.KeyDown(KEY_K))
		{
			otherdirlight.SetActive(!otherdirlight.Active());
		}
		olight.SetPosition(
			(glm::cos(glm::radians(t) * 100)) * 10,
			5.0f,
			(glm::sin(glm::radians(t) * 100)) * 10
		);
		plight.SetPosition(
			(glm::cos(glm::radians(t) * 100 + 10)) * 10,
			5.0f,
			(glm::sin(glm::radians(t) * 100 + 10)) * 10
		);
		alight.SetPosition(
			(glm::cos(glm::radians(t) * 100 - 10)) * 10,
			5.0f,
			(glm::sin(glm::radians(t) * 100 - 10)) * 10
		);

		spot.SetDirection(
			(glm::cos(glm::radians(t) * 35)) * 100,
			0.0f,
			(glm::sin(glm::radians(t) * 35)) * 100
		);
		flashlight.SetPosition(camera.Position());
		flashlight.SetDirection(camera.Direction());

		lighting.UpdateShaders(program);

		point.Translate(plight.Position());
		point.Draw(lightsource);
		point.Translate(olight.Position());
		point.Draw(lightsource);
		point.Translate(alight.Position());
		point.Draw(lightsource);

		point.Translate(spot.Position());
		point.Draw(lightsource);

		if (window.KeyDown(KEY_F))
		{
			flashlight.SetActive(!flashlight.Active());
		}

		if (window.KeyDown(KEY_MINUS))
		{
			model.ScaleBy(glm::vec3(0.9f));
		}
		else if (window.KeyDown(KEY_EQUAL))
		{
			model.ScaleBy(glm::vec3(1.1f));
		}

		if (window.KeyPressed(KEY_B))
		{
			model.Draw(basicprogram);
		} else 	model.Draw(program);

		// When the window mode changes, the cursor position spikes if the cursor is disabled
		if (window.KeyPressed(KEY_RIGHT_ALT) &&
			window.KeyDown(KEY_ENTER))
		{
			if (window.WindowMode() != Window::WINDOW_MODE::FULLSCREEN)
				window.MakeFullscreen();
			else if (window.WindowMode() != Window::WINDOW_MODE::WINDOWED)
				window.MakeWindowed();
		}

		t = Time();
		window.Update();
	}

	return 0;
}