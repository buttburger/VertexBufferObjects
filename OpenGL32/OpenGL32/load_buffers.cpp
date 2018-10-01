#include "Common.h"

extern GLuint MatrixID;
extern glm::mat4 projection, view, ModelMatrix, MVP, RotationMatrix, TranslationMatrix, ScalingMatrix;

extern float deltaTime;
extern float rotX, rotY, rotZ;
extern float modelX, modelY, modelZ;

GLuint programID;
GLuint TextureID;
GLuint Texture[10];
GLuint VertexArrayID[10];
GLuint vertexbuffer[10];
GLuint uvbuffer[10];
GLuint normalbuffer[10];
GLuint elementbuffer[10];

std::vector<glm::vec3> vectorvertices[10];
std::vector<glm::vec2> vectoruvs[10];
std::vector<glm::vec3> vectornormals[10];
std::vector<unsigned short> indices[10];
std::vector<glm::vec3> indexed_vertices[10];
std::vector<glm::vec2> indexed_uvs[10];
std::vector<glm::vec3> indexed_normals[10];

quat gOrientation2[10];



void initVBO(char*modelfile, char*texturefile, int vboLoop)
{
	loadShaders(programID, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");

	int imgwidth, imgheight;
	bool hasAlpha;
	//Load the texture
	Texture[vboLoop] = loadPngImage(texturefile, imgwidth, imgheight, hasAlpha, &textureImage);
		//loadBMP_custom("test.bmp");
		//LoadBMP("test.bmp");
		//loadDDS("uvmap.DDS");
	
	//Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTexture");
	MatrixID = glGetUniformLocation(programID, "MVP");

	char getFileExtension[64];
	for(int i = strlen(modelfile)-1; i >= 0; i--)
	{
		//printf("strNum: %d\n", i);
		//printf("strChar: %c\n", modelfile[i]);
		getFileExtension[i] = modelfile[i];
		if(strncmp(&getFileExtension[i],".obj", 4) == 0)//if(getFileExtension == ".obj")
		{
			printf(".obj file detected!\n");
			loadOBJ(modelfile, vectorvertices[vboLoop], vectoruvs[vboLoop], vectornormals[vboLoop]);
			if(vboLoop == 0)indexVBO(vectorvertices[vboLoop], vectoruvs[vboLoop], vectornormals[vboLoop], indices[vboLoop], indexed_vertices[vboLoop], indexed_uvs[vboLoop], indexed_normals[vboLoop]);
			if(vboLoop == 1)indexVBO(vectorvertices[vboLoop], vectoruvs[vboLoop], vectornormals[vboLoop], indices[vboLoop], indexed_vertices[vboLoop], indexed_uvs[vboLoop], indexed_normals[vboLoop]);
			break;
		}
		if(strncmp(&getFileExtension[i],".fbx", 4) == 0)//if(getFileExtension == ".obj")
		{
			printf(".fbx file detected!\n");
			//loadmyFBX(modelfile, vectorvertices, vectoruvs, vectornormals);
			//indexVBO(vectorvertices, vectoruvs, vectornormals, indices, indexed_vertices, indexed_uvs, indexed_normals);
			break;
		}
	}
	
	

	/*
	for(int i = 0; i < 36; i++)
	{
		printf("%d polyV X: %f Y: %f Z: %f\n", i, vectorvertices1[i].x, vectorvertices1[i].y, vectorvertices1[i].z);
		printf("%d polyUV X: %f Y: %f\n", i, vectoruvs1[i].x, vectoruvs1[i].y);
		printf("%d polyN X: %f Y: %f Z: %f\n", i, vectornormals1[i].x, vectornormals1[i].y, vectornormals1[i].z);
	}
	*/

	glGenVertexArrays(1, &VertexArrayID[vboLoop]);
	glBindVertexArray(VertexArrayID[vboLoop]);
	//VERTEXBUFFER
	glGenBuffers(1, &vertexbuffer[vboLoop]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[vboLoop]);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices[vboLoop].size() * sizeof(glm::vec3), &indexed_vertices[vboLoop][0], GL_STATIC_DRAW);
	//UVBUFFER
	glGenBuffers(1, &uvbuffer[vboLoop]);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[vboLoop]);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs[vboLoop].size() * sizeof(glm::vec2), &indexed_uvs[vboLoop][0], GL_STATIC_DRAW);
	//NORMALBUFFER
	glGenBuffers(1, &normalbuffer[vboLoop]);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[vboLoop]);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals[vboLoop].size() * sizeof(glm::vec3), &indexed_normals[vboLoop][0], GL_STATIC_DRAW);
	//INDICEBUFFERS
	glGenBuffers(1, &elementbuffer[vboLoop]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[vboLoop]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[vboLoop].size() * sizeof(unsigned short), &indices[vboLoop][0], GL_STATIC_DRAW);

	/*
	if(asdf == 1)
	{
		glGenVertexArrays(1, &VertexArrayID[asdf]);
		glBindVertexArray(VertexArrayID[asdf]);
		//VERTEXBUFFER
		glGenBuffers(1, &vertexbuffer[asdf]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[asdf]);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices[asdf].size() * sizeof(glm::vec3), &indexed_vertices[asdf][0], GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, vectorvertices1.size() * sizeof(glm::vec3), &vectorvertices1[0], GL_STATIC_DRAW);
		//UVBUFFER
		glGenBuffers(1, &uvbuffer[asdf]);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[asdf]);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs[asdf].size() * sizeof(glm::vec2), &indexed_uvs[asdf][0], GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, vectoruvs1.size() * sizeof(glm::vec2), &vectoruvs1[0], GL_STATIC_DRAW);
		//NORMALBUFFER
		glGenBuffers(1, &normalbuffer[asdf]);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[asdf]);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals[asdf].size() * sizeof(glm::vec3), &indexed_normals[asdf][0], GL_STATIC_DRAW);
		//INDICEBUFFERS
		glGenBuffers(1, &elementbuffer[asdf]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[asdf]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[asdf].size() * sizeof(unsigned short), &indices[asdf][0], GL_STATIC_DRAW);
	}
	*/

	vboLoop++;
}

void loadVBO(int vboID)
{
	glUseProgram(programID);

	vec3 desiredDir = glm::vec3(rotX, rotY, rotZ);//gPosition1-gPosition2;
	vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f);//+Y
	//EULER TO QUATERNION
	quat targetOrientation = normalize(qLookAt(desiredDir, desiredUp));//Compute the desired orientation
	gOrientation2[vboID] = RotateTowards(gOrientation2[vboID], targetOrientation, 1.0f*deltaTime);//And interpolate
	//printf("ROT X: %f Y: %f Z: %f W: %f\n", gOrientation2[0], gOrientation2[1], gOrientation2[2], gOrientation2[3]);
	RotationMatrix = mat4_cast(gOrientation2[vboID]);
		
	//RotationMatrix = eulerAngleYXZ(rotX, rotY, rotZ);
	TranslationMatrix = translate(mat4(), glm::vec3(modelX, modelY, modelZ));
	ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));//scaling[0], scaling[1], scaling[2]));
	ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
	MVP = projection * view * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);//Send our transformation to the currently bound shader in the "MVP" uniform

	glActiveTexture(GL_TEXTURE0);//Bind our texture in Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, Texture[vboID]);
	glUniform1i(TextureID, 0);//Set "myTextureSampler" sampler to use Texture Unit 0
	//1st attribute buffer:vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[vboID]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//2nd attribute buffer:UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[vboID]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//3rd attribute buffer:normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[vboID]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[vboID]);
	glDrawElements(GL_TRIANGLES, indices[vboID].size(), GL_UNSIGNED_SHORT, (void*)0);
	
	/*
	if(vboID == 1)
	{
		vec3 desiredDir = glm::vec3(0.01f, 90.0f, 0.01f);//gPosition1-gPosition2;
		vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f);//+Y
		//EULER TO QUATERNION
		quat targetOrientation = normalize(qLookAt(desiredDir, desiredUp));//Compute the desired orientation
		gOrientation2[vboID] = RotateTowards(gOrientation2[vboID], targetOrientation, 1.0f*deltaTime);//And interpolate
		//printf("ROT X: %f Y: %f Z: %f W: %f\n", gOrientation2[0], gOrientation2[1], gOrientation2[2], gOrientation2[3]);
		RotationMatrix = mat4_cast(gOrientation2[vboID]);
		
		//RotationMatrix = eulerAngleYXZ(rotX, rotY, rotZ);
		TranslationMatrix = translate(mat4(), glm::vec3(6.0f, modelY, modelZ));
		ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));//scaling[0], scaling[1], scaling[2]));
		ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
		MVP = projection * view * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);//Send our transformation to the currently bound shader in the "MVP" uniform

		glActiveTexture(GL_TEXTURE0);//Bind our texture in Texture Unit 0
		glBindTexture(GL_TEXTURE_2D, Texture[1]);
		glUniform1i(TextureID, 0);//Set "myTextureSampler" sampler to use Texture Unit 0

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[1]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[1]);
		glDrawElements(GL_TRIANGLES, indices[1].size(), GL_UNSIGNED_SHORT, (void*)0);
	}
	*/
}

void deleteVBO(GLuint vboID)
{
	glDeleteVertexArrays(1, &VertexArrayID[vboID]);
	glDeleteBuffers(1, &vertexbuffer[vboID]);
	glDeleteBuffers(1, &uvbuffer[vboID]);
	glDeleteBuffers(1, &normalbuffer[vboID]);
	glDeleteBuffers(1, &elementbuffer[vboID]);
	glDeleteTextures(1, &Texture[vboID]);

	vectorvertices[vboID].clear(), vectoruvs[vboID].clear(), vectornormals[vboID].clear(), indices[vboID].clear(), indexed_vertices[vboID].clear(), indexed_uvs[vboID].clear(), indexed_normals[vboID].clear();
}

void deleteVBOall()
{
	glDeleteVertexArrays(1, VertexArrayID);
	glDeleteBuffers(1, vertexbuffer);
	glDeleteBuffers(1, uvbuffer);
	glDeleteBuffers(1, normalbuffer);
	glDeleteBuffers(1, elementbuffer);
	glDeleteTextures(1, Texture);
	glDeleteProgram(programID);
}