varying vec4 pos; // координата пикселя в пространстве
varying vec2 texCoord; // текстурная координата пикселя
uniform float param; // некоторый параметр, например, время
uniform sampler2D texture1; // текстура

uniform vec3 u_light_brightness, u_light, u_eye;
varying vec3 vertex, normal, fragColor;

float dot(vec3 a, vec3 b){
	return a[0]*b[0]+
		   a[1]*b[1]+
		   a[2]*b[2];
}

vec3 cross(vec3 a, vec3 b)
{
	return vec3(a.y*b.z-a.z*b.y,
				a.z*b.x - a.x*b.z,
				a.x*b.y-a.y*b.x);
}

float norm(vec3 a){
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

float geth(int i, int j)
{
	vec2 coord = texCoord + vec2(i,j)/50;
	vec4 texColor = texture2D(texture1, coord); // цвет из текстуры
	
	//texColor = (2*texColor-1);
	return (texColor[0]+texColor[1]+texColor[2])/3;
}

vec3 imposeNormal(vec3 normalGlobal, vec3 hnormal)
{
	vec3 N = normalGlobal;
	vec3 T;
	vec3 B;
	
	if(N.x!=0)
	{
		B.y=1;
		B.z=0;
		B.x = -(B.y*N.y+B.z*N.z)/N.x;
	}else if(N.y!=0)
	{
		B.x=1;
		B.z=0;
		B.y = -(B.x*N.x+B.z*N.z)/N.y;
	}else
	{
		B.x=1;
		B.y=0;
		B.z = -(B.x*N.x+B.y*N.y)/N.z;
	}

	
	T=normir(cross(B,N));

	//T=abs(T);
//	/*
//	mat3 Mz = mat3(
//	0, -1, 0,
//	1, 0, 0,
//	0, 0, 1);
//	
//	mat3 My = mat3(
//	0, 0, -1,
//	0, 	1, 0,
//	1, 	0, 0
//	);
//
//	mat3 Mx = mat3(
//	1, 0, 0,
//	0, 	0, -1,
//	0, 	1, 0
//	);
//	
//	vec3 Z = Mz*N;
//	vec3 X = Mx*N;
//	vec3 Y = My*N;
//	*/


	mat3 M = mat3(
	T.x, B.x, N.x,
	T.y, B.y, N.y,
	T.z, B.z, N.z
	);

	//N = (0,0,1)
	
	//T = (0,1,0)
	//B = (1,0,0)

	//hnormal = (0,0,1)

	vec3 res = M*hnormal;
	res = (res+1)/2;
	res = abs(res);

	return normir(res);
}

void main() {
	float hij = geth(0,0);
	float hi1j = geth(1,0);
	float hij1 = geth(0,1);
	
	float alpha = 0.9;
	
	vec3 hnormal = vec3(
		-(hi1j-hij)*alpha,
		-(hij1-hij)*alpha,
		1
	);

//шейдер (модель фонга)
	vec3 light = u_light - vertex;
	vec3 visitant=u_eye-vertex;
	
	vec3 po_koef = vec3(0.9,0.5,0.3); 

	//нормировка
	light = light/norm(light);
	visitant = visitant/norm(visitant);
	vec3 curNormal;
	curNormal=imposeNormal(normal,hnormal);
	//curNormal=normal;

	int f=1;
	vec3 reflected = 2*dot(light, curNormal)*curNormal-light;
	float Bs = pow(dot(visitant, reflected),f)*u_light_brightness[0]*po_koef[0];
	float Bd = dot(light,curNormal)*u_light_brightness[1]*po_koef[1];
	float Ba = u_light_brightness[2]*po_koef[2];

	float brightness = Bs+Bd+Ba;
	//brightness=1;


	//текстура
	vec4 texColor = texture2D(texture1, texCoord);
	vec3 curColor;
	curColor = vec3(
	fragColor[0]*texColor[0],
	fragColor[1]*texColor[1],
	fragColor[2]*texColor[2]
	);


	//curColor=curNormal;
	//  int koef = hij>0.5?1:0;
	//  curColor=mix(curColor,fragColor,koef);
	//curColor=vec3(1,0,0);
	//curColor=fragColor;

	//цвет пикселя
  	gl_FragColor =  vec4(
  	brightness*curColor[0],
  	brightness*curColor[1],
  	brightness*curColor[2],
  	1);

}