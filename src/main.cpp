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
player* playerObject = new player();
wallController* walls = new wallController(core);
solidController* solids = new solidController();

int main()
{
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

		solids->step();
		solids->draw();
		
		
		glfwSwapBuffers(window);
	}

	delete playerObject;
	delete solids;
	delete walls;
	glfwTerminate();
	return 0;
}

void moveSandpile(int startX, int startY, int offsetX, int offsetY)
{
	if(solids->solids[startX + offsetX][startY + offsetY].type() == typeid(solidController::sandpile))
	{
		std::cout << "pushing sandpile" << "\n";
		moveSandpile(startX + offsetX, startY + offsetY, offsetX, offsetY);
	}

	const solidController::sandpile sandpile = boost::any_cast<solidController::sandpile>(solids->solids[startX][startY]);
	solids->solids[startX][startY] = 0;
	solids->solids[startX + offsetX][startY + offsetY] = sandpile;
	std::cout << "moving sandpile" << "\n";

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool checkSandpileCollisions(int x, int y, int offsetX, int offsetY)
{
	bool move = true;
	
	if(solids->solids[x+offsetX][y+offsetY].type() == typeid(solidController::sandpile))
	{
		move = checkSandpileCollisions(x+offsetX, y+offsetY, offsetX, offsetY);
		std::cout << "checking collisions" << "\n";
	}
	if(x+offsetX < 0 or x+offsetX >= 5 or y+offsetY < 0 or y+offsetY >= 5 or walls->walls[x+offsetX][y+offsetY] != 0)
	{
		move = false;
	}
	return move;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int x = static_cast<int>(playerObject->x/16.f);
	int y = static_cast<int>(playerObject->y/16.f);
	//std::cout << playerObject->x << "\n";
	//std::cout << playerObject->y << "\n";
	//std::cout << x << "\n";
	//std::cout << y << "\n";
    if (key == GLFW_KEY_W && action == GLFW_PRESS && not(walls->walls[x][y+1]) && y+1 < 5)
	{
		bool move = checkSandpileCollisions(x, y, 0, 1);

		if(solids->solids[x][y+1].type() == typeid(solidController::sandpile) && y+2 < 5 && move)
		{
			moveSandpile(x, y+1, 0, 1);
			playerObject->up();
		}
		else if(move)
		{
			playerObject->up();
		}
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS && not(walls->walls[x-1][y]) && x-1 >= 0)
	{
		bool move = checkSandpileCollisions(x, y, -1, 0);

		if(solids->solids[x-1][y].type() == typeid(solidController::sandpile) && x-2 >= 0 && move)
		{
			moveSandpile(x-1, y, -1, 0);
			playerObject->left();
		}
		else if(move)
		{
			playerObject->left();
		}
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS && not(walls->walls[x][y-1]) && y-1 >= 0)
	{
		bool move = checkSandpileCollisions(x, y, 0, -1);

		if(solids->solids[x][y-1].type() == typeid(solidController::sandpile) && y-2 >= 0 && move)
		{
			moveSandpile(x, y-1, 0, -1);
			playerObject->down();
		}
		else if(move)
		{
			playerObject->down();
		}
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS && not(walls->walls[x+1][y]) && x+1 < 5)
	{
		bool move = checkSandpileCollisions(x, y, 1, 0);

		if(solids->solids[x+1][y].type() == typeid(solidController::sandpile) && x+1 < 5 && move)
		{
			moveSandpile(x+1, y, 1, 0);
			playerObject->right();
		}
		else if(move)
		{
			playerObject->right();
		}
	}
}
