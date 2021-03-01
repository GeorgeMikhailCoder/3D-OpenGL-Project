varying vec4 pos; // координата вершины для передачи во фрагментный шейдер
varying vec2 texCoord; // координата текстуры для передачи во фрагментный шейдер
attribute float vertexBrightness; // цвет вершины
varying float fragBrightness; // значение атрибута для передачи во фрагментный шейдер


uniform vec3 u_light_brightness, u_light, u_eye;
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

vec3 normir(vec3 a)
{
	return a/norm(a);
}

vec3 coord_obs2usl(vec4 obs)
{
return vec3(
	obs[0]/obs[3],
	obs[1]/obs[3],
	obs[2]/obs[3]);
}

void main() {
  //шейдер
	
	vec4 tempNormal = vec4(gl_Normal[0],gl_Normal[1],gl_Normal[2],0);
	tempNormal = gl_ModelViewProjectionMatrix*tempNormal;
	normal = normir(coord_obs2usl(tempNormal));		//перевод в обычные координаты
	//normal = gl_Normal;

	vertex = normir(gl_ModelViewProjectionMatrix*gl_Vertex);

	//текстура
  	fragColor = gl_Color; // передаём атрибут во фрагментный шейдер
  	texCoord = gl_MultiTexCoord0.xy; // передаём координату текстуры во фрагментный шейдер
  	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // координата на экране
  }
