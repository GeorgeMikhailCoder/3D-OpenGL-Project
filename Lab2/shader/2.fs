  uniform vec3 u_light_brightness, u_light, u_eye, u_color;
  varying vec3 vertex, normal, fragColor;
  
  float dot(vec3 a, vec3 b){
	return a[0]*b[0]+
		   a[1]*b[1]+
		   a[2]*b[2];
}
float norm(vec3 a){
    return sqrt(a[0]*a[0]+
		    a[1]*a[1]+
		    a[2]*a[2]);
}

  void main() {
  	
    vec3 light = u_light - vertex;
	vec3 visitant=u_eye-vertex;
	
	vec3 po_koef = vec3(0.9,0.8,0.7); 

	//нормировка
	light = light/norm(light);
	visitant = visitant/norm(visitant);
	vec3 curNormal=normal/norm(normal);

	int f=1;
	vec3 reflected = 2*dot(light, curNormal)*curNormal-light;
	float Bs = pow(dot(visitant, reflected),f)*u_light_brightness[0]*po_koef[0];
	float Bd = dot(light,curNormal)*u_light_brightness[1]*po_koef[1];
	float Ba = u_light_brightness[2]*po_koef[2];

	float brightness = Bs+Bd+Ba;

  	gl_FragColor =  vec4(
  	brightness*fragColor[0],
  	brightness*fragColor[1],
  	brightness*fragColor[2],
  	1);
  }
  
