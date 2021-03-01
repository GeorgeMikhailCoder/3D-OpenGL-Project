
uniform vec3 u_light_brightness, u_light, u_eye, u_color;
varying vec3 vertex, normal, fragColor;

float dot(vec3 a, vec3 b)
{
	return a[0]*b[0]+
		   a[1]*b[1]+
		   a[2]*b[2];
}

float norm(vec3 a)
{
return sqrt(a[0]*a[0]+
		    a[1]*a[1]+
		    a[2]*a[2]);
}

void main() {
	vec4 tempNormal = vec4(gl_Normal[0],gl_Normal[1],gl_Normal[2],0);
	tempNormal = gl_ModelViewProjectionMatrix*tempNormal;
	
	//перевод в обычные координаты
	normal = vec3(
	tempNormal[0]/tempNormal[3],
	tempNormal[1]/tempNormal[3],
	tempNormal[2]/tempNormal[3]);

	vec4 tempVertex = gl_ModelViewProjectionMatrix*gl_Vertex;
	vertex = vec3(
	tempVertex[0]/tempVertex[3],
	tempVertex[1]/tempVertex[3],
	tempVertex[2]/tempVertex[3]);

	fragColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // координата на экране
}
