/*#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/maths/maths.h"

#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include "src/graphics/buffers/vertexarray.h"

#include "src/graphics/renderer2d.h"
#include "src/graphics/simple2drenderer.h"

int main()
{
	using namespace sparky;
	using namespace graphics;
	using namespace maths;

	Window window("Sparky! ", 960, 540);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);        // blue colored window
*/
	/*GLfloat vertices[] =
	{
		0, 0, 0,
		0, 3, 0,
		8, 3, 0,
		8, 0, 0
	};

	GLushort indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	GLfloat colorsA[] =                       // 4 as there are 4 vertices
	{
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		1, 0, 1, 1

	};

	GLfloat colorsB[] =                       // 4 as there are 4 vertices
	{
		0.2f, 0.8f, 0.8f, 1,
		0.2f, 0.3f, 0.8f, 1,
		0.8f, 0.3f, 0.8f, 1,
		0.2f, 0.3f, 0.8f, 1
	};


	VertexArray sprite1, sprite2;
	//Buffer* vbo = new Buffer(vertices, 4 * 3, 3);        // 3 as they are vec3
	IndexBuffer ibo(indices, 6);

	sprite1.addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
	sprite1.addBuffer(new Buffer(colorsA, 4 * 4, 4), 1);       // at location 1

	sprite2.addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
	sprite2.addBuffer(new Buffer(colorsB, 4 * 4, 4), 1);       // at location 1

	//vao.addBuffer(vbo, 0);                                // adding buffer at index 0

	*/


/*	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();
	shader.setUniformMat4("pr_matrix", ortho);
	shader.setUniformMat4("ml_matrix", mat4::translation(vec3(4, 3, 0)));

	Renderable2D sprite(maths::vec3(5, 5, 0), maths::vec2(4, 4), maths::vec4(1, 0, 1, 1), shader);
	Renderable2D sprite2(maths::vec3(7, 1, 0), maths::vec2(2, 3), maths::vec4(0.2f, 0, 1, 1), shader);
	Simple2DRenderer renderer;

	shader.setUniform2f("light_pos", vec2(4.0f, 1.5f));
	shader.setUniform4f("colour", vec4(0.2f, 0.3f, 0.8f, 1.0f));

	while (!window.closed())
	{
*/
		/*std::cout << window.getWidth() << ", " <<  window.getHeight() << std::endl;
		if (window.isKeyPressed(GLFW_KEY_A))
		{
			std::cout << "Pressed A " << std::endl;
		}
		if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressed mouse left! " << std::endl;
			
		}
		double x, y;
		window.getMousePosition(x, y);
		std::cout << x << " , " << y << std::endl;*/
/*		window.clear();
		double x, y;
		window.getMousePosition(x, y);
		shader.setUniform2f("light_pos", vec2((float)(x * 16.0f / 960.0f), (float)(9.0f - y * 9.0f / 540.0f)));
*/
		/*
		glDrawArrays(GL_TRIANGLES, 0, 6);

		sprite1.bind();
		ibo.bind();
		shader.setUniformMat4("ml_matrix", mat4::translation(vec3(4, 3, 0)));
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);              // 0 as we are binding our indices
		ibo.unbind();
		sprite1.unbind();
		                                // we need to rebind ibo as it need to be bound when a new vertexarray object is bound
		sprite2.bind();
		ibo.bind();                                                  
		shader.setUniformMat4("ml_matrix", mat4::translation(vec3(0, 0, 0)));
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);              // 0 as we are binding our indices
		ibo.unbind();
		sprite2.unbind();
		*/
/*
		renderer.submit(&sprite);
		renderer.submit(&sprite2);
		renderer.flush();

		window.update();
	}
	//system("PAUSE");
	return 0;
}
*/

#include <iostream>
#include "MainGame.h"


int main(int argc, char** argv)
{
	MainGame maingame;
	maingame.run();	

	
	return 0;
}