#include "Common.h"

std::vector<glm::vec3> vectorvertices, vectorvertices1;
std::vector<glm::vec2> vectoruvs, vectoruvs1;
std::vector<glm::vec3> vectornormals, vectornormals1;
int verticevectorsize, verticevectorsize1;
std::vector<unsigned short> indices, indices1;
std::vector<glm::vec3> indexed_vertices, indexed_vertices1;
std::vector<glm::vec2> indexed_uvs, indexed_uvs1;
std::vector<glm::vec3> indexed_normals, indexed_normals1;

extern GLuint MatrixID;

GLuint programID;
GLuint TextureID;
GLuint Texture;
GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;

void initVBO(char*modelfile, char*texturefile)
{
	loadShaders(programID, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");

	int imgwidth, imgheight;
	bool hasAlpha;
	//Load the texture
	Texture = loadPngImage(texturefile, imgwidth, imgheight, hasAlpha, &textureImage);
		//loadBMP_custom("test.bmp");
		//LoadBMP("test.bmp");
		//loadDDS("uvmap.DDS");
	
	//Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTexture");
	MatrixID = glGetUniformLocation(programID, "MVP");

	//Get a handle for our "LightPosition" uniform
	//GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	/*
	if(bool rslt = loadmyFBX("test1.fbx", vectorvertices, vectoruvs, vectornormals) == 1)
		printf("TEST %d\n", rslt);
	else
	{
		printf("DERP: %d\n", rslt);
	}
	*/

	loadOBJ(modelfile, vectorvertices, vectoruvs, vectornormals);
	indexVBO(vectorvertices, vectoruvs, vectornormals, indices, indexed_vertices, indexed_uvs, indexed_normals);
	//VAO
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	//VERTEXBUFFER
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	//UVBUFFER
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	//NORMALBUFFER
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	//INDICEBUFFERS
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void loadVBO()
{
	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);//Bind our texture in Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);//Set "myTextureSampler" sampler to use Texture Unit 0

	//1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void deleteVBO()
{
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	//glDeleteProgram(programID);
}