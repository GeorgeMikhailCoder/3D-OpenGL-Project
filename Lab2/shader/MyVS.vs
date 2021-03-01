varying vec4 normal, vertex; // координата вершины для передачи во фрагментный шейдер

void main() {
	normal = gl_Normal;
	vertex = gl_Vertex;

const double Bs = 50;
const double Bd = 50;
const double Be = 50;

const double lx = 50;
const double ly = 50;
const double lz = 50;

const double ps = 0.5;
const double pd = 0.5;
const double pa = 0.5;


	gl_Position = glModelViewProjectionMatrix*gl_Vertex;
}