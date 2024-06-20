#include "config.h"
#include "loadShader.h"
#include "player.h"
#include "solids.h"
#include "core.h"
#include "walls.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void generateTexture(const std::string filename);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void moveSandpile(int startX, int startY, int offsetX, int offsetY);

coreController* core = new coreController();
player* playerObject = new player(core);
player* playerObject2 = new player(core);
wallController* walls = new wallController(core);
solidController* solids = new solidController(core);

int main()
{
	playerObject2->x = 32;
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "test", NULL, NULL);

	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glClearColor(0.25,0.25,0.75,1.0);
	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);	//input management

	playerObject->setup();
	playerObject2->setup();
	solids->setup();
	walls->setup();

	int w;
	int a;
	int s;
	int d;

	while(!glfwWindowShouldClose(window))
	{
		//input management
		w = glfwGetKey(window, GLFW_KEY_W);
		a = glfwGetKey(window, GLFW_KEY_A);
		s = glfwGetKey(window, GLFW_KEY_S);
		d = glfwGetKey(window, GLFW_KEY_D);

		
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		walls->step();
		walls->draw();

		playerObject->step();
		playerObject->draw();

		playerObject2->step();
		playerObject2->draw();

		solids->step();
		solids->draw();
		
		
		glfwSwapBuffers(window);
	}

	delete playerObject;
	//delete playerObject2;
	delete solids;
	delete walls;
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		playerObject->up();
		playerObject2->up();
	}


	if(key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		playerObject->right();
		playerObject2->right();
	}

	if(key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		playerObject->down();
		playerObject2->down();
	}

	if(key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		playerObject->left();
		playerObject2->left();
	}
}
