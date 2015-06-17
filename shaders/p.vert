#version 330

// 24576-number of vertices of terrain


layout(location=0)in vec2 position;
layout(location=1)in vec2 uv;

uniform mat4 matrices[3];

out vec2 texCoords;


void main(){
	if(gl_VertexID<24576)gl_Position=matrices[0]*vec4(position, 1.0, 1.0);
	if(gl_VertexID>=24576 && gl_VertexID<24576+6 )gl_Position=vec4(position, 1.0, 1.0);
	if(gl_VertexID>=24576+6 && gl_VertexID<24576+12 ) gl_Position=matrices[1]*vec4(position, 1.0, 1.0);
	if(gl_VertexID>=24576+12 && gl_VertexID<24576+18 ) gl_Position=matrices[2]*vec4(position, 1.0, 1.0);
	texCoords=uv;
}



/*
zrobi� dwa vbo, jedno statyczne, zawierajace teren i to co si� nie zmienia
drugie dynamiczne na to co si� zmienia, to drugie niech zawiera particle(napisy, pociski)

DO ZROBIENIA!!!!!!!!!!!!!!
	wyswietlanie napis�w
	animacje
	cz�steczki
	poruszanie wieloma obiektami usprawni�
	
*/


	// if(ID==-1)gl_Position=vec4(position, 1.0, 1.0);
	// if(ID==0) gl_Position=matrices[0]*vec4(position, 1.0, 1.0);
	// if(ID==1) gl_Position=matrices[1]*vec4(position, 1.0, 1.0);
	// texCoords=uv;
// }

/*28.06.2012
napisy przekaza� jako uniform;
wymiary ekranu te�
w VBO(statycznym) trzyma� same -1ki
korzystaj�c z gl_VertexID rozsuwa� wierzcho�ki
*/