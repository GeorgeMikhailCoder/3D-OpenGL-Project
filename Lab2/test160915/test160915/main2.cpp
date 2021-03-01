#include <GL/glew.h> // ���� ���� ������ ���� ������ � ������ !
#include <cstdio>
#include <iostream>
#include <ctime>
#include <math.h>
#include <SOIL2/SOIL2.h>
#include <GL/glut.h>

GLdouble eye_x = 7.0, eye_y = 5.0, eye_z = 7.0;
GLdouble rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;

GLdouble light_x = 5.0, light_y = 0.0, light_z = 4.0;

int			mouseOldX = 0;
int			mouseOldY = 0;

/////////////////////////////////////////////////////////////////////////////////



GLuint shaderProgram;


// ������� ��������� ��������� ���� � ��������� ����������
char *readTextFile(char *fn) {
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

GLuint location;
GLuint texLocation;
GLuint attrLocation;

void init() {
	// �������� ������ ������ �������
	glEnable(GL_DEPTH_TEST);

	// ��������� �������� ��� ���������� ������� �� ������
	char* vertexShaderCode = readTextFile("i:\\VS2015\\test160915\\shader\\2.vs");
		
	// ������ ������������� ������ ���������� �������
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);   
	
	// ��������� ������������� ������� � ��� �������� ���
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); 

	// ����������� ������
	glCompileShader(vertexShader); 

	// ��������� ���������� ���������� ���� �������
	GLint success;
	GLchar infoLog[512];
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// ��������� �������� ��� ������������ ������� �� ������
	char* fragmentShaderCode = readTextFile("i:\\VS2015\\test160915\\shader\\2.fs");

	// ������ ������������� ������ ���������� �������
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// ��������� ������������� ������� � ��� �������� ���
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);

	// ����������� ������
	glCompileShader(fragmentShader);

	// ��������� ���������� ���������� ���� �������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// ������ "���������": ������������� ����� ��������
	shaderProgram = glCreateProgram();

	// ��������� � ��������� �������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// ������ (��������) �������� � "���������"
	glLinkProgram(shaderProgram);

	// ��������� ���������� ������ ���������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// ������� �������� ������ �������������� ��������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ��������� uniform-����������
	location = glGetUniformLocation(shaderProgram, "param");

	// ��������� attribute-����������
	attrLocation = glGetAttribLocation(shaderProgram, "attr1");
	
	// ��������� uniform-���������� ��� ��������
	texLocation = glGetUniformLocation(shaderProgram, "texture1");

	glUniform1i(texLocation, 0); // ����������� texLocation � �������� "0"

	GLuint texId;
	glGenTextures(1, &texId); // ������ ���� �������� � �������������� texId
	glActiveTexture(GL_TEXTURE0); // ���������� �������� "0"
	glBindTexture(GL_TEXTURE_2D, texId); // �������� �������� texId, ������������� ��������� texId � GL_TEXTURE0
	
	int width, height;

	// ��������� ��������
	unsigned char *image = 
		SOIL_load_image("i:\\VS2015\\test160915\\shader\\nature.jpg", &width, &height, NULL, SOIL_LOAD_RGB);
	

	// ������ ������ � ���������: �������� �����
	for (int x = 0; x < width/2; x++) { // ������������ ������ �������� ��������
		for (int y = 0; y < height; y++) {
			int indexR = (x + y * width) * 3;
			int indexG = indexR + 1;
			int indexB = indexR + 2;
			image[indexR] = 255 - image[indexR];
			image[indexG] = 255 - image[indexG];
			image[indexB] = 255 - image[indexB];
		}
	}

	// ������� ������� � ������� ��������, �� ���� � texId
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image); // ����������� ������

	
	// ��������� ��������: ��� ����������� ���������� �������� ��� ������� [0,1]
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��������� ��������: ��� ����������� ������� ��� ��������������� �������� 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ������� ������������� ���� �������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

unsigned int start_time = clock();

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int diff = clock() - start_time;

	// ����������� �������� uniform-����������
	float uniformParam = sin(diff / 1000.0) * 0.5 + 0.5;
	glUniform1f(location, uniformParam);
	
	// �������� ������
	glUseProgram(shaderProgram);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);
	glRotatef(rot_z, 0, 0, 1);

	
	glScalef(5.0, 5.0, 5.0);

	//glColor3f(0.0, 0.0, 1.0);
	//glutSolidCube(1.0);
	//glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0); // ���������� ��������
	glVertexAttrib1f(attrLocation, 0.0); // �������� ��������
	glVertex3f(0.0, 0.0, 0.0); // ���������� �������

	glTexCoord2f(1.0, 0.0); 
	glVertexAttrib1f(attrLocation, 1.0);
	glVertex3f(1.0, 0.0, 0.0);
	
	glTexCoord2f(1.0, 1.0);
	glVertexAttrib1f(attrLocation, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	
	glTexCoord2f(0.0, 1.0);
	glVertexAttrib1f(attrLocation, 1.0);
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
	if (key == 27 || key == 'q' || key == 'Q') // �����
		exit(0);
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
