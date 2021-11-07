
// Shader-ul de fragment / Fragment shader  
 
 #version 400

in vec4 ex_Color;
uniform int codCol;
 
out vec4 out_Color;

void main(void)
  {
	out_Color = ex_Color;

  }
 