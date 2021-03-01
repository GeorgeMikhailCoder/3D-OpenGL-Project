//varying vec4 pos; // координата вершины для передачи во фрагментный шейдер
varying vec2 texCoord; // координата текстуры для передачи во фрагментный шейдер
attribute float attr1; // значение атрибута
varying float attr1_toFragment; // значение атрибута для передачи во фрагментный шейдер

//source version
  void main() {
  	attr1_toFragment = attr1; // передаём атрибут во фрагментный шейдер
  	texCoord = gl_MultiTexCoord0.xy; // передаём координату текстуры во фрагментный шейдер
  	pos = gl_Vertex; // передаём координату вершины во фрагментный шейдер
  	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // координата на экране
  }
