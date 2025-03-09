#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh(); //0
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();//1
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();//2
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();//3
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}

float anguloOreja1 = 0.0f;  // Ángulo de rotación de la oreja izquierda
float anguloOreja2 = 0.0f;  // Ángulo de rotación de la oreja derecha
float anguloPata1 = 0.0f;  // Ángulo de rotación de la pata delantera izquierda
float anguloPata2 = 0.0f;  // Ángulo de rotación de la pata delantera derecha
float anguloPata3 = 0.0f;  // Ángulo de rotación de la pata trasera izquierda
float anguloPata4 = 0.0f;  // Ángulo de rotación de la pata trasera derecha


void processInput(GLFWwindow* window) {
	// Control de la Oreja 1 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		anguloOreja1 += 1.0f;  // Subir oreja izquierda
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		anguloOreja1 -= 1.0f;  // Bajar oreja izquierda
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		anguloPata1 += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		anguloPata1 -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		anguloPata1 += 1.0f;  // Pata delantera izquierda hacia arriba
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		anguloPata1 -= 1.0f;  // Pata delantera izquierda hacia abajo
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		anguloPata2 += 1.0f;  // Pata delantera derecha hacia arriba
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		anguloPata2 -= 1.0f;  // Pata delantera derecha hacia abajo
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		anguloPata3 += 1.0f;  // Pata trasera izquierda hacia arriba
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		anguloPata3 -= 1.0f;  // Pata trasera izquierda hacia abajo
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		anguloPata4 += 1.0f;  // Pata trasera derecha hacia arriba
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {  // Tecla de coma ","
		anguloPata4 -= 1.0f;  // Pata trasera derecha hacia abajo
	}
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(10, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía
	glm::mat4 modelaux2(1.0);

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;


	while (!mainWindow.getShouldClose())
	{

		processInput(mainWindow.getGLFWWindow());

		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		


		//Ejercicio uno de la practica

		
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -14.0f));
		modelaux = model;
		
		// Base de la grúa
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 1.5f, 4.5f));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[4]->RenderMesh();


		//Entre llantas solo modificamos los diferentes translate
		//Llanta uno
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 2.9f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMeshGeometry();
		
		//Llanta dos
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, -2.9f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMeshGeometry();

		//Llanta tres
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, -2.0f, -2.9f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMeshGeometry();

		//Llanta cuatro
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, -2.0f, 2.9f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[2]->RenderMeshGeometry();
		
		// Cuerpo
		model = modelaux;
		model = glm::scale(model, glm::vec3(5.0f, 2.5f, 3.5f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		//articulación 1
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-2.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//primer brazo que conecta con la cabina
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		meshList[0]->RenderMesh(); //dibuja cubo, pirámide triangular, pirámide base cuadrangular
	
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		model = modelaux;

		//articulación 3 
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		//tercer brazo
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		
		//cabina
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.0f, 4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); 
		

		//Ejercicio numero 2 de la practica

		//model = glm::mat4(1.0);

		//model = glm::translate(model, glm::vec3(0.0f, 4.0f, -14.0f));
		////Almacenamiento en auxiliar
		//modelaux = model;

		////Base 
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(6.0f, 4.0f, 4.5f));
		//color = glm::vec3(0.6f, 0.6f, 0.6f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[2]->RenderMeshGeometry();
		//
		////Cola del animal 
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(6.0f, -0.5f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 0.5f, 0.5f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(110.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		////model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		//color = glm::vec3(0.6f, 0.3f, 0.1f);
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.4f, 0.4f, 0.4f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[0]->RenderMeshGeometry();

		////Pierna 1
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(3.0f, -3.0f, 1.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux2 = model;
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.9f));
		//color = glm::vec3(0.6f, 0.3f, 0.1f); 
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[0]->RenderMesh();


		////Pata 1
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-1.0f, -3.0f, -0.0f));
		//model = glm::rotate(model, glm::radians(anguloPata1), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación controlada
		//model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//color = glm::vec3(0.3f, 0.3f, 0.3f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[4]->RenderMesh();


		////Pierna 2
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(3.0f, -3.0f, -1.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux2 = model;
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.9f));
		//color = glm::vec3(0.6f, 0.3f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[0]->RenderMesh();

		////Pata 2
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-1.0f, -3.0f, -0.0f));
		//model = glm::rotate(model, glm::radians(anguloPata2), glm::vec3(1.0f, 0.0f, 0.0f));  
		//model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//color = glm::vec3(0.3f, 0.3f, 0.3f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[4]->RenderMesh();


		////Pierna 3
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-3.0f, -3.0f, 1.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux2 = model;
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.9f));
		//color = glm::vec3(0.6f, 0.3f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[0]->RenderMesh();

		////Pata 3
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-1.0f, -3.0f, -0.0f));
		//model = glm::rotate(model, glm::radians(anguloPata3), glm::vec3(1.0f, 0.0f, 0.0f)); 
		//model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//color = glm::vec3(0.3f, 0.3f, 0.3f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[4]->RenderMesh();


		////Pierna 4
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-3.0f, -3.0f, -1.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux2 = model;
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.9f));
		//color = glm::vec3(0.6f, 0.3f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[0]->RenderMesh();

		////Pata 4
		//model = modelaux2;
		//model = glm::translate(model, glm::vec3(-1.0f, -3.0f, -0.0f));
		//model = glm::rotate(model, glm::radians(anguloPata4), glm::vec3(1.0f, 0.0f, 0.0f));  
		//model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//color = glm::vec3(0.3f, 0.3f, 0.3f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[4]->RenderMesh();


		////Cuello
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-4.0f, 2.0f, 0.0f));
		//modelaux = model;
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 3.0f, 4.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.7f, 0.7f, 0.7f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		//meshList[0]->RenderMesh();

		////Cabeza
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-2.0f, 2.0f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.9f, 0.9f, 0.9f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//sp.render();

		//// Oreja 1 
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.0f, 1.9f, 3.8f)); 
		//model = glm::rotate(model, glm::radians(anguloOreja1), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.2f, 0.2f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[3]->RenderMeshGeometry();

		//// Oreja 2
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.0f, 1.9f, -3.8f)); 
		//model = glm::rotate(model, glm::radians(anguloOreja2), glm::vec3(0.0f, 0.0f, 1.0f)); 
		//model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(0.2f, 0.2f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[3]->RenderMeshGeometry();


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		