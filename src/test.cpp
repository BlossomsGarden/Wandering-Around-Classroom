#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>

//#define  STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>


#include "RenderCore/Renderer.h"
#include "RenderCore/VertexBuffer.h"
#include "RenderCore/IndexBuffer.h"
#include "RenderCore/VertexBufferLayout.h"
#include "RenderCore/VertexArray.h"
#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "Global.h"
#include "Scene/Camera.h"
#include "Scene/Controller.h"
#include "Scene/SkyBox.h"
#include "Scene/Model.h"
#include "GUI/Crosshair.h"
#include "Scene/SimpleObject/3D/Sphere.h"
#include "Scene/Light.h"
#include "Scene/SimpleObject/2D/Plane.h"
#include "Scene/ModelInstances.h"
#include "Scene/Particles.h"


void doc() {
	std::ifstream in("res/notes.txt");
	std::stringstream ss;
	std::string line;
	while (getline(in, line)) {
		ss << line;
		ss << '\n';
	}
	std::cout << ss.str();
}
void showCursor(bool show) {
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = show;
	cursor.dwSize = 1;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hOut, &cursor);
}
void RenderQuad();
void ShowFps(float deltaTime) {
	std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\bFPS:" << 1 / deltaTime;
}

int main(void)
{
	doc();
	showCursor(false);


	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window,0, 0);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
		std::cout << "ERROR" << std::endl;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	Renderer renderer;

	SkyBox skybox("res/Textures/Skybox_01");

	Plane Floor(glm::vec3(25.0f, 0.0f, 25.0f), 10.0f, 10.0f);
	Plane Wall(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 20.0f);
	Plane BlackBoard(glm::vec3(0.0f, 0.0f, 0.0f), 30.0f, 10.0f);
	Plane Door(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 15.0f);

	Texture FloorTexture("res/Textures/laminate_floor_02_diff_1k.jpg");
	Texture WallTexture("res/Textures/plastered_wall_diff_1k.jpg");
	Texture BlackBoardTexture("res/Textures/Blackboard.jpg");
	Texture	DoorTexture("res/Textures/Door.jpg");

	Model TDesk("res/Models/teacher-desk/teacher-desk.obj");
	Model SDesk("res/Models/student-desk/student-desk.obj");
	Model SChair("res/Models/student-chair/student-chair.obj");
	Model Curtain("res/Models/curtain/curtain.obj");
	//Model Flower("res/Models/flower/flower.obj");

	// shaders
	Shader CurtainShader("res/Shaders/Blinn-Phong/curtain_shader.sdc");
	CurtainShader.Bind();
	CurtainShader.SetFloat("shininess", 8.0f);


	Shader ModelShader("res/Shaders/Blinn-Phong/Material.sdc");
	ModelShader.Bind();
	ModelShader.SetFloat("shininess", 8.0f);
	


	Shader InstanceShader("res/Shaders/Blinn-Phong/Instancing.sdc");
	InstanceShader.Bind();
	InstanceShader.SetFloat("shininess", 8.0f);

	Shader LightShader("res/Shaders/Blinn-Phong/Light.sdc");
	Shader FloorShader("res/Shaders/Blinn-Phong/Plane.sdc");
	Shader WallShader("res/Shaders/Blinn-Phong/Plane.sdc");
	Shader SkyboxShader("res/Shaders/SkyBox.sdc");
	Shader ShadowmapShader("res/Shaders/ShadowMap.sdc");
	Shader InstancingShadowmapShader("res/Shaders/InstancingShadowMap.sdc");
	Shader CurtainShadowmapShader("res/Shaders/CurtainShadowMap.sdc");
	Shader CherryShader("res/Shaders/cherry_blossom.sdc");



	Shader DebugDepthShader("res/Shaders/DebugDepthQuad.sdc");

	ParticleSystem p(CherryShader, "res/Textures/flower.jpg");


	ModelShader.Bind();

	//Set up camera
	Camera cam(glm::vec3(50.0f, 10.0f, 25.0f));
	cam.SetPitchLock(true);
	Controller CamHandler(&cam, window);
	CamHandler.BindThis();
	CamHandler.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Set up lightings
	LightManager Lights1(7);
	Lights1.SetDirLight(glm::vec3(0.0f, -1.0f, 0.0f),		//Direction
		glm::vec3(0.1f, 0.1f, 0.1f),						//Ka
		glm::vec3(0.1f, 0.1f, 0.1f),						//Kd
		glm::vec3(0.1f, 0.1f, 0.1f));						//Ks

	PointLight LightTemplate(0,
		glm::vec3(0.0f, 0.0f, 0.0f), 			//Position
		glm::vec3(1.0f, 1.0f, 1.0f), 		//LightColor
		0.3f, 0.2f, 0.2f,  					//A,D,S ratio
		0.4f / 200,1.0f /200, 0.05f / 200);	//attenuation param
	PointLight Outside(0,
		glm::vec3(0.0f, 0.0f, 0.0f), 			//Position
		glm::vec3(255.0f/255, 200.0f / 255, 150.0f / 255), 		//LightColor
		0.5f, 0.5f, 0.3f,  					//A,D,S ratio
		0.2f / 200, 0.8f / 200, 0.0f / 200);	//attenuation param


	Lights1.SetPointLight(0, glm::vec3(0 * 10.0f, 20.0f, 1 * 10.0f), LightTemplate);
	Lights1.SetPointLight(1, glm::vec3(0 * 10.0f, 19.5f, 4 * 10.0f), LightTemplate);
	Lights1.SetPointLight(2, glm::vec3(3 * 10.0f, 20.0f, 1 * 10.0f), LightTemplate);
	Lights1.SetPointLight(3, glm::vec3(3 * 10.0f, 20.0f, 4 * 10.0f), LightTemplate);
	Lights1.SetPointLight(4, glm::vec3(6 * 10.0f, 20.0f, 1 * 10.0f), LightTemplate);
	Lights1.SetPointLight(5, glm::vec3(6 * 10.0f, 20.0f, 4 * 10.0f), LightTemplate);
	Lights1.SetPointLight(6, glm::vec3(55.0f, 20.0f, -60.0f), Outside);
	//Switch lights here
	//Lights1.DirLightStat(false);
	Lights1.PointLightStat(0,false);
	//Lights1.PointLightStat(1, false);
	Lights1.PointLightStat(2, false);
	Lights1.PointLightStat(3, false);
	Lights1.PointLightStat(4, false);
	Lights1.PointLightStat(5, false);
	//Lights1.PointLightStat(6, false);

	//Instancing
	ModelInstances SDeskInstances(6 * 6);
	ModelInstances SChairInstances(6 * 6);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			float y = 0.0f;
			float x = j * 10.0f;
			float z = i * 10.0f;
			model = glm::translate(model, glm::vec3(x, y, z));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			SDeskInstances.AddInstanceModel(model);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x - 10.0f, y, z));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			SChairInstances.AddInstanceModel(model);

		}
	}

	SDeskInstances.SetUpInstancing(SDesk);
	SChairInstances.SetUpInstancing(SChair);


	ShadowMap Shadow1(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 20.0f, 200.0f);



	DebugDepthShader.Bind();
	DebugDepthShader.SetInt("depthMap", 0);

	ModelShader.Bind();
	ModelShader.SetInt("shadowmap", 8);

	InstanceShader.Bind();
	InstanceShader.SetInt("shadowmap", 8);

	CurtainShader.Bind();
	CurtainShader.SetInt("shadowmap", 8);

	//Frame
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		CamHandler.ProcessInput(deltaTime);

		renderer.Clear(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


	
		//TODO ******ShadowMapping Stage****
		// Shadow S1
		//GLCALL(glCullFace(GL_FRONT));
		
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		
		lightProjection = glm::perspective(glm::radians(90.0f) , (GLfloat)SHADOWMAP_WIDTH / (GLfloat)SHADOWMAP_HEIGHT, Shadow1.m_Near, Shadow1.m_Far); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
		lightView = glm::lookAt(Lights1.m_Pointlights[6].m_Pos, glm::vec3(30.0f,0.0f,0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		ShadowmapShader.Bind();
		ShadowmapShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		InstancingShadowmapShader.Bind();
		InstancingShadowmapShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		CurtainShadowmapShader.Bind();
		CurtainShadowmapShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		


		glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER,Shadow1.GetFBO());
		glClear(GL_DEPTH_BUFFER_BIT);
		GLCALL(glCullFace(GL_FRONT));
		//******Draw stage*******
		{
			glm::mat4 model = glm::mat4(1.0f);
			//Draw Floor
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(-15.0f + i * 10.0f, 0.0f, -15.0f + j * 10.0f));
					model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::scale(model, glm::vec3(Floor.GetWidth() / 2, Floor.GetHeight() / 2, 1.0f));
					ShadowmapShader.Bind();
					ShadowmapShader.SetMat4("model", model);
					Floor.Draw(ShadowmapShader, FloorTexture);
				}
			}
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(-15.0f + i * 10.0f, 19.5f, -16.0f + j * 10.0f));
					model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::scale(model, glm::vec3(Floor.GetWidth() / 2, Floor.GetHeight() / 2, 1.0f));
					ShadowmapShader.Bind();
					ShadowmapShader.SetMat4("model", model);
					Floor.Draw(ShadowmapShader, FloorTexture);
				}
			}
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(-15.0f + i * 10.0f, 20.1f, -16.0f + j * 10.0f));
					model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::scale(model, glm::vec3(Floor.GetWidth() / 2, Floor.GetHeight() / 2, 1.0f));
					ShadowmapShader.Bind();
					ShadowmapShader.SetMat4("model", model);
				 	Floor.Draw(ShadowmapShader, FloorTexture);
				}
			}
			//Draw Wall
			model = glm::mat4(1.0f);
			//Left 
			for (int i = 0; i < 9; i++)
			{

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-15.0f + 10.0f * i, 10.0f, 70.0f));
				model = glm::scale(model, glm::vec3(Wall.GetWidth() / 2, Wall.GetHeight() / 2, 1.0f));
				ShadowmapShader.Bind();
				ShadowmapShader.SetMat4("model", model);
				Wall.Draw(ShadowmapShader, WallTexture);

			}
			//Front
			for (int i = 0; i < 9; i++)
			{

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(70.0f, 10.0f, -15.0f + 10.0f * i));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(Wall.GetWidth() / 2, Wall.GetHeight() / 2, 1.0f));
				ShadowmapShader.Bind();
				ShadowmapShader.SetMat4("model", model);
				Wall.Draw(ShadowmapShader, WallTexture);

			}
			//Back
			for (int i = 0; i < 9; i++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-20.0f, 10.0f, -15.0f + 10.0f * i));
				model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(Wall.GetWidth() / 2, Wall.GetHeight() / 2, 1.0f));
				ShadowmapShader.Bind();
				ShadowmapShader.SetMat4("model", model);
				Wall.Draw(ShadowmapShader, WallTexture);

			}

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(69.8f, 12.0f, 25.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(BlackBoard.GetWidth() / 2, BlackBoard.GetHeight() / 2, 1.0f));
			ShadowmapShader.Bind();
			ShadowmapShader.SetMat4("model", model);
			BlackBoard.Draw(ShadowmapShader, BlackBoardTexture);


			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(65.0f, 7.5f, 69.5f));

			model = glm::scale(model, glm::vec3(Door.GetWidth() / 2 - 1.0f, Door.GetHeight() / 2, 1.0f));
			ShadowmapShader.Bind();
			ShadowmapShader.SetMat4("model", model);
			Door.Draw(ShadowmapShader, DoorTexture);

		
			//Draw tdesk
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(55.0f, 0.0f, 25.0f));
			model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));	// it's a bit too big for our scene, so scale it down
			ShadowmapShader.Bind();
			ShadowmapShader.SetMat4("model", model);
			TDesk.Draw(ShadowmapShader);


			for (int row = 0; row < 6; row++) {
				for (int col = 0; col < 6; col++)
				{
					//Draw sdesk
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(col * 10.0f, 0.0f, row * 10.0f));
					model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
					ShadowmapShader.Bind();
					ShadowmapShader.SetMat4("model", model);
					SDesk.Draw(ShadowmapShader);
			
					//Draw schair
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(col * 10.0f - 10.0f, 0.0f, row * 10.0f));
					model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
					ShadowmapShader.Bind();
					ShadowmapShader.SetMat4("model", model);
					SChair.Draw(ShadowmapShader);
				}
			}

			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(55.0f, 6.7f, 28.0f));
			//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			//ShadowmapShader.Bind();
			//ShadowmapShader.SetMat4("model", model);
			//Flower.Draw(ShadowmapShader);
			//
			//GLCALL(glCullFace(GL_BACK));
			model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(18.5f, 5.75f, -19.5f));
			model = glm::translate(model, glm::vec3(18.5f, 6.0f, -20.0f));
			//Width Scale to 0.09, height scaled to 0.05, thickness scaled to 0.05
			model = glm::scale(model, glm::vec3(0.090f, 0.050f, 0.05f));
			CurtainShadowmapShader.Bind();
			CurtainShadowmapShader.SetMat4("model", model);
			CurtainShadowmapShader.SetFloat("time", glfwGetTime());
			Curtain.Draw(CurtainShadowmapShader);
			GLCALL(glCullFace(GL_BACK));

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


		
		
		// 2. Render scene as normal 
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		glm::mat4 view = cam.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(cam.GetFOV()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 500.0f);

		LightShader.Bind();
		LightShader.SetMat4("view", view);
		LightShader.SetMat4("projection", projection);


		InstanceShader.Bind();
		InstanceShader.SetMat4("view", view);
		InstanceShader.SetMat4("projection", projection);
		InstanceShader.SetVec3("viewPos", cam.GetPos());
		InstanceShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		ModelShader.Bind();
		ModelShader.SetMat4("view", view);
		ModelShader.SetMat4("projection", projection);
		ModelShader.SetVec3("viewPos", cam.GetPos());
		ModelShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);


		Lights1.ShowPointLight(LightShader);
		//Load Light Uniforms to shaders
		Lights1.CastLight(InstanceShader);
		Lights1.CastLight(ModelShader);
		Lights1.CastLight(FloorShader);
		Lights1.CastLight(WallShader);
		Lights1.CastLight(CurtainShader);


		CurtainShader.Bind();
		CurtainShader.SetMat4("view", view);
		CurtainShader.SetMat4("projection", projection);
		CurtainShader.SetVec3("viewPos", cam.GetPos());
		CurtainShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		FloorShader.Bind();
		FloorShader.SetMat4("view", view);
		FloorShader.SetMat4("projection", projection);
		FloorShader.SetVec3("viewPos", cam.GetPos());
		FloorShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);


		WallShader.Bind();
		WallShader.SetMat4("view", view);
		WallShader.SetMat4("projection", projection);
		WallShader.SetVec3("viewPos", cam.GetPos());
		WallShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);


		CherryShader.Bind();
		CherryShader.SetMat4("view", view);
		CherryShader.SetMat4("projection", projection);
		CherryShader.SetVec3("skyColor", glm::vec3(1.0, 1.0, 1.0));


		GLCALL(glActiveTexture(GL_TEXTURE8));
		GLCALL(glBindTexture(GL_TEXTURE_2D, Shadow1.GetDepthMap()));

		//GLCALL(glActiveTexture(GL_TEXTURE0));
		//******Draw stage*******
		glm::mat4 model = glm::mat4(1.0f);
		//Draw Floor
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-15.0f + i * 10.0f, 0.0f, -15.0f + j * 10.0f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(Floor.GetWidth() / 2, Floor.GetHeight() / 2, 1.0f));
				FloorShader.Bind();
				FloorShader.SetMat4("model", model);
				Floor.Draw(FloorShader, FloorTexture,Shadow1.GetDepthMap(),8);
			}
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-15.0f + i * 10.0f, 19.5f, -15.0f + j * 10.0f));
				model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(Floor.GetWidth() / 2, Floor.GetHeight() / 2, 1.0f));
				FloorShader.Bind();
				FloorShader.SetMat4("model", model);
				Floor.Draw(FloorShader, FloorTexture, Shadow1.GetDepthMap(), 8);
			}
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-15.0f + i * 10.0f, 20.1f, -15.0f + j * 10.0f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(Floor.GetWidth() / 2, Floor.GetHeight() / 2, 1.0f));
				FloorShader.Bind();
				FloorShader.SetMat4("model", model);
				Floor.Draw(FloorShader, FloorTexture, Shadow1.GetDepthMap(), 8);
			}
		}
		//Draw Wall
		model = glm::mat4(1.0f);
		//Left 
		for (int i = 0; i < 9; i++)
		{

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-15.0f + 10.0f * i, 10.0f, 70.0f));
			model = glm::scale(model, glm::vec3(Wall.GetWidth() / 2, Wall.GetHeight() / 2, 1.0f));
			WallShader.Bind();
			WallShader.SetMat4("model", model);
			Wall.Draw(WallShader, WallTexture,Shadow1.GetDepthMap(),8);

		}
		//Front
		for (int i = 0; i < 9; i++)
		{

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(70.0f, 10.0f, -15.0f + 10.0f * i));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(Wall.GetWidth() / 2, Wall.GetHeight() / 2, 1.0f));
			WallShader.Bind();
			WallShader.SetMat4("model", model);
			Wall.Draw(WallShader, WallTexture, Shadow1.GetDepthMap(), 8);

		}
		//Back
		for (int i = 0; i < 9; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-20.0f, 10.0f, -15.0f + 10.0f * i));
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(Wall.GetWidth() / 2, Wall.GetHeight() / 2, 1.0f));
			WallShader.Bind();
			WallShader.SetMat4("model", model);
			Wall.Draw(WallShader, WallTexture, Shadow1.GetDepthMap(), 8);

		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(69.8f, 12.0f, 25.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(BlackBoard.GetWidth() / 2, BlackBoard.GetHeight() / 2, 1.0f));
		WallShader.Bind();
		WallShader.SetMat4("model", model);
		BlackBoard.Draw(WallShader, BlackBoardTexture,Shadow1.GetDepthMap(),8);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(65.0f, 7.5f, 69.5f));
		model = glm::scale(model, glm::vec3(Door.GetWidth() / 2 - 1.0f, Door.GetHeight() / 2, 1.0f));
		WallShader.Bind();
		WallShader.SetMat4("model", model);
		Door.Draw(WallShader, DoorTexture,Shadow1.GetDepthMap(),8);


		//Draw tdesk
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));	// it's a bit too big for our scene, so scale it down
		ModelShader.Bind();
		ModelShader.SetMat4("model", model);
		TDesk.Draw(ModelShader);


		InstanceShader.Bind();
		SChairInstances.Draw(SDesk, InstanceShader);
		SChairInstances.Draw(SChair, InstanceShader);
		InstanceShader.Unbind();

		//for (int row = 0; row < 6; row++) {
		//	for (int col = 0; col < 6; col++)
		//	{
		//		//Draw sdesk
		//		model = glm::mat4(1.0f);
		//		model = glm::translate(model, glm::vec3(col * 10.0f, 0.0f, row * 10.0f));
		//		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//		ModelShader.Bind();
		//		ModelShader.SetMat4("model", model);
		//		SDesk.Draw(ModelShader);
		//
		//		//Draw schair
		//		model = glm::mat4(1.0f);
		//		model = glm::translate(model, glm::vec3(col * 10.0f - 10.0f, 0.0f, row * 10.0f));
		//		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//		ModelShader.Bind();
		//		ModelShader.SetMat4("model", model);
		//		SChair.Draw(ModelShader);
		//	}
		//}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(18.5f, 5.5f, -19.5f));
		//Width Scale to 0.09, height scaled to 0.05, thickness scaled to 0.05
		model = glm::scale(model, glm::vec3(0.090f, 0.05f, 0.05f));
		//model = glm::scale(model, glm::vec3(0.090f, 0.055f, 0.05f));
		CurtainShader.Bind();
		CurtainShader.SetMat4("model", model);
		CurtainShader.SetFloat("time", glfwGetTime());
		Curtain.Draw(CurtainShader);

		//// Draw the flower
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(55.0f, 6.7f, 28.0f));
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//ModelShader.Bind();
		//ModelShader.SetMat4("model", model);
		////Flower.Draw(ModelShader);


		// Draw Cherry Particales
		p.update1(deltaTime);
		p.Draw1();
		p.update2(deltaTime);
		p.Draw2();



		//Draw Skybox
		glm::mat4 view_skybox = glm::mat4(glm::mat3(cam.GetViewMatrix()));
		SkyboxShader.Bind();
		SkyboxShader.SetMat4("projection", projection);
		SkyboxShader.SetMat4("view", view_skybox);
		skybox.Draw(SkyboxShader);
		SkyboxShader.Unbind();

		const float Pointx1 = -10.0f;
		const float Pointx2 = 10.0f;
		const float Pointy1 = 20.0f;
		const float Pointy2 = 25.0f;
		const float Pointz1 = -10.0f;
		const float Pointz2 = 10.0f;

		glLineWidth(10.0f); // 设置线条粗细
		glColor3f(1.0f, 0.0f, 0.0f); // 设置线条颜色为红色
		glBegin(GL_LINES);
		glVertex3f(Pointx1, Pointy1, Pointz1); // 起点坐标
		glVertex3f(Pointx2, Pointy1, Pointz1); // 终点坐标

		glVertex3f(Pointx1, Pointy1, Pointz1); // 起点坐标
		glVertex3f(Pointx1, Pointy1, Pointz2); // 终点坐标

		glVertex3f(Pointx1, Pointy1, Pointz2); // 起点坐标
		glVertex3f(Pointx2, Pointy1, Pointz2); // 终点坐标

		glVertex3f(Pointx2, Pointy1, Pointz1); // 起点坐标
		glVertex3f(Pointx2, Pointy1, Pointz2); // 终点坐标


		glVertex3f(Pointx1, Pointy2, Pointz1); // 起点坐标
		glVertex3f(Pointx2, Pointy2, Pointz1); // 终点坐标

		glVertex3f(Pointx1, Pointy2, Pointz1); // 起点坐标
		glVertex3f(Pointx1, Pointy2, Pointz2); // 终点坐标

		glVertex3f(Pointx1, Pointy2, Pointz2); // 起点坐标
		glVertex3f(Pointx2, Pointy2, Pointz2); // 终点坐标

		glVertex3f(Pointx2, Pointy2, Pointz1); // 起点坐标
		glVertex3f(Pointx2, Pointy2, Pointz2); // 终点坐标

		glVertex3f(Pointx1, Pointy1, Pointz1); // 起点坐标
		glVertex3f(Pointx1, Pointy2, Pointz1); // 终点坐标

		glVertex3f(Pointx2, Pointy1, Pointz1); // 起点坐标
		glVertex3f(Pointx2, Pointy2, Pointz1); // 终点坐标

		glVertex3f(Pointx1, Pointy1, Pointz2); // 起点坐标
		glVertex3f(Pointx1, Pointy2, Pointz2); // 终点坐标

		glVertex3f(Pointx2, Pointy1, Pointz2); // 起点坐标
		glVertex3f(Pointx2, Pointy2, Pointz2); // 终点坐标

		glEnd();

		ShowFps(deltaTime);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}