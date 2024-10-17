#include <GL/glew.h>
#ifdef	_WIN32
#include <GL/wglew.h>
#else
#include <GL/glxew.h>
#endif
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "auxiliary/shader_manager.h"
#include "auxiliary/model_manager.h"

GLuint program0;
model model0;
float scale = 0.5;
float angle = 0;
int model_choice = 0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program0);
	
	glm::mat4 model = glm::mat4(1.0f);
	if (model_choice == 1)
	{
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
	}
	else if (model_choice == 2)
	{
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(0.0f, -7.0f, -13.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	}
	glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model)));
	
	glUniformMatrix4fv(glGetUniformLocation(program0, "transform.model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(glGetUniformLocation(program0, "transform.normal_mat"), 1, GL_FALSE, glm::value_ptr(normal_mat));
	
	model0.display_model(program0);
	glUseProgram(0);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glUseProgram(program0);
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(program0, "transform.projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);
	glutPostRedisplay();
}

void update(int val)
{
	angle++;
	if (angle > 360) 
		angle -= 360;
	glutPostRedisplay();
	glutTimerFunc(30, update, 0);
}

void key(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q' || key == 'Q')
		exit(0);
}

void mouse_wheel(int wheel, int dir, int mouseX, int mouseY)
{
	if (dir > 0)
		scale += 0.05;
	else
		scale -= 0.05;
	glUseProgram(program0);
	glUniform1f(glGetUniformLocation(program0, "scale"), scale);
	glUseProgram(0);
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");
	int choice = 0;
	while (1)
	{
		std::cout << "Введите значение, соответствующее желаемой модели освещения:" << std::endl;
		std::cout << "1 - Lambert\n2 - Toon shading\n3 - Oren-Nayar\n4 - Phong\n5 - Cook-Torrance\n6 - Ashikhmin-Shirley\n-1 - выход из программы" << std::endl;
		std::cin >> choice;
		if (choice == -1)
			return 0;
		else if (choice >= 1 && choice <= 6)
			break;
		std::cout << "Введено некорректное значение, пожалуйста, повторите попытку, введя одно из указанных значений.\n" << std::endl;
	}
	float roughness, sheen_coef, metal_prop, refr_index, n_u, n_v;
	switch (choice)
	{
		case 3:
			std::cout << "\nВведите значение шероховатости:" << std::endl;
			std::cin >> roughness;
			if (roughness < 0 || roughness > 1)
			{
				roughness = 0;
				std::cout << "Введенное значение не лежит в диапазоне [0; 1], будет использовано значение 0" << std::endl;
			}
			break;
		case 4:
			std::cout << "\nВведите коэффициент блеска:" << std::endl;
			std::cin >> sheen_coef;
			if (sheen_coef < 1)
			{
				sheen_coef = 1;
				std::cout << "Введенное значение меньше 1, будет использовано значение 1" << std::endl;
			}
			break;
		case 5:
			std::cout << "\nВведите значение шероховатости:" << std::endl;
			std::cin >> roughness;
			if (roughness < 0 || roughness > 1)
			{
				roughness = 0;
				std::cout << "Введенное значение не лежит в диапазоне [0; 1], будет использовано значение 0" << std::endl;
			}
			std::cout << "Введите индекс преломления:" << std::endl;
			std::cin >> refr_index;
			if (refr_index < 0)
			{
				refr_index = 1;
				std::cout << "Введенное значение меньше 0, будет использовано значение 1" << std::endl;
			}
			std::cout << "Введите коэффициент металлического свойства:" << std::endl;
			std::cin >> metal_prop;
			if (metal_prop < 0 || metal_prop > 1)
			{
				metal_prop = 0;
				std::cout << "Введенное значение не лежит в диапазоне [0; 1], будет использовано значение 0" << std::endl;
			}
			break;
		case 6:
			std::cout << "\nВведите индекс преломления:" << std::endl;
			std::cin >> refr_index;
			if (refr_index < 0)
			{
				refr_index = 1;
				std::cout << "Введенное значение меньше 0, будет использовано значение 1" << std::endl;
			}
			std::cout << "Введите значение параметра n_u:" << std::endl;
			std::cin >> n_u;
			if (n_u < 0)
			{
				n_u = 100;
				std::cout << "Введенное значение меньше 0, будет использовано значение 100" << std::endl;
			}
			std::cout << "Введите значение параметра n_v:" << std::endl;
			std::cin >> n_v;
			if (n_v < 0)
			{
				n_v = 100;
				std::cout << "Введенное значение меньше 0, будет использовано значение 100" << std::endl;
			}
			break;
	}
	while (1)
	{
		std::cout << "\nВыберите модель для демонстрации:" << std::endl;
		std::cout << "1 - Техно-сфера\n2 - Шестерня\n3 - Фигурка человека\n-1 - выход из программы" << std::endl;
		std::cin >> model_choice;
		if (model_choice == -1)
			return 0;
		else if (model_choice >= 1 && model_choice <= 3)
			break;
		std::cout << "Введено некорректное значение, пожалуйста, повторите попытку, введя одно из указанных значений.\n" << std::endl;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 900);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Lighting models");
	glewExperimental = GL_TRUE;
	glewInit();
	if (!GLEW_VERSION_3_3)
	{
		printf("OpenGL 3.3 not supported.\n");
		return 1;
	}
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glEnable(GL_DEPTH_TEST);
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutMouseWheelFunc(mouse_wheel);
	glutTimerFunc(33, update, 0);
	if (!GL_ARB_vertex_array_object)
		printf("No VAO support\n");
	
	shader my_shader_phong = shader("shaders/phong_shad.vert", "shaders/phong_shad.frag");
	shader my_shader_oren = shader("shaders/oren_nayar_shad.vert", "shaders/oren_nayar_shad.frag");
	shader my_shader_toon = shader("shaders/toon_shad.vert", "shaders/toon_shad.frag");
	shader my_shader_shirley = shader("shaders/a_shirley_shad.vert", "shaders/a_shirley_shad.frag");
	shader my_shader_cook = shader("shaders/cook_torr_shad.vert", "shaders/cook_torr_shad.frag");
	shader my_shader_lambert = shader("shaders/lambert_shad.vert", "shaders/lambert_shad.frag");
	switch (choice)
	{
		case 1:		
			program0 = my_shader_lambert.ID;
			glUseProgram(program0);
			glUniform3f(glGetUniformLocation(program0, "light.diffuse"), 0.62f, 0.4f, 0.82f);
			glUniform3f(glGetUniformLocation(program0, "light.position"), 0.0f, 0.8f, 3.0f);
			break;
		case 2:
			program0 = my_shader_toon.ID;
			glUseProgram(program0);
			glUniform3f(glGetUniformLocation(program0, "light.diffuse"), 0.62f, 0.4f, 0.82f);
			glUniform3f(glGetUniformLocation(program0, "light.position"), 0.0f, 0.8f, 3.0f);
			break;		
		case 3:
			program0 = my_shader_oren.ID;
			glUseProgram(program0);
			glUniform3f(glGetUniformLocation(program0, "light.diffuse"), 0.62f, 0.4f, 0.82f);
			glUniform3f(glGetUniformLocation(program0, "light.position"), 0.0f, 0.8f, 3.0f);
			glUniform1f(glGetUniformLocation(program0, "roughness"), 0);
			break;
		case 4:	
			program0 = my_shader_phong.ID;
			glUseProgram(program0);
			glUniform3f(glGetUniformLocation(program0, "material.ambient"), 0.62f, 0.4f, 0.82f);
			glUniform3f(glGetUniformLocation(program0, "material.diffuse"), 0.62f, 0.4f, 0.82f);
			glUniform3f(glGetUniformLocation(program0, "material.specular"), 0.5f, 0.5f, 0.5f);
			glUniform1f(glGetUniformLocation(program0, "material.sheen_coef"), sheen_coef);
			glUniform3f(glGetUniformLocation(program0, "light.ambient"), 0.3f, 0.3f, 0.3f);
			glUniform3f(glGetUniformLocation(program0, "light.diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(program0, "light.specular"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(program0, "light.position"), 0.0f, 0.8f, 3.0f);
			break;
		case 5:
			program0 = my_shader_cook.ID;
			glUseProgram(program0);
			glUniform3f(glGetUniformLocation(program0, "material_diffuse"), 1.0f, 0.86f, 0.57f);
			glUniform3f(glGetUniformLocation(program0, "light.position"), 0.0f, 0.8f, 5.0f);
			glUniform3f(glGetUniformLocation(program0, "light.color"), 1.0f, 0.86f, 0.57f);
			glUniform1f(glGetUniformLocation(program0, "roughness"), roughness);
			glUniform1f(glGetUniformLocation(program0, "metal_prop"), metal_prop);
			glUniform1f(glGetUniformLocation(program0, "refr_index"), refr_index);
			break;
		default:
			program0 = my_shader_shirley.ID;
			glUseProgram(program0);
			glUniform2f(glGetUniformLocation(program0, "ashikhmin_anisotropy"), n_u, n_v);
			glUniform1f(glGetUniformLocation(program0, "refr_index"), refr_index);
			glUniform3f(glGetUniformLocation(program0, "light.diffuse"), 0.62f, 0.4f, 0.82f);
			glUniform3f(glGetUniformLocation(program0, "light.specular"), 0.8f, 0.8f, 0.8f);
			glUniform3f(glGetUniformLocation(program0, "light.position"), 0.75f, 0.3f, 10.0f);
			break;		
	}
	if (model_choice == 1)
		model0 = model("3d_models/weird sphere.obj");
	else if (model_choice == 2)
		model0 = model("3d_models/Gear1.obj");
	else
		model0 = model("3d_models/GLman02.obj");
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 900.0f, 0.1f, 100.0f);
		
	glUniformMatrix4fv(glGetUniformLocation(program0, "transform.view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program0, "transform.projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(program0, "view_pos"), 0.0f, 0.0f, 3.0f);
	glUniform1f(glGetUniformLocation(program0, "scale"), scale);
	glUseProgram(0);
	glutMainLoop();
	return 0;
}