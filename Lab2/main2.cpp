#include <glew.h> // этот файл должен идти первым в списке !
#include <cstdio>
#include <iostream>
#include <ctime>
#include <math.h>
#include <SOIL2.h>
#include <glut.h>

GLdouble eye_x = 7.0, eye_y = 7.0, eye_z = 7.0;
GLdouble rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;

GLdouble light_x = 0.0, light_y = 7.0, light_z = 7.0;

int			mouseOldX = 0;
int			mouseOldY = 0;

/////////////////////////////////////////////////////////////////////////////////

GLuint shaderProgram;
GLuint location;
GLuint texLocation;
GLuint vertexBrightness;
GLuint u_eye;
GLuint u_light;
GLuint u_light_brightness;


// Функция считывает текстовый файл в строковую переменную
char *readTextFile(const char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		errno_t error = fopen_s(&fp, fn, "rt");
		if (error == 0) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

GLuint initShader(GLenum type, const char* src)
{
	// Включаем работу буфера грубины
	glEnable(GL_DEPTH_TEST);

	// Считываем исходный код вершинного шейдера из файлов
	char* shaderCode = readTextFile(src);


	// Создаём идентификатор нового вершинного шейдера
	GLuint Shader = glCreateShader(type);

	// Связываем идентификатор шейдера и его исходный код
	glShaderSource(Shader, 1, &shaderCode, NULL);

	// Компилируем шейдер
	glCompileShader(Shader);

	// Проверяем успешность компиляции кода шейдера
	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(Shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return Shader;
}

GLuint createShaderProgram()
{
	GLuint vertexShader = initShader(GL_VERTEX_SHADER, "shader\\3.vs");
	GLuint fragmentShader = initShader(GL_FRAGMENT_SHADER, "shader\\3.fs");

	// Создаём "программу": упорядоченный набор шейдеров
	shaderProgram = glCreateProgram();

	// Добавляем в программу шейдеры
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Сборка (линковка) шейдеров в "программе"
	glLinkProgram(shaderProgram);

	// Проверяем успешность сборки программы
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Удаляем ненужные теперь идентификаторы шейдеров
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

void init() {

	//GLuint shaderProgram = createShaderProgram();
	    // Включаем работу буфера грубины
	    glEnable(GL_DEPTH_TEST);
	    
	    // Считываем исходный код вершинного шейдера из файлов
	    char* vertexShaderCode = readTextFile("shader\\3.vs");
	    
	    	
	    // Создаём идентификатор нового вершинного шейдера
	    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);   
	    
	    // Связываем идентификатор шейдера и его исходный код
	    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); 
	    
	    // Компилируем шейдер
	    glCompileShader(vertexShader); 
	    
	    // Проверяем успешность компиляции кода шейдера
	    GLint success;
	    GLchar infoLog[512];
	    
	    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	    if (!success) {
	    	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	    }
	  
	    // Считываем исходный код фрагментного шейдера из файлов
	    char* fragmentShaderCode = readTextFile("shader\\3.fs");
	    
	    
	    // Создаём идентификатор нового вершинного шейдера
	    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	    
	    // Связываем идентификатор шейдера и его исходный код
	    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	    
	    // Компилируем шейдер
	    glCompileShader(fragmentShader);
	    
	    // Проверяем успешность компиляции кода шейдера
	    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	    
	    if (!success) {
	    	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	    	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	    }
	  
	  	// Создаём "программу": упорядоченный набор шейдеров
	  shaderProgram = glCreateProgram();
	  
	  // Добавляем в программу шейдеры
	  glAttachShader(shaderProgram, vertexShader);
	  glAttachShader(shaderProgram, fragmentShader);
	  
	  // Сборка (линковка) шейдеров в "программе"
	  glLinkProgram(shaderProgram);
	  
	  // Проверяем успешность сборки программы
	  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	  if (!success) {
	  	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	  	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	  }
	  
	  // Удаляем ненужные теперь идентификаторы шейдеров
	  glDeleteShader(vertexShader);
	  glDeleteShader(fragmentShader);

	// Объявляем uniform-переменную
	location = glGetUniformLocation(shaderProgram, "param");
	u_light_brightness = glGetUniformLocation(shaderProgram, "u_light_brightness");
	u_light = glGetUniformLocation(shaderProgram, "u_light");
	u_eye = glGetUniformLocation(shaderProgram, "u_eye");
	
	// Объявляем attribute-переменную
	vertexBrightness = glGetAttribLocation(shaderProgram, "vertexBrightness");
	
	// Объявляем uniform-переменную для текстуры
	texLocation = glGetUniformLocation(shaderProgram, "texture1");

	glUniform1i(texLocation, 0); // привызываем texLocation к текстуре "0"

	GLuint texId;
	glGenTextures(1, &texId); // создаём одну текстуру с идентификаторм texId
	glActiveTexture(GL_TEXTURE0); // активируем текстуру "0"
	glBindTexture(GL_TEXTURE_2D, texId); // выбираем текстуру texId, автоматически связываем texId и GL_TEXTURE0
	
	int width, height;

	// загружаем картинку
	unsigned char *image = 
		SOIL_load_image("height_map\\01.jpg", &width, &height, NULL, SOIL_LOAD_RGB);
	

//	// пример работы с пикселями: инверсия цвета
//	for (int x = 0; x < width/2; x++) { // модифицируем только половину картинки
//		for (int y = 0; y < height; y++) {
//			int indexR = (x + y * width) * 3;
//			int indexG = indexR + 1;
//			int indexB = indexR + 2;
//			image[indexR] = 255 - image[indexR];
//			image[indexG] = 255 - image[indexG];
//			image[indexB] = 255 - image[indexB];
//		}
//	}

	// передаём пиксели в текущую текстуру, то есть в texId
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image); // освобождаем память

	
	// параметры текстуры: как расчитывать координаты текстуры вне отрезка [0,1]
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // повторяем текстуру
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// параметры текстуры: как расчитывать пиксели при масштабировании текстуры 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // линейно интерполируем цвет пикселя
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

unsigned int start_time = clock();

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int diff = clock() - start_time;

	// Присваиваем значание uniform-переменной
	float uniformParam = sin(diff / 1000.0) * 0.5 + 0.5;
	glUniform1f(location, uniformParam);
	glUniform3f(u_light, light_x, light_y,light_z);
	glUniform3f(u_eye, eye_x, eye_y, eye_z);
	glUniform3f(u_light_brightness, 0.8, 0.5, 0.3);


	// включаем шейдер
	glUseProgram(shaderProgram);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);
	glRotatef(rot_z, 0, 0, 1);

	double scale = 6.0;
	glScalef(scale,scale,scale);

	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);


	
	glTexCoord2f(0.0, 0.0); // координата карты высот
	glColor3f(1, 0, 0); //выбор цвета
	glVertex3f(0.0, 0.0, 0.0); // координата вершины
	
	glTexCoord2f(1.0, 0.0); 
			glColor3f(1, 1, 0);
	glVertex3f(1.0, 0.0, 0.0);
	
	glTexCoord2f(1.0, 1.0);
			glColor3f(0, 1, 1);	
	glVertex3f(1.0, 1.0, 0.0);
	
	glTexCoord2f(0.0, 1.0);
			glColor3f(0, 0, 1);
	glVertex3f(0.0, 1.0, 0.0);
	
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
}

void redraw() {
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z,	// eye
		0, 0, 0,				// center
		0.0, 0.0, 1.0);		// up
}

void motion(int x, int y) {
	rot_y -= ((mouseOldY - y) * 180.0f) / 200.0f;
	rot_z -= ((mouseOldX - x) * 180.0f) / 200.0f;
	rot_x = 0;

	if (rot_z > 360)
		rot_z -= 360;

	if (rot_z < -360)
		rot_z += 360;

	if (rot_y > 360)
		rot_y -= 360;

	if (rot_y < -360)
		rot_y += 360;

	mouseOldX = x;
	mouseOldY = y;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mouseOldX = x;
		mouseOldY = y;
	}
}

void key(unsigned char key, int x, int y) {
	if (key == 27 || key == 'q' || key == 'Q') // выход
		exit(0);
	else if (key == 'w' || key == 'W')eye_x += 0.1;
	else if (key == 's' || key == 'S')eye_x -= 0.1;
	else if (key == 'a' || key == 'A')eye_y += 0.1;
	else if (key == 'd' || key == 'D')eye_y -= 0.1;
	else if (key == 'c' || key == 'C')eye_z += 0.1;
	else if (key == 'v' || key == 'V')eye_z -= 0.1;
	
	//  glMatrixMode(GL_MODELVIEW);
	//  
	//  glRotatef(-rot_x, 1, 0, 0);
	//  glRotatef(-rot_y, 0, 1, 0);
	//  glRotatef(-rot_z, 0, 0, 1);
	//  gluLookAt(eye_x, eye_y, eye_z,	// eye
	//  	0, 0, 0,				// center
	//  	0.0, 0.0, 1.0);		// up
	//  glRotatef(rot_x, 1, 0, 0);
	//  glRotatef(rot_y, 0, 1, 0);
	//  glRotatef(rot_z, 0, 0, 1);
	//  
	//  
	//  redraw();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	
	glutCreateWindow("GLEW Test");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		getchar();
		//exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(redraw);
	init();
	glutMainLoop();
	return 0;
}
