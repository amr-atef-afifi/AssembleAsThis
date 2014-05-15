#include "windows.h"
#include "Angel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <common/objloader.hpp>
#include <common/texture.hpp>
#include <iostream>
#include "Cube.h"
#include "mmsystem.h"

using namespace std;
// Functions
void sleepForAWhile(int sleeping_period);
void sleeping(int value);
void initBasicCube();
void highlightPossiblePositions(int** possible_positions);
void putHighlightCube(int n ,int x, int y, int z);
void initHeiglightCubes();
glm::vec3 getProperPosition(GLfloat xf, GLfloat yf, GLfloat zf);
int isHighlighted(int x, int y, int z);
void putHighlightedCubesInWorkspace();
void removeHighlightedCubesInWorkspace();
void rotateCube(glm::vec3 rotation_vector, GLfloat angle);
void applyRotation(int value);
void applyReverseRotation(int value);
void initDesk();



// Game Logic Functios
GLboolean findGroundAll(GLint src_floor, GLint src_y, GLint src_x,GLint dst_floor, GLint dst_y, GLint dst_x);
GLboolean findGround(GLint floor, GLint y, GLint x);
void clearVisited(GLint floor, GLint y, GLint x);
GLboolean isConnected(GLint floor, GLint y, GLint x);
int** getPossibleRotations(GLint in_x, GLint in_y, GLint in_z);
void shiftInReverseX();
void shiftInX();
void shiftInReverseY();
void shiftInY();

void changeIndex(GLint oldX, GLint oldY, GLint oldZ, GLint newX, GLint newY, GLint newZ);
bool checkWin();
void readFile(char* fileName);
void genRand(GLint numOfRotations);

void createCube(glm::mat4 translation_matrix , glm::vec4 color);

//Window Dimensions
const GLfloat WindowWidth = 1366;
const GLfloat WindowHeight = 768;

//Colors
const glm::vec3 RED_COLOR    = glm::vec3(0.7,0.0,0.0);
const glm::vec3 GREEN_COLOR  = glm::vec3(0.0,0.7,0.0);
const glm::vec3 BLUE_COLOR   = glm::vec3(0.0,0.0,0.7);
const glm::vec3 YELLOW_COLOR = glm::vec3(0.7,0.7,0.0);
const glm::vec3 SILVER_COLOR = glm::vec3(0.4,0.4,0.4);
const glm::vec3 DARK_GRAY    = glm::vec3(0.1,0.1,0.1);
const glm::vec3 BLACK_COLOR  = glm::vec3(0.0,0.0,0.0);

// Rotation Axises
const GLint X0 = 0 ;
const GLint X1 = 1 ;
const GLint X2 = 2 ;
const GLint X3 = 3 ;

const GLint Y0 = 4 ;
const GLint Y1 = 5 ;
const GLint Y2 = 6 ;
const GLint Y3 = 7 ;

const GLint Z0 = 8 ;
const GLint Z1 = 9 ;
const GLint Z2 = 10 ;
const GLint Z3 = 11 ;

const glm::vec3 X_AXIS  = glm::vec3(1.0,0.0,0.0);
const glm::vec3 Y_AXIS  = glm::vec3(0.0,1.0,0.0);
const glm::vec3 Z_AXIS  = glm::vec3(0.0,0.0,1.0);

glm::vec3 rotation_vector;
GLfloat rotation_angle;
const int ANIMATION_SPEED = 1;
const int ROTATION_STEP = 5 ;
bool animating = false;
// Zooming Variables

const GLfloat MAX_ZOOMING_VALUE = 1.5;
const GLfloat MIN_ZOOMING_VALUE = 0.3;
const GLfloat ZOOMING_STEP = 0.03;

GLuint vertices_buffer,colors_buffer, uv_buffer, normals_buffer;

// Shader Program
GLuint program ;

// Transformation Matrices
GLint uniModel;
glm::mat4  model;

GLint uniView;
glm::mat4 view ;

GLint uniProj;
glm::mat4 proj;

// Highlighting Data
int** highlightedPositions;

GLint uniHLM_1;
glm::mat4 HLM_1;

GLint uniHLM_2;
glm::mat4 HLM_2;

GLint uniHLM_3;
glm::mat4 HLM_3;

GLint uniHLM_4;
glm::mat4 HLM_4;

GLint uniHLM_5;
glm::mat4 HLM_5;

GLint uniHLM_6;
glm::mat4 HLM_6;

// Selected Cube Data
glm::vec3 selected_cube ;
glm::vec3 dest_cube ;

GLint uniHighlightColor ;
glm::vec4 highlightColor;
GLint uniisSelected ;
GLfloat is_selected ;

GLint uniisDesk;
GLfloat isDesk ;

GLint desk_index;


GLint uniTranslationMatrix;
glm::mat4 translationMatrix;

GLint uniAnimationMatrix;
glm::mat4 animationMatrix;

GLint uniTranslationBackMatrix;
glm::mat4 translationBackMatrix;

//camera co-ordinates
glm::vec3 cam_pos, cam_up; 

GLfloat x_eye , y_eye, z_eye;
GLfloat x_coi , y_coi , z_coi;
GLfloat zooming_value = 1.000;

//co-ordinates of previous mouse location
GLint px = 0, py = 0;

//Basic Cube Data
std::vector<glm::vec3> basic_cube_vertices;
std::vector<glm::vec2> basic_cube_uvs;
std::vector<glm::vec3> basic_cube_normals;
GLint corners_points_num, sides_points_num, faces_points_num, cube_points_num;

GLfloat scaling_ratio = 1.0;
GLfloat translation_amount = 1.0;
GLfloat initial_x_trans = 0.0;
GLfloat initial_z_trans= 0.0;



std::vector<glm::vec3> vertices;
std::vector<glm::vec4> colors;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; 

//workspace
Cube**** workspace;

//Number of avaialble cubes
GLuint NumCubes;

//readfile
Cube**** targetModel;
int** currentPoints;
int** originalPoints;
int noOfCubes;



bool first_time = true;
void init( void )
{
	//initializing workspace (game grid)
	//|SND_LOOP
	readFile("home.txt");
	NumCubes = noOfCubes;

	highlightedPositions = new int*[18];
	for(int i = 0 ; i < 18 ; i++)
	{
		highlightedPositions[i] = new int[5];
		highlightedPositions[i][0] = -1;
		highlightedPositions[i][1] = -1;
		highlightedPositions[i][2] = -1;
		highlightedPositions[i][3] = -1;
		highlightedPositions[i][4] = -1;
	}

	is_selected = 0 ;
	selected_cube = glm::vec3(-1,-1,-1);
	dest_cube = glm::vec3(-1,-1,-1);
	//NumCubes=9;

	initial_x_trans = -(float)(NumCubes*0.5);
	initial_z_trans = -(float)(NumCubes*0.5);

	/*workspace = new Cube***[NumCubes];
	for (int i = 0; i <  NumCubes; i++)
	{
		workspace[i] = new Cube**[NumCubes];
		for (int j = 0; j  < NumCubes ; j ++)
		{
			workspace[i][j] = new Cube*[NumCubes];
			for (int k = 0; k < NumCubes; k++)
			{
				workspace[i][j][k] = NULL;
			}
		}
	}*/



	initBasicCube();


	int current_cube = 0 ;
	// Initialize Cubes Ordering
	/*int floor,y_ws,x_ws;

	for(int i = 15 ; i < 20; i++)
	{
		for(int j = 15 ; j < 20; j++)
		{
			floor = NumCubes - 0 -1;
			x_ws=i;
			y_ws=j;
			workspace[floor][y_ws][x_ws] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, true);	
		}
	}

	for(int i = 16 ; i < 19; i++)
	{
		for(int j = 16 ; j < 19; j++)
		{
			floor = NumCubes - 1 -1;
			x_ws=i;
			y_ws=j;
			workspace[floor][y_ws][x_ws] = new Cube(glm::vec4(GREEN_COLOR, 1.0), 0, false);
		}
	}

	floor = NumCubes - 2 -1;
	x_ws=17;
	y_ws=17;
	workspace[floor][y_ws][x_ws] = new Cube(glm::vec4(YELLOW_COLOR, 1.0), 0, false);*/

	//workspace[0][0][0] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	/*workspace[8][3][3] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	workspace[8][4][3] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	workspace[8][5][3] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	
	workspace[8][3][4] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	workspace[8][4][4] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, true);	
	workspace[8][5][4] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	

	workspace[8][3][5] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	workspace[8][4][5] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	
	workspace[8][5][5] = new Cube(glm::vec4(RED_COLOR, 1.0), 0, false);	*/
	
	for (int i = 0; i < NumCubes; i++)
	{
		for (int j = 0; j < NumCubes; j++)
		{
			for (int l = 0; l < NumCubes; l++)
			{
				if(workspace[i][j][l] != NULL)
				{
					//yzx
					glm::mat4 translation_matrix = glm::translate(glm::mat4(),glm::vec3(l*translation_amount,(NumCubes - i - 1)*translation_amount,j*translation_amount));
					workspace[i][j][l]->setPointsIndex(current_cube*cube_points_num);
					current_cube++;
					createCube(translation_matrix, workspace[i][j][l]->getColor());
				}
			}
		}
	}



	initHeiglightCubes();
	initDesk();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Load shaders and use the resulting shader program
	program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );

	// Create and initialize a buffer object for vertices
	glGenBuffers( 1, &vertices_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Initialize the vertex position attribute in the vertex shader
	GLuint position_loc = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( position_loc );
	glVertexAttribPointer( position_loc, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	// Create and initialize a buffer object for vertices
	glGenBuffers( 1, &uv_buffer);
	glBindBuffer( GL_ARRAY_BUFFER, uv_buffer );
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	// Initialize the vertex position attribute in the vertex shader
	GLuint uv_loc = glGetAttribLocation( program, "UV" );
	glEnableVertexAttribArray( uv_loc );
	glVertexAttribPointer( uv_loc, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	// Create and initialize a buffer object for colors
	glGenBuffers( 1, &colors_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, colors_buffer );
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

	// Initialize the vertex color attribute in the vertex shader
	GLuint colors_loc = glGetAttribLocation( program, "vertexColor" );
	glEnableVertexAttribArray( colors_loc );
	glVertexAttribPointer( colors_loc, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );


	// Create and initialize a buffer object for normals
	glGenBuffers( 1, &normals_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, normals_buffer );
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	// Initialize the vertex position attribute in the vertex shader
	GLuint normal_loc = glGetAttribLocation( program, "vNormal" );
	glEnableVertexAttribArray( normal_loc );
	glVertexAttribPointer( normal_loc, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	// rotation data 
	uniisSelected = glGetUniformLocation(program,"isSelected"); 
	glUniform1f(uniisSelected,is_selected);

	// Desk data 
	uniisDesk = glGetUniformLocation(program,"isDesk"); 
	glUniform1f(uniisDesk,isDesk);

	// Load the texture
	GLuint Texture = loadDDS("desk.dds");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(program, "myTextureSampler");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	// Initialize HeighLight Data

	highlightColor = glm::vec4(0.5,0.0,0.5,1.0);
	uniHighlightColor = glGetUniformLocation(program,"heighlightColor"); 
	glUniform4fv(uniHighlightColor,1,&highlightColor[0]);

	HLM_1= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	uniHLM_1= glGetUniformLocation(program, "HLM_1");
	glUniformMatrix4fv(uniHLM_1, 1, GL_FALSE, glm::value_ptr(HLM_1));

	HLM_2= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	uniHLM_2= glGetUniformLocation(program, "HLM_2");
	glUniformMatrix4fv(uniHLM_2, 1, GL_FALSE, glm::value_ptr(HLM_2));

	HLM_3= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	uniHLM_3= glGetUniformLocation(program, "HLM_3");
	glUniformMatrix4fv(uniHLM_3, 1, GL_FALSE, glm::value_ptr(HLM_3));

	HLM_4= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	uniHLM_4= glGetUniformLocation(program, "HLM_4");
	glUniformMatrix4fv(uniHLM_4, 1, GL_FALSE, glm::value_ptr(HLM_4));

	HLM_5= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	uniHLM_5= glGetUniformLocation(program, "HLM_5");
	glUniformMatrix4fv(uniHLM_5, 1, GL_FALSE, glm::value_ptr(HLM_5));

	HLM_6= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	uniHLM_6= glGetUniformLocation(program, "HLM_6");
	glUniformMatrix4fv(uniHLM_6, 1, GL_FALSE, glm::value_ptr(HLM_6));

	// Set up diffuse
	GLfloat diffuse_0[4];
	diffuse_0[0] = 0.5;
	diffuse_0[1] = 0.5;
	diffuse_0[2] = 0.5;
	diffuse_0[3] = 1.0;

	GLuint diffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
	glUniform4fv(diffuseProduct,4,diffuse_0);

	// Set up ambient
	GLfloat ambient_0[4];
	ambient_0[0] = 0.0;
	ambient_0[1] = 0.0;
	ambient_0[2] = 0.0;
	ambient_0[3] = 1.0;

	GLuint ambientProduct = glGetUniformLocation(program, "AmbientProduct");
	glUniform4fv(ambientProduct ,4,ambient_0);

	// Set up specular
	GLfloat specular_0[4];
	specular_0[0] = 0.6;
	specular_0[1] = 0.6;
	specular_0[2] = 0.6;
	specular_0[3] = 1.0;

	GLuint specularProduct = glGetUniformLocation(program, "SpecularProduct");
	glUniform4fv(specularProduct,4,specular_0);

	// Set up light_position
	GLfloat light_position_0[4];
	light_position_0[0] = -10.0;
	light_position_0[1] = 10.0;
	light_position_0[2] = -10.0;
	light_position_0[3] = 0.0;

	GLuint lightPosition= glGetUniformLocation(program, "LightPosition");
	glUniform4fv(lightPosition,4,light_position_0);


	GLfloat shine = 100.0;
	GLuint shininessLoc = glGetUniformLocation(program, "Shininess");
	glUniform1f(shininessLoc,shine);

	// Set up model
	model = glm::mat4();
	uniModel = glGetUniformLocation(program, "Model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	// Set up Camera/View
	x_eye = 0.0001f;
	y_eye = 4.000f;
	z_eye = 16.000f;

	x_coi = 0.0f;
	y_coi = 0.0f;
	z_coi = 0.0f;

	cam_pos = glm::vec3(x_eye, y_eye, z_eye); 
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 aux_mat = glm::rotate(
		glm::mat4(),
		-30.0f,
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	// Calculate transformation
	cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
	cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

	aux_mat = glm::rotate(
		glm::mat4(),
		45.0f,
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	// Calculate transformation
	cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
	cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

	x_eye = cam_pos.x;
	y_eye = cam_pos.y;
	z_eye = cam_pos.z;

	view = glm::lookAt(
		cam_pos,
		glm::vec3(x_coi, y_coi, z_coi),
		cam_up
		);

	uniView = glGetUniformLocation(program, "Camera");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	// Setup Projection
	proj = glm::perspective(45.0f, WindowWidth / WindowHeight, 1.0f, 500.0f);
	uniProj = glGetUniformLocation(program, "Projection");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	animationMatrix = glm::mat4();
	uniAnimationMatrix = glGetUniformLocation(program, "AnimationMatrix");
	glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animationMatrix));

	translationMatrix = glm::translate(glm::mat4(),glm::vec3(0.0,0.0,0.0));
	uniTranslationMatrix = glGetUniformLocation(program, "TranslationMatrix");
	glUniformMatrix4fv(uniTranslationMatrix, 1, GL_FALSE, glm::value_ptr(translationMatrix));

	translationBackMatrix = glm::translate(glm::mat4(),glm::vec3(0.0,0.0,0.0));
	uniTranslationBackMatrix = glGetUniformLocation(program, "TranslationBackMatrix");
	glUniformMatrix4fv(uniTranslationBackMatrix, 1, GL_FALSE, glm::value_ptr(translationBackMatrix));


	glClearColor(0.0,0.0,0.0,1.0); // Background Color
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//TODO:: 
	sleepForAWhile(1);
}

/*
Initialize Basic Cube
*/

void initBasicCube()
{
	// Read our cube
	bool res_0 = loadOBJ("game_cube_corners.obj", basic_cube_vertices, basic_cube_uvs, basic_cube_normals);
	corners_points_num = basic_cube_vertices.size();

	bool res_1 = loadOBJ("game_cube_sides.obj", basic_cube_vertices, basic_cube_uvs, basic_cube_normals);
	sides_points_num = basic_cube_vertices.size() - corners_points_num ;

	bool res_2 = loadOBJ("game_cube_faces.obj", basic_cube_vertices, basic_cube_uvs, basic_cube_normals);
	faces_points_num = basic_cube_vertices.size() - corners_points_num - sides_points_num;

	glm::mat4 translation_matrix ;

	for(int i = 0 ; i < basic_cube_vertices.size() ; i++)
	{
		translation_matrix = glm::translate(glm::mat4(),glm::vec3(initial_x_trans ,0,initial_z_trans ));
		basic_cube_vertices.at(i) = glm::vec3(translation_matrix * glm::vec4(basic_cube_vertices.at(i),1.0));
	}
	cube_points_num = corners_points_num + sides_points_num + faces_points_num;
}

void initDesk()
{
	desk_index = vertices.size();
	// Read our cube
	std::vector<glm::vec3> temp_vertices;
	bool res_0 = loadOBJ("desk.obj", temp_vertices, uvs, normals);

	for(int i = 0 ; i < temp_vertices.size() ; i++)
	{
		vertices.push_back(temp_vertices.at(i));
		colors.push_back(glm::vec4(0.0,0.0,0.0,1.0));
	}
}

void initHeiglightCubes()
{
	for(int n = 0 ; n < 6 ; n++)
	{
		for(int i=0 ; i<basic_cube_vertices.size(); i++)
		{
			vertices.push_back(basic_cube_vertices.at(i));
		}

		for(int i=0 ; i<basic_cube_uvs.size(); i++)
		{
			uvs.push_back(basic_cube_uvs.at(i));
		}

		for(int i=0 ; i<basic_cube_normals.size();i++)
		{
			normals.push_back(basic_cube_normals.at(i));
		}

		// Colors
		for(int i = 0 ; i< corners_points_num ;i++)
		{
			colors.push_back(glm::vec4(BLACK_COLOR,0.42-.01*n));
		}

		for(int i = 0 ; i< sides_points_num ;i++)
		{
			colors.push_back(glm::vec4(1.0,1.0,1.0,0.42-.01*n));
		}

		for(int i = 0 ; i< faces_points_num ;i++)
		{
			colors.push_back(glm::vec4(1.0,1.0,1.0,0.42-.01*n));
		}
	}
}
/*
Create Cube
*/

void createCube(glm::mat4 translation_matrix , glm::vec4 color)
{
	for(int i=0 ; i<basic_cube_vertices.size(); i++)
	{
		// Scaling cube down
		glm::mat4 scaling_mat = glm::scale(glm::mat4(),glm::vec3(scaling_ratio,scaling_ratio,scaling_ratio));
		vertices.push_back(glm::vec3(translation_matrix * scaling_mat * glm::vec4(basic_cube_vertices.at(i), 1.0)));
	}

	for(int i=0 ; i<basic_cube_uvs.size(); i++)
	{
		uvs.push_back(glm::vec2(0.0,0.0));
	}

	for(int i=0 ; i<basic_cube_normals.size();i++)
	{
		normals.push_back(basic_cube_normals.at(i));
	}

	// Colors
	for(int i = 0 ; i< corners_points_num ;i++)
	{
		colors.push_back(glm::vec4(BLACK_COLOR,color.w));
	}

	for(int i = 0 ; i< sides_points_num ;i++)
	{
		colors.push_back(glm::vec4(SILVER_COLOR,color.w));
	}

	for(int i = 0 ; i< faces_points_num ;i++)
	{
		colors.push_back(color);
	}
}


void highlightPossiblePositions(int** possible_positions)
{

	HLM_1 = glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_1, 1, GL_FALSE, glm::value_ptr(HLM_1));

	HLM_2 = glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_2, 1, GL_FALSE, glm::value_ptr(HLM_2));

	HLM_3 = glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_3, 1, GL_FALSE, glm::value_ptr(HLM_3));

	HLM_4 = glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_4, 1, GL_FALSE, glm::value_ptr(HLM_4));

	HLM_5 = glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_5, 1, GL_FALSE, glm::value_ptr(HLM_5));

	HLM_6 = glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_6, 1, GL_FALSE, glm::value_ptr(HLM_6));


	int floor,y_ws,x_ws;
	floor = NumCubes-selected_cube.y-1;
	y_ws  = selected_cube.z;
	x_ws  = selected_cube.x;  

	highlightColor = workspace[floor][y_ws][x_ws]->getColor();
	highlightColor.a = 0.3; 
	glUniform4fv(uniHighlightColor,1,&highlightColor[0]);


	int x,y,z;
	int n = 1 ;
	for (int i = 0 ; i < 18 ; i++)
	{
		if(possible_positions[i][0] >= 0 && possible_positions[i][1] >= 0 && possible_positions[i][2] >= 0
			&& possible_positions[i][0] < NumCubes && possible_positions[i][1] < NumCubes && possible_positions[i][2] < NumCubes)
		{
			// get position to be highlighted
			y = NumCubes - possible_positions[i][0] -1;
			z = possible_positions[i][1];
			x = possible_positions[i][2];
			// highlight position
			putHighlightCube(n++,x,y,z);
		}
	}
}

void putHighlightCube(int n , int x ,int y, int z)
{
	switch(n)
	{
	case 1 :
		HLM_1 = glm::translate(glm::mat4(),glm::vec3(x,y,z));
		glUniformMatrix4fv(uniHLM_1, 1, GL_FALSE, glm::value_ptr(HLM_1));
		break;
	case 2 :
		HLM_2 = glm::translate(glm::mat4(),glm::vec3(x,y,z));
		glUniformMatrix4fv(uniHLM_2, 1, GL_FALSE, glm::value_ptr(HLM_2));
		break;
	case 3 :
		HLM_3 = glm::translate(glm::mat4(),glm::vec3(x,y,z));
		glUniformMatrix4fv(uniHLM_3, 1, GL_FALSE, glm::value_ptr(HLM_3));
		break;
	case 4 :
		HLM_4 = glm::translate(glm::mat4(),glm::vec3(x,y,z));
		glUniformMatrix4fv(uniHLM_4, 1, GL_FALSE, glm::value_ptr(HLM_4));
		break;
	case 5 :
		HLM_5 = glm::translate(glm::mat4(),glm::vec3(x,y,z));
		glUniformMatrix4fv(uniHLM_5, 1, GL_FALSE, glm::value_ptr(HLM_5));
		break;
	case 6 :
		HLM_6 = glm::translate(glm::mat4(),glm::vec3(x,y,z));
		glUniformMatrix4fv(uniHLM_6, 1, GL_FALSE, glm::value_ptr(HLM_6));
		break;
	}
}

//TODO
int** getPossibleRotations(GLint in_x, GLint in_y, GLint in_z)
{
	int **rotations = new int*[18];
	for (int i = 0; i < 18; i++)
	{
		rotations[i] = new int[5];
		rotations[i][0] = rotations[i][1] = rotations[i][2] = rotations[i][3] = rotations[i][4] = -1;
	}

	//x, y, z
	//x = x, y = z, floor = -y
	GLint x = in_x, y = in_z, floor = NumCubes - in_y - 1;
	if(workspace[floor][y][x]->isBase())
		return rotations;
	/*
	y
	@------> x
	|3 2 0
	|  # 1
	|6 5 4
	|
	\/ Floor
	*/
	
	// case 0: 
	if(floor > 0 && (x + 1 >= NumCubes || workspace[floor - 1][y][x+1] == NULL) && 
		workspace[floor - 1][y][x] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && 
		(x + 1 >= NumCubes || floor + 1 < NumCubes && workspace[floor + 1][y][x + 1] == NULL) && rotations[0][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x + 1)){
			rotations[0][0] = floor - 1;
			rotations[0][1] = y ;
			rotations[0][2] = x + 1;
			rotations[0][3] = 9;
			rotations[0][4] = 180;
	}
	// case 1:
	if(floor > 0 && x + 1 < NumCubes && workspace[floor - 1][y][x + 1] != NULL && 
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && floor + 1 < NumCubes && workspace[floor + 1][y][x + 1] == NULL) && 
		(x + 1 < NumCubes && floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && rotations[1][0] == -1 &&
		findGroundAll(floor, y, x, floor, y, x + 1)){
			rotations[1][0] = floor;
			rotations[1][1] = y;
			rotations[1][2] = x + 1;
			rotations[1][3] = 9;
			rotations[1][4] = 90;
	}
	// case 2:
	if(floor > 0 &&  workspace[floor - 1][y][x] == NULL && 
		x - 1 >= 0 && workspace[floor - 1][y][x - 1] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(x + 1 >= NumCubes || workspace[floor - 1][y][x + 1] == NULL) && rotations[2][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x)){
			rotations[2][0] = floor - 1;
			rotations[2][1] = y ;
			rotations[2][2] = x ;
			rotations[2][3] = 8;
			rotations[2][4] = 90;
	}
	// case 3:
	if(x - 1 >= 0 && floor > 0 &&  workspace[floor - 1][y][x] == NULL && 
		( workspace[floor - 1][y][x - 1] == NULL) &&
		( workspace[floor][y][x - 1] != NULL) &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && workspace[floor - 1][y][x + 1] == NULL) && rotations[3][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x - 1)){
			rotations[3][0] = floor - 1;
			rotations[3][1] = y ;
			rotations[3][2] = x  - 1;
			rotations[3][3] = 8;
			rotations[3][4] = 180;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// case 4:
	if(floor + 1 < NumCubes && (x + 1 >= NumCubes || workspace[floor + 1][y][x+1] == NULL) && 
		workspace[floor + 1][y][x] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && 
		(x + 1 >= NumCubes || floor - 1 < 0 || workspace[floor - 1][y][x + 1] == NULL) && rotations[4][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x + 1)){
			rotations[4][0] = floor + 1;
			rotations[4][1] = y ;
			rotations[4][2] = x + 1;
			rotations[4][3] = 10;
			rotations[4][4] = -180;
	}
	// case 1:
	if((floor + 1 == NumCubes || floor + 1 < NumCubes && x + 1 < NumCubes && workspace[floor + 1][y][x + 1] != NULL) && 
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && (floor - 1 < 0 || workspace[floor - 1][y][x + 1] == NULL)) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && rotations[1][0] == -1 &&
		findGroundAll(floor, y, x, floor , y, x + 1)){
			rotations[1][0] = floor;
			rotations[1][1] = y;
			rotations[1][2] = x + 1;
			rotations[1][3] = 10;
			rotations[1][4] = -90;
	}
	// case 5:
	if(floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		x - 1 >= 0 && workspace[floor + 1][y][x - 1] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(x + 1 >= NumCubes || workspace[floor + 1][y][x + 1] == NULL) && rotations[5][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x )){
			rotations[5][0] = floor + 1;
			rotations[5][1] = y ;
			rotations[5][2] = x ;
			rotations[5][3] = 11;
			rotations[5][4] = -90;
	}
	// case 6:
	if(x - 1 >= 0 && floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		( workspace[floor + 1][y][x - 1] == NULL) &&
		( workspace[floor][y][x - 1] != NULL) &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && workspace[floor + 1][y][x + 1] == NULL) && rotations[6][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x - 1)){
			rotations[6][0] = floor + 1;
			rotations[6][1] = y ;
			rotations[6][2] = x - 1;
			rotations[6][3] = 11;
			rotations[6][4] = -180;
	}
	//=================================================================================================================
	/*
	y
	@------> x
	|3 2 0
	|7 # 
	|6 5 4
	|
	\/ Floor
	*/
	// case 3:
	if(floor > 0 && (x - 1 < 0 || workspace[floor - 1][y][x-1] == NULL) && 
		workspace[floor - 1][y][x] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && 
		(x - 1 < 0 || workspace[floor + 1][y][x - 1] == NULL) && rotations[3][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x - 1)){
			rotations[3][0] = floor - 1;
			rotations[3][1] = y ;
			rotations[3][2] = x - 1;
			rotations[3][3] = 8;
			rotations[3][4] = -180;
	}
	// case 7:
	if(floor > 0 && x - 1 >= 0 && workspace[floor - 1][y][x - 1] != NULL && 
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && floor + 1 < NumCubes &&
		workspace[floor + 1][y][x - 1] == NULL) && 
		(floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && rotations[7][0] == -1 &&
		findGroundAll(floor, y, x, floor, y, x - 1)){
			rotations[7][0] = floor;
			rotations[7][1] = y;
			rotations[7][2] = x - 1;
			rotations[7][3] = 8;
			rotations[7][4] = -90;
	}
	// case 2:
	if(floor > 0 &&  workspace[floor - 1][y][x] == NULL && 
		x + 1 < NumCubes && workspace[floor - 1][y][x + 1] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(x - 1 < 0 || workspace[floor - 1][y][x - 1] == NULL) && rotations[2][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x )){
			rotations[2][0] = floor - 1;
			rotations[2][1] = y ;
			rotations[2][2] = x ;
			rotations[2][3] = 9;
			rotations[2][4] = -90;
	}
	// case 0:
	if(x + 1 < NumCubes && (floor > 0 &&  workspace[floor - 1][y][x] == NULL) && 
		( workspace[floor - 1][y][x + 1] == NULL) &&
		( workspace[floor][y][x + 1] != NULL) &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && workspace[floor - 1][y][x - 1] == NULL) && rotations[0][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x + 1)){
			rotations[0][0] = floor - 1;
			rotations[0][1] = y ;
			rotations[0][2] = x + 1;
			rotations[0][3] = 9;
			rotations[0][4] = -180;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// case 6:
	if(floor + 1 < NumCubes && (x - 1 < 0 || workspace[floor + 1][y][x - 1] == NULL) && 
		workspace[floor + 1][y][x] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && 
		(x - 1 < 0 || floor - 1 < 0 || workspace[floor - 1][y][x - 1] == NULL) && rotations[6][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x - 1)){
			rotations[6][0] = floor + 1;
			rotations[6][1] = y ;
			rotations[6][2] = x - 1;
			rotations[6][3] = 11;
			rotations[6][4] = 180;
	}
	// case 7:
	if((floor + 1 == NumCubes || floor + 1 < NumCubes && x - 1 >= 0 && workspace[floor + 1][y][x - 1] != NULL) && 
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && (floor - 1 < 0 || workspace[floor - 1][y][x - 1] == NULL)) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && rotations[7][0] == -1 &&
		findGroundAll(floor, y, x, floor , y, x - 1)){
			rotations[7][0] = floor;
			rotations[7][1] = y;
			rotations[7][2] = x - 1;
			rotations[7][3] = 11;
			rotations[7][4] = 90;
	}
	// case 5:
	if(floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		x + 1 < NumCubes && workspace[floor + 1][y][x + 1] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(x - 1 < 0 || workspace[floor + 1][y][x - 1] == NULL) && rotations[5][0] == -1 &&
		findGroundAll(floor, y, x, floor+ 1, y, x)){
			rotations[5][0] = floor + 1;
			rotations[5][1] = y ;
			rotations[5][2] = x ;
			rotations[5][3] = 10;
			rotations[5][4] = 90;
	}
	// case 4:
	if(x + 1 < NumCubes && floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		( workspace[floor + 1][y][x + 1] == NULL) &&
		( workspace[floor][y][x + 1] != NULL) &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && workspace[floor + 1][y][x - 1] == NULL) && rotations[4][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x + 1)){
			rotations[4][0] = floor + 1;
			rotations[4][1] = y ;
			rotations[4][2] = x + 1;
			rotations[4][3] = 10;
			rotations[4][4] = 180;
	}

	//?????????????????????????????????????????????????????????????????????????????????????????????????????????????????
	/*
	x
	@------> y
	|10 2  8
	|   #  9
	|12 5  11
	|
	\/ Floor
	*/
	// case 8: 
	if(floor > 0 && (y + 1 >= NumCubes || workspace[floor - 1][y+1][x] == NULL) && 
		workspace[floor - 1][y][x] != NULL &&
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL) && 
		(floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && 
		(y + 1 >= NumCubes || floor + 1 < NumCubes && workspace[floor + 1][y + 1][x] == NULL) && rotations[8][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y + 1, x)){
			rotations[8][0] = floor - 1;
			rotations[8][1] = y + 1 ;
			rotations[8][2] = x ;
			rotations[8][3] = 3;
			rotations[8][4] = -180;
	}
	// case 9:
	if(floor > 0 && y + 1 < NumCubes && workspace[floor - 1][y + 1][x] != NULL && 
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL && floor + 1 < NumCubes && workspace[floor + 1][y + 1][x] == NULL) && 
		(y + 1 < NumCubes && floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && rotations[9][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x)){
			rotations[9][0] = floor;
			rotations[9][1] = y + 1;
			rotations[9][2] = x;
			rotations[9][3] = 3;
			rotations[9][4] = -90;
	}
	// case 2:
	if(floor > 0 &&  workspace[floor - 1][y][x] == NULL && 
		y - 1 >= 0 && workspace[floor - 1][y - 1][x] != NULL &&
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL) && 
		(y + 1 >= NumCubes || workspace[floor - 1][y + 1][x] == NULL) && rotations[2][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x)){
			rotations[2][0] = floor - 1;
			rotations[2][1] = y ;
			rotations[2][2] = x ;
			rotations[2][3] = 0;
			rotations[2][4] = -90;
	}
	// case 10:
	if(y - 1 >= 0 && floor > 0 &&  workspace[floor - 1][y][x] == NULL && 
		( workspace[floor - 1][y - 1][x] == NULL) &&
		( workspace[floor][y - 1][x] != NULL) &&
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL && workspace[floor - 1][y + 1][x] == NULL) && rotations[10][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y - 1, x)){
			rotations[10][0] = floor - 1;
			rotations[10][1] = y - 1;
			rotations[10][2] = x;
			rotations[10][3] = 0;
			rotations[10][4] = -180;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// case 11:
	if(floor + 1 < NumCubes && (y + 1 >= NumCubes || workspace[floor + 1][y+1][x] == NULL) && 
		workspace[floor + 1][y][x] != NULL &&
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && 
		(y + 1 >= NumCubes || floor - 1 < 0 || workspace[floor - 1][y + 1][x] == NULL) && rotations[11][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y + 1, x)){
			rotations[11][0] = floor + 1;
			rotations[11][1] = y + 1;
			rotations[11][2] = x;
			rotations[11][3] = 2;
			rotations[11][4] = 180;
	}
	// case 9:
	if((floor + 1 == NumCubes || (floor + 1 < NumCubes && y + 1 < NumCubes && workspace[floor + 1][y + 1][x] != NULL)) && 
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL && (floor - 1 < 0 || workspace[floor - 1][y + 1][x] == NULL)) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && rotations[9][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x)){
			rotations[9][0] = floor;
			rotations[9][1] = y + 1;
			rotations[9][2] = x;
			rotations[9][3] = 2;
			rotations[9][4] = 90;
	}
	// case 5:
	if(floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		y - 1 >= 0 && workspace[floor + 1][y - 1][x] != NULL &&
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL) && 
		(y + 1 >= NumCubes || workspace[floor + 1][y + 1][x] == NULL) && rotations[5][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x)){
			rotations[5][0] = floor + 1;
			rotations[5][1] = y ;
			rotations[5][2] = x ;
			rotations[5][3] = 1;
			rotations[5][4] = 90;
	}
	// case 12:
	if(y - 1 >= 0 && floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		( workspace[floor + 1][y - 1][x] == NULL) &&
		( workspace[floor][y - 1][x] != NULL) &&
		(y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL && workspace[floor + 1][y + 1][x] == NULL) && rotations[12][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y - 1, x)){
			rotations[12][0] = floor + 1;
			rotations[12][1] = y - 1 ;
			rotations[12][2] = x;
			rotations[12][3] = 1;
			rotations[12][4] = 180;\
	}
	//=================================================================================================================
	/*
	x
	@------> y
	|10 2 8
	|13 # 
	|12 5 11
	|
	\/ Floor
	*/
	// case 10:
	if(floor > 0 && (y - 1 < 0 || workspace[floor - 1][y-1][x] == NULL) && 
		workspace[floor - 1][y][x] != NULL &&
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		(floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && 
		(y - 1 < 0 || workspace[floor + 1][y - 1][x] == NULL) && rotations[10][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y - 1, x)){
			rotations[10][0] = floor - 1;
			rotations[10][1] = y - 1;
			rotations[10][2] = x;
			rotations[10][3] = 0;
			rotations[10][4] = 180;
	}
	// case 13:
	if(floor > 0 && y - 1 >= 0 && workspace[floor - 1][y - 1][x]!= NULL && 
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL && floor + 1 < NumCubes &&
		workspace[floor + 1][y - 1][x] == NULL) && 
		(floor + 1 < NumCubes && workspace[floor + 1][y][x] == NULL) && rotations[13][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x)){
			rotations[13][0] = floor;
			rotations[13][1] = y-1;
			rotations[13][2] = x;
			rotations[13][3] = 0;
			rotations[13][4] = 90;
	}
	// case 2:
	if(floor > 0 &&  workspace[floor - 1][y][x] == NULL && 
		y + 1 < NumCubes && workspace[floor - 1][y + 1][x] != NULL &&
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		(y - 1 < 0 || workspace[floor - 1][y - 1][x] == NULL) && rotations[2][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y, x)){
			rotations[2][0] = floor - 1;
			rotations[2][1] = y ;
			rotations[2][2] = x ;
			rotations[2][3] = 3;
			rotations[2][4] = 90;
	}
	// case 8:
	if(y + 1 < NumCubes && (floor > 0 &&  workspace[floor - 1][y][x] == NULL) && 
		( workspace[floor - 1][y + 1][x] == NULL) &&
		( workspace[floor][y + 1][x] != NULL) &&
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL && workspace[floor - 1][y - 1][x] == NULL) && rotations[8][0] == -1 &&
		findGroundAll(floor, y, x, floor - 1, y + 1, x)){
			rotations[8][0] = floor - 1;
			rotations[8][1] = y + 1 ;
			rotations[8][2] = x;
			rotations[8][3] = 3;
			rotations[8][4] = 180;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// case 12:
	if(floor + 1 < NumCubes && (y - 1 < 0 || workspace[floor + 1][y - 1][x] == NULL) && 
		workspace[floor + 1][y][x] != NULL &&
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && 
		(y - 1 < 0 || floor - 1 < 0 || workspace[floor - 1][y - 1][x] == NULL) && rotations[12][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y - 1, x)){
			rotations[12][0] = floor + 1;
			rotations[12][1] = y-1 ;
			rotations[12][2] = x;
			rotations[12][3] = 1;
			rotations[12][4] = -180;
	}
	// case 13:
	if((floor + 1 == NumCubes || floor + 1 < NumCubes && y - 1 >= 0 && workspace[floor + 1][y - 1][x] != NULL) && 
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL && (floor - 1 < 0 || workspace[floor - 1][y - 1][x] == NULL)) && 
		(floor - 1 < 0 || workspace[floor - 1][y][x] == NULL) && rotations[13][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x)){
			rotations[13][0] = floor;
			rotations[13][1] = y-1;
			rotations[13][2] = x;
			rotations[13][3] = 1;
			rotations[13][4] = -90;
	}
	// case 5:
	if(floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		y + 1 < NumCubes && workspace[floor + 1][y + 1][x] != NULL &&
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		(y - 1 < 0 || workspace[floor + 1][y - 1][x] == NULL) && rotations[5][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y, x)){
			rotations[5][0] = floor + 1;
			rotations[5][1] = y ;
			rotations[5][2] = x ;
			rotations[5][3] = 2;
			rotations[5][4] = -90;
	}
	// case 11:
	if(y + 1 < NumCubes && floor + 1 < NumCubes &&  workspace[floor + 1][y][x] == NULL && 
		( workspace[floor + 1][y + 1][x] == NULL) &&
		( workspace[floor][y + 1][x] != NULL) &&
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL && workspace[floor + 1][y - 1][x] == NULL) && rotations[11][0] == -1 &&
		findGroundAll(floor, y, x, floor + 1, y + 1, x)){
			rotations[11][0] = floor + 1;
			rotations[11][1] = y + 1 ;
			rotations[11][2] = x;
			rotations[11][3] = 2;
			rotations[11][4] = -180;
	}
	//?????????????????????????????????????????????????????????????????????????????????????????????????????????????????
	/*
	floor
	.------> x
	|15 13 14
	|   #  1
	|17 9  16
	|
	\/ y
	*/

	// case 14: 
	if(y > 0 && (x + 1 >= NumCubes || workspace[floor][y - 1][x+1] == NULL) && 
		workspace[floor][y - 1][x] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(y + 1 < NumCubes && workspace[floor][y + 1][x] == NULL) && 
		(x + 1 >= NumCubes || y + 1 < NumCubes && workspace[floor][y + 1][x + 1] == NULL) && rotations[14][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x + 1)){
			rotations[14][0] = floor ;
			rotations[14][1] =  y - 1 ;
			rotations[14][2] = x + 1 ;
			rotations[14][3] = 5;
			rotations[14][4] = 180;
	}
	// case 1:
	if(y > 0 && x + 1 < NumCubes && workspace[floor][y - 1][x + 1] != NULL && 
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && y + 1 < NumCubes && workspace[floor][y + 1][x + 1] == NULL) && 
		(x + 1 < NumCubes && y + 1 < NumCubes && workspace[floor][y + 1][x] == NULL) && rotations[1][0] == -1 &&
		findGroundAll(floor, y, x, floor, y, x + 1)){
			rotations[1][0] = floor;
			rotations[1][1] = y;
			rotations[1][2] = x + 1;
			rotations[1][3] = 5;
			rotations[1][4] = 90;
	}
	// case 13:
	if(y > 0 &&  workspace[floor][y - 1][x] == NULL && 
		x - 1 >= 0 && workspace[floor][y - 1][x - 1] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(x + 1 >= NumCubes || workspace[floor][y - 1][x + 1] == NULL) && rotations[13][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x)){
			rotations[13][0] = floor;
			rotations[13][1] = y - 1;
			rotations[13][2] = x ;
			rotations[13][3] = 4;
			rotations[13][4] = 90;
	}
	// case 15:
	if(x - 1 >= 0 && (y - 1 < 0 ||  workspace[floor][y - 1][x] == NULL) && 
		(y - 1 < 0 || workspace[floor][y - 1][x - 1] == NULL) &&
		( workspace[floor][y][x - 1] != NULL) &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && (y - 1 < 0 || workspace[floor][y - 1][x + 1] == NULL)) && rotations[15][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x - 1)){
			rotations[15][0] = floor;
			rotations[15][1] = y - 1;
			rotations[15][2] = x  - 1;
			rotations[15][3] = 4;
			rotations[15][4] = 180;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// case 16:
	if(y + 1 < NumCubes && (x + 1 >= NumCubes || workspace[floor][y + 1][x+1] == NULL) && 
		workspace[floor][y + 1][x] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		(x + 1 >= NumCubes || y - 1 < 0 || workspace[floor][y - 1][x + 1] == NULL) && rotations[16][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x + 1)){
			rotations[16][0] = floor;
			rotations[16][1] = y + 1;
			rotations[16][2] = x + 1;
			rotations[16][3] = 6;
			rotations[16][4] = -180;
	}
	// case 1:
	if((y + 1 < NumCubes && x + 1 < NumCubes && workspace[floor][y + 1][x + 1] != NULL) && 
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && (y - 1 < 0 || workspace[floor][y - 1][x + 1] == NULL)) && 
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && rotations[1][0] == -1 &&
		findGroundAll(floor, y, x, floor, y, x + 1)){
			rotations[1][0] = floor;
			rotations[1][1] = y;
			rotations[1][2] = x + 1;
			rotations[1][3] = 6;
			rotations[1][4] = -90;
	}
	// case 9:
	if(y + 1 < NumCubes &&  workspace[floor][y + 1][x] == NULL && 
		x - 1 >= 0 && workspace[floor][y + 1][x - 1] != NULL &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL) && 
		(x + 1 >= NumCubes || workspace[floor][y + 1][x + 1] == NULL) && rotations[9][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x)){
			rotations[9][0] = floor ;
			rotations[9][1] = y + 1 ;
			rotations[9][2] = x ;
			rotations[9][3] = 7;
			rotations[9][4] = -90;
	}
	// case 17:
	if(x - 1 >= 0 && (y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL) && 
		(y + 1 >= NumCubes || workspace[floor][y + 1][x - 1] == NULL) &&
		( workspace[floor][y][x - 1] != NULL) &&
		(x + 1 >= NumCubes || workspace[floor][y][x + 1] == NULL && (y + 1 >= NumCubes || workspace[floor][y + 1][x + 1] == NULL)) && rotations[17][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x - 1)){
			rotations[17][0] = floor ;
			rotations[17][1] = y + 1;
			rotations[17][2] = x - 1;
			rotations[17][3] = 7;
			rotations[17][4] = -180;
	}
	//=================================================================================================================
	/*
	floor
	@------> x
	|15 13 14
	|7  # 
	|17 9  16
	|
	\/ y
	*/
	// case 15:
	if(y > 0 && (x - 1 < 0 || workspace[floor][y - 1][x-1] == NULL) && 
		workspace[floor][y - 1][x] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(y + 1 < NumCubes && workspace[floor][y + 1][x] == NULL) && 
		(x - 1 < 0 || workspace[floor][y + 1][x - 1] == NULL) && rotations[15][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x - 1)){
			rotations[15][0] = floor;
			rotations[15][1] = y - 1;
			rotations[15][2] = x - 1;
			rotations[15][3] = 4;
			rotations[15][4] = -180;
	}
	// case 7:
	if(y > 0 && x - 1 >= 0 && workspace[floor][y - 1][x - 1] != NULL && 
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && y + 1 < NumCubes &&
		workspace[floor][y + 1][x - 1] == NULL) && 
		(y + 1 < NumCubes && workspace[floor][y + 1][x] == NULL) && rotations[7][0] == -1 &&
		findGroundAll(floor, y, x, floor, y, x - 1)){
			rotations[7][0] = floor;
			rotations[7][1] = y;
			rotations[7][2] = x - 1;
			rotations[7][3] = 4;
			rotations[7][4] = -90;
	}
	// case 13:
	if(y > 0 &&  workspace[floor][y - 1][x] == NULL && 
		x + 1 < NumCubes && workspace[floor][y - 1][x + 1] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(x - 1 < 0 || workspace[floor][y - 1][x - 1] == NULL) && rotations[13][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x)){
			rotations[13][0] = floor;
			rotations[13][1] = y - 1;
			rotations[13][2] = x ;
			rotations[13][3] = 5;
			rotations[13][4] = -90;
	}
	// case 14:
	if(x + 1 < NumCubes && (y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		( y - 1 < 0 || workspace[floor][y - 1][x + 1] == NULL) &&
		( workspace[floor][y][x + 1] != NULL) &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && (y - 1 < 0 || workspace[floor][y - 1][x - 1] == NULL)) && rotations[14][0] == -1 &&
		findGroundAll(floor, y, x, floor, y - 1, x + 1)){
			rotations[14][0] = floor;
			rotations[14][1] = y - 1;
			rotations[14][2] = x + 1;
			rotations[14][3] = 5;
			rotations[14][4] = -180;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// case 17:
	if(y + 1 < NumCubes && (x - 1 < 0 || workspace[floor][y + 1][x - 1] == NULL) && 
		workspace[floor][y + 1][x] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && 
		(x - 1 < 0 || y - 1 < 0 || workspace[floor][y - 1][x - 1] == NULL) && rotations[17][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x - 1)){
			rotations[17][0] = floor;
			rotations[17][1] = y + 1;
			rotations[17][2] = x - 1;
			rotations[17][3] = 7;
			rotations[17][4] = 180;
	}
	// case 7:
	if((y + 1 < NumCubes && x - 1 >= 0 && workspace[floor][y + 1][x - 1] != NULL) && 
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && (y - 1 < 0 || workspace[floor][y - 1][x - 1] == NULL)) && 
		(y - 1 < 0 || workspace[floor][y - 1][x] == NULL) && rotations[7][0] == -1 &&
		findGroundAll(floor, y, x, floor, y, x - 1)){
			rotations[7][0] = floor;
			rotations[7][1] = y;
			rotations[7][2] = x - 1;
			rotations[7][3] = 7;
			rotations[7][4] = 90;
	}
	// case 9:
	if(y + 1 < NumCubes &&  workspace[floor][y + 1][x] == NULL && 
		x + 1 < NumCubes && workspace[floor][y + 1][x + 1] != NULL &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL) && 
		(x - 1 < 0 || workspace[floor][y + 1][x - 1] == NULL) && rotations[9][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x)){
			rotations[9][0] = floor;
			rotations[9][1] = y + 1;
			rotations[9][2] = x ;
			rotations[9][3] = 6;
			rotations[9][4] = 90;
	}
	// case 16:
	if(x + 1 < NumCubes && (y + 1 >= NumCubes || workspace[floor][y + 1][x] == NULL) && 
		(y + 1 >= NumCubes || workspace[floor][y + 1][x + 1] == NULL) &&
		( workspace[floor][y][x + 1] != NULL) &&
		(x - 1 < 0 || workspace[floor][y][x - 1] == NULL && (y + 1 >= NumCubes || workspace[floor][y + 1][x - 1] == NULL)) && rotations[16][0] == -1 &&
		findGroundAll(floor, y, x, floor, y + 1, x + 1)){
			rotations[16][0] = floor;
			rotations[16][1] = y + 1;
			rotations[16][2] = x + 1;
			rotations[16][3] = 6;
			rotations[16][4] = 180;
	}
	
	return rotations;
}
//shift the contents of the workspace in the reverse directin of x
void shiftInReverseX(){
	for (int i = 0; i < NumCubes; i++)
	{
		for (int j = 0; j < NumCubes; j++)
		{
			for (int k = 1; k < NumCubes; k++)
			{
				workspace[i][j][k - 1] = workspace[i][j][k];
				workspace[i][j][k] = NULL;
			}
		}
	}
}

//shift the contents of the workspace in the directin of x
void shiftInX(){
	for (int i = 0; i < NumCubes; i++)
	{
		for (int j = 0; j < NumCubes; j++)
		{
			for (int k = 1; k < NumCubes; k++)
			{
				workspace[i][j][k] = workspace[i][j][k - 1];
				workspace[i][j][k] = NULL;
			}
		}
	}
}

//shift the contents of the workspace in the reverse directin of y
void shiftInReverseY(){
	for (int i = 0; i < NumCubes; i++)
	{
		for (int j = 1; j < NumCubes; j++)
		{
			for (int k = 0; k < NumCubes; k++)
			{
				workspace[i][j - 1][k] = workspace[i][j][k];
				workspace[i][j][k] = NULL;
			}
		}
	}
}

//shift the contents of the workspace in the directin of x
void shiftInY(){
	for (int i = 0; i < NumCubes; i++)
	{
		for (int j = 1; j < NumCubes; j++)
		{
			for (int k = 0; k < NumCubes; k++)
			{
				workspace[i][j][k] = workspace[i][j - 1][k];
				workspace[i][j][k] = NULL;
			}
		}
	}
}

//the method to move a cube to the selected position
void playCube(GLint src_floor, GLint src_y, GLint src_x,GLint dst_floor, GLint dst_y, GLint dst_x){
	//Assign the new position
	Cube* temp = workspace[src_floor][src_y][src_x];
	workspace[src_floor][src_y][src_x] = NULL;
	if(dst_x >= NumCubes){
		shiftInReverseX();
		workspace[dst_floor][dst_y][dst_x - 1] = temp;
		changeIndex(src_floor,src_y,src_x,dst_floor,dst_y,dst_x);
	}else if(dst_x < 0){
		shiftInX();
		workspace[dst_floor][dst_y][dst_x + 1] = temp;
		changeIndex(src_floor,src_y,src_x,dst_floor,dst_y,dst_x);
	}else if(dst_y >= NumCubes){
		shiftInReverseY();
		workspace[dst_floor][dst_y - 1][dst_x] = temp;
		changeIndex(src_floor,src_y,src_x,dst_floor,dst_y,dst_x);
	}else if(dst_y < 0){
		shiftInY();
		workspace[dst_floor][dst_y + 1][dst_x] = temp;
		changeIndex(src_floor,src_y,src_x,dst_floor,dst_y,dst_x);
	}else{
		workspace[dst_floor][dst_y][dst_x] = temp;
		changeIndex(src_floor,src_y,src_x,dst_floor,dst_y,dst_x);
	}
	if(checkWin()){
		cout<<"YOU WON"<<endl;
		printf("\a");
	}
}

//DFS through all surroundings (Make sure all surroundings are grounded)
GLboolean findGroundAll(GLint src_floor, GLint src_y, GLint src_x,GLint dst_floor, GLint dst_y, GLint dst_x){
	//return true;
	Cube* temp = workspace[src_floor][src_y][src_x];
	workspace[src_floor][src_y][src_x] = NULL;
	if(dst_floor >= 0 && dst_floor < NumCubes && dst_y >= 0 && dst_y < NumCubes && dst_x >= 0 && dst_x < NumCubes){
		workspace[dst_floor][dst_y][dst_x] = temp;
		GLboolean result = findGround(src_floor + 1, src_y, src_x) && 
			findGround(src_floor - 1, src_y, src_x) &&
			findGround(src_floor, src_y + 1, src_x) &&
			findGround(src_floor, src_y - 1, src_x) &&
			findGround(src_floor, src_y, src_x + 1) &&
			findGround(src_floor, src_y, src_x - 1);
		workspace[src_floor][src_y][src_x] = temp;
		workspace[dst_floor][dst_y][dst_x] = NULL;

		return result;
	}
	//border
	GLboolean result = false;
	if(dst_floor + 1 < NumCubes && dst_floor + 1 >= 0 && dst_y >= 0 && dst_y < NumCubes && dst_x >= 0 && dst_x < NumCubes && workspace[dst_floor + 1][dst_y][dst_x] != NULL)
		result = findGround(dst_floor + 1, dst_y, dst_x);
	if(dst_floor - 1 >= 0 && dst_floor - 1 < NumCubes && dst_y >= 0 && dst_y < NumCubes && dst_x >= 0 && dst_x < NumCubes && workspace[dst_floor - 1][dst_y][dst_x] != NULL)
		result = findGround(dst_floor - 1, dst_y, dst_x);
	if(dst_y + 1 < NumCubes && dst_floor >= 0 && dst_floor < NumCubes && dst_y + 1 >= 0 && dst_x >= 0 && dst_x < NumCubes && workspace[dst_floor][dst_y + 1][dst_x] != NULL)
		result = findGround(dst_floor, dst_y + 1, dst_x);
	if(dst_y - 1 >= 0 && dst_floor >= 0 && dst_floor < NumCubes && dst_y - 1 < NumCubes && dst_x >= 0 && dst_x < NumCubes && workspace[dst_floor][dst_y - 1][dst_x] != NULL)
		result = findGround(dst_floor, dst_y - 1, dst_x);
	if(dst_x + 1 < NumCubes && dst_floor >= 0 && dst_floor < NumCubes && dst_y >= 0 && dst_y < NumCubes && dst_x + 1 >= 0 && workspace[dst_floor][dst_y][dst_x + 1] != NULL)
		result = findGround(dst_floor, dst_y, dst_x + 1);
	if(dst_x - 1 >= 0 && dst_floor >= 0 && dst_floor < NumCubes && dst_y >= 0 && dst_y < NumCubes && dst_x - 1 < NumCubes && workspace[dst_floor][dst_y][dst_x - 1] != NULL)
		result = findGround(dst_floor, dst_y, dst_x - 1);
	workspace[src_floor][src_y][src_x] = temp;
	return result;
}
//DFS through your workspace untill you reach the ground
GLboolean findGround(GLint floor, GLint y, GLint x){
	if(floor < 0 || floor >= NumCubes || y < 0 || y >= NumCubes || x < 0 || x >= NumCubes || workspace[floor][y][x] == NULL)
		return true;

	GLboolean result = isConnected(floor, y, x);
	clearVisited(floor, y, x);
	return result;
}

void clearVisited(GLint floor, GLint y, GLint x){
	workspace[floor][y][x]->setVisited(false);
	if(floor - 1 >= 0 && workspace[floor - 1][y][x] != NULL && workspace[floor - 1][y][x]->isVisited())
		clearVisited(floor - 1, y, x);
	if(floor + 1 < NumCubes && workspace[floor + 1][y][x] != NULL && workspace[floor + 1][y][x]->isVisited())
		clearVisited(floor + 1, y, x);
	if(x - 1 >= 0 && workspace[floor][y][x - 1] != NULL && workspace[floor][y][x - 1]->isVisited())
		clearVisited(floor, y, x - 1);
	if(x + 1 < NumCubes && workspace[floor][y][x + 1] != NULL && workspace[floor][y][x + 1]->isVisited())
		clearVisited(floor, y, x + 1);
	if(y - 1 >= 0 && workspace[floor][y - 1][x] != NULL && workspace[floor][y - 1][x]->isVisited())
		clearVisited(floor, y - 1, x);
	if(y + 1 < NumCubes && workspace[floor][y + 1][x] != NULL && workspace[floor][y + 1][x]->isVisited())
		clearVisited(floor, y + 1, x);

}

GLboolean isConnected(GLint floor, GLint y, GLint x){
	workspace[floor][y][x]->setVisited(true);
	if(workspace[floor][y][x]->isBase())
		return true;

	GLboolean result = false;
	if(floor - 1 >= 0 && workspace[floor - 1][y][x] != NULL && !workspace[floor - 1][y][x]->isVisited())
		result |= isConnected(floor - 1, y, x);
	if(floor + 1 < NumCubes && workspace[floor + 1][y][x] != NULL && !workspace[floor + 1][y][x]->isVisited())
		result |= isConnected(floor + 1, y, x);
	if(x - 1 >= 0 && workspace[floor][y][x - 1] != NULL && !workspace[floor][y][x - 1]->isVisited())
		result |= isConnected(floor, y, x - 1);
	if(x + 1 < NumCubes && workspace[floor][y][x + 1] != NULL && !workspace[floor][y][x + 1]->isVisited())
		result |= isConnected(floor, y, x + 1);
	if(y - 1 >= 0 && workspace[floor][y - 1][x] != NULL && !workspace[floor][y - 1][x]->isVisited())
		result |= isConnected(floor, y - 1, x);
	if(y + 1 < NumCubes && workspace[floor][y + 1][x] != NULL && !workspace[floor][y + 1][x]->isVisited())
		result |= isConnected(floor, y + 1, x);

	return result;
}

//read input from file and initialize the Models
void readFile(char* fileName) {
	int x, y, z, index, base, count = 0;
	float c0, c1, c2, c3;
	bool isbase;

	ifstream read(fileName);
	read >> noOfCubes;
	workspace = new Cube***[noOfCubes];
	targetModel = new Cube***[noOfCubes];
	for (int i = 0; i < noOfCubes; i++) {
		workspace[i] = new Cube**[noOfCubes];
		targetModel[i] = new Cube**[noOfCubes];
		for (int j = 0; j < noOfCubes; j++) {
			workspace[i][j] = new Cube*[noOfCubes];
			targetModel[i][j] = new Cube*[noOfCubes];
			for (int k = 0; k < noOfCubes; k++) {
				workspace[i][j][k] = NULL;
				targetModel[i][j][k] = NULL;
			}
		}
	}
	currentPoints = new int*[noOfCubes];
	for (int i = 0; i < noOfCubes; i++) {
		currentPoints[i] = new int[3];
		for (int j = 0; j < 3; j++)
			currentPoints[i][j] = -1;
	}

	originalPoints = new int*[noOfCubes];
	for (int i = 0; i < noOfCubes; i++) {
		originalPoints[i] = new int[3];
		for (int j = 0; j < 3; j++)
			originalPoints[i][j] = -1;
	}

	while (read >> x >> y >> z >> index >> base >> c0 >> c1 >> c2 >> c3) {
		isbase = (base == 0) ? false : true;
		workspace[x][y][z] = new Cube(glm::vec4(c0, c1, c2, c3), index, isbase);
		isbase = (workspace[x][y][z]->isBase()) ? true : false;
		targetModel[x][y][z] = new Cube(glm::vec4(c0, c1, c2, c3), index,
			isbase);
		currentPoints[count][0] = x, originalPoints[count][0] = x;
		currentPoints[count][1] = y, originalPoints[count][1] = y;
		currentPoints[count][2] = z, originalPoints[count][2] = z;
		count++;
	}
}

//randomize the workspace array
void genRand(GLint numOfRotations) {

	int currentToSwap, startIndex, i = 0, count = 0;
	GLint oldX, oldY, oldZ;
	srand(time(0));
	while (count < numOfRotations) {
		i = 0;
		currentToSwap = rand() % noOfCubes;
		oldX = currentPoints[currentToSwap][0], 
			oldY = currentPoints[currentToSwap][1], 
			oldZ = currentPoints[currentToSwap][2];
		int** x = getPossibleRotations(oldZ, noOfCubes - oldX - 1, oldY);

		startIndex = rand() % 18;
		while ((x[startIndex][0] == -1 || x[startIndex][1] == -1
			|| x[startIndex][2] == -1 || x[startIndex][0] >= noOfCubes
			|| x[startIndex][1] >= noOfCubes
			|| x[startIndex][2] >= noOfCubes) && i < 18) {
				startIndex = (startIndex + 1) % 18;
				i++;
		}
		
		if (i < 18) {
			workspace[x[startIndex][0]][x[startIndex][1]][x[startIndex][2]] =
				workspace[oldX][oldY][oldZ];
			workspace[oldX][oldY][oldZ] = NULL;
			currentPoints[currentToSwap][0] = x[startIndex][0];
			currentPoints[currentToSwap][1] = x[startIndex][1];
			currentPoints[currentToSwap][2] = x[startIndex][2];
			count++;

		}
	}
}



//check for win
bool checkWin() {
	for (int i = 0; i < noOfCubes; i++) {
		for(int j=0;j<noOfCubes;j++)
		{
			for(int k=0;k<noOfCubes;k++)
			{
				if( (workspace[i][j][k]==NULL && targetModel[i][j][k]!=NULL) || (workspace[i][j][k]!=NULL && targetModel[i][j][k]==NULL) || ((workspace[i][j][k]!=NULL && targetModel[i][j][k]!=NULL) && (workspace[i][j][k]->getColor().r-targetModel[i][j][k]->getColor().r>0.0001 || 
					workspace[i][j][k]->getColor().g-targetModel[i][j][k]->getColor().g>0.0001 ||
					workspace[i][j][k]->getColor().b-targetModel[i][j][k]->getColor().b>0.0001)))
			return false;
			}
		}
	}
	return true;
}

void changeIndex(GLint oldX, GLint oldY, GLint oldZ, GLint newX, GLint newY,
				 GLint newZ) {
	for (int i = 0; i < noOfCubes; i++) {
		if (currentPoints[i][0] == oldX && currentPoints[i][1] == oldY
			&& currentPoints[i][2] == oldZ) {
				currentPoints[i][0] = newX;
				currentPoints[i][1] = newY;
				currentPoints[i][2] = newZ;
				break;
		}
	}
}


glm::vec3 getProperPosition(GLfloat xf, GLfloat yf, GLfloat zf)
{
	int x = (int)xf;
	int y = (int)yf;
	int z = (int)zf;

	GLfloat margin = 0.1;
	if((xf - x) < margin || (x+1 - xf) < margin)
	{
		if(workspace[NumCubes-y-1][z][x] != NULL)
		{
			return glm::vec3(x,y,z);
		}
		else if(x-1 >= 0 && x-1 <NumCubes && workspace[NumCubes-y-1][z][x-1] != NULL)
		{
			return glm::vec3(x-1,y,z);
		}
		else if(x+1 >= 0 && x+1 <NumCubes && workspace[NumCubes-y-1][z][x+1] != NULL)
		{
			return glm::vec3(x+1,y,z);
		}
	}
	else if((yf - y) < margin || (y+1 - yf) < margin)
	{
		if(workspace[NumCubes-y-1][z][x] != NULL)
		{
			return glm::vec3(x,y,z);
		}
		else if(NumCubes-(y-1)-1 >= 0 && NumCubes-(y-1)-1< NumCubes && workspace[NumCubes-(y-1)-1][z][x] != NULL)
		{
			return glm::vec3(x,y-1,z);
		}
		else if(NumCubes-(y+1)-1 >= 0 && NumCubes-(y+1)-1<NumCubes && workspace[NumCubes-(y+1)-1][z][x] != NULL)
		{
			return glm::vec3(x,y+1,z);
		}
	}
	else if((zf - z) < margin || (z+1 - zf) < margin)
	{
		if(workspace[NumCubes-y-1][z][x] != NULL)
		{
			return glm::vec3(x,y,z);
		}
		else if(z-1 >= 0 && z-1<NumCubes && workspace[NumCubes-y-1][z-1][x] != NULL)
		{
			return glm::vec3(x,y,z-1);
		}
		else if(z+1 >= 0 && z+1 <NumCubes && workspace[NumCubes-y-1][z+1][x] != NULL)
		{
			return glm::vec3(x,y,z+1);
		}
	}
	else
	{
		return glm::vec3(-1.0,-1.0,-1.0);
	}
}

/*
Display Callback Function
*/
void display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the window

	isDesk = 0.0 ;	
	glUniform1f(uniisDesk,isDesk);

	if(selected_cube.x >=0)
	{
		int floor,y_ws,x_ws;
		floor = NumCubes-selected_cube.y-1;
		y_ws  = selected_cube.z;
		x_ws  = selected_cube.x;

		int selected_cube_index = workspace[floor][y_ws][x_ws]->getPointsIndex();

		is_selected = 0 ;	
		glUniform1f(uniisSelected,is_selected);

		glDrawArrays(GL_TRIANGLES, 0, selected_cube_index);

		is_selected = 1 ;	
		glUniform1f(uniisSelected,is_selected);

		glDrawArrays(GL_TRIANGLES, selected_cube_index, cube_points_num);

		is_selected = 0 ;	
		glUniform1f(uniisSelected,is_selected);

		glDrawArrays(GL_TRIANGLES, selected_cube_index+cube_points_num , desk_index-(selected_cube_index+cube_points_num));
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, desk_index);
	}

	isDesk = 1.0 ;	
	glUniform1f(uniisDesk,isDesk);

	glDrawArrays(GL_TRIANGLES, desk_index, vertices.size()-desk_index);
	glutSwapBuffers();
}

// -------------------------------------------- Input/Outpu Callback Functions -----------------------------
/**
Keyboard Callback Functions
*/

void
	keyboard( unsigned char key, int x, int y )
{ 
	switch ( key ) 
	{
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}


/*
Special Keyboard Key callback method
*/

void specialKey( int key, int x, int y )
{ 
	glm::mat4 aux_mat;
	switch(key)
	{
	case GLUT_KEY_UP:
		if(zooming_value > MIN_ZOOMING_VALUE)
			zooming_value -= ZOOMING_STEP;

		view = glm::lookAt(
			glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
			glm::vec3(x_coi, y_coi, z_coi),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		break;
	case GLUT_KEY_DOWN:
		if(zooming_value < MAX_ZOOMING_VALUE)
			zooming_value += ZOOMING_STEP;

		view = glm::lookAt(
			glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
			glm::vec3(x_coi, y_coi, z_coi),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		break;
	case GLUT_KEY_RIGHT:
		aux_mat = glm::rotate(
			glm::mat4(),
			3.0f,
			glm::vec3(0.0, 1.0, 0.0)
			);

		// Calculate transformation
		cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
		cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

		view = glm::lookAt(
			glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
			glm::vec3(x_coi, y_coi, z_coi),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		break;
	case GLUT_KEY_LEFT:

		aux_mat = glm::rotate(
			glm::mat4(),
			-3.0f,
			glm::vec3(0.0, 1.0, 0.0)
			);

		// Calculate transformation
		cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
		cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

		view = glm::lookAt(
			glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
			glm::vec3(x_coi, y_coi, z_coi),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		break;
	}
	glutPostRedisplay();
}

/**
Mouse Clicks Callback Functions
*/

void mouse( int button, int state, int x, int y )
{	
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(state== GLUT_UP)
		{
			// Camera Adjustments
			px = 0;
			py = 0;

			// Cube Selection Adjustments
			GLfloat winX, winY, winZ;
			glm::vec4 viewport = glm::vec4(0, 0, WindowWidth, WindowHeight);
			winX = (float)x;
			winY = (float)viewport[3] - (float)y;
			glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

			glm::vec3 H=  glm::unProject( glm::vec3(winX,winY,winZ), view * model, proj, viewport);


			GLfloat xf,yf,zf;
			xf = 0.995*((H[0] - initial_x_trans)+0.01) ;
			yf = 0.995*(H[1]+0.01);
			zf = 0.995*(H[2]- initial_z_trans+0.01);

			cout<< "x = : " << xf << " , y = : "<<yf<<" , z = : " <<zf<<endl;
			if(!animating)
			{
				if( !(xf<0 || yf<0 || zf< 0) && !(xf>=NumCubes ||  yf>=NumCubes ||  zf>=NumCubes))	
				{
					putHighlightedCubesInWorkspace();
					glm::vec3 positions = getProperPosition(xf,yf,zf);
					removeHighlightedCubesInWorkspace();

					if(positions.x >= 0)
					{
						int highlight_index = isHighlighted(positions.x,positions.y,positions.z);
						
						if(highlight_index!=-1)
						{
							//Highlight inside Workspace
							cout<< "Highlight Index inside Workspace :: x = : " << positions.x << " , y = : "<<positions.y<<" , z = : " <<positions.z<<" , angle = : " <<highlightedPositions[highlight_index][4]<<endl;
							dest_cube = glm::vec3(positions.x,positions.y,positions.z);
							// get rotation axis and angle
							int axis = highlightedPositions[highlight_index][3];
							int angle = highlightedPositions[highlight_index][4];
							//int axis = Z3;
							//int angle = -360;
							GLfloat x_shift = 0, y_shift = 0, z_shift = 0;
							glm::vec3 rot_vec ;

							switch(axis)
							{
							case X0 :
								x_shift = 0.0;
								y_shift = -(selected_cube.y + 1.0);

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5));
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z);

								rot_vec = X_AXIS;
								break;
							case X1 :
								x_shift = 0.0;
								y_shift = -selected_cube.y;

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5));
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z);
								rot_vec = X_AXIS;
								break;
							case X2 :
								x_shift = 0.0;
								y_shift = -selected_cube.y;

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5) + 1);
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z - 1);

								rot_vec = X_AXIS;
								break;
							case X3 :
								x_shift = 0.0;
								y_shift = -(selected_cube.y + 1.0);;

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5) + 1);
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z - 1);

								rot_vec = X_AXIS;
								break;
							case Y0:
								y_shift = 0.0;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5));
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x);

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5));
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z);

								rot_vec = Y_AXIS;
								break;
							case Y1:
								y_shift = 0.0;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5)+1);
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x -1);

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5)); 
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z);

								rot_vec = Y_AXIS;
								break;
							case Y2:
								y_shift = 0.0;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5)+1);
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x -1);

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5)+1); 
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z-1);

								rot_vec = Y_AXIS;
								break;
							case Y3:
								y_shift = 0.0;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5));
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x);

								if(selected_cube.z > (float)(NumCubes*0.5))
									z_shift = -(selected_cube.z- (float)(NumCubes*0.5)+1); 
								else
									z_shift = +((float)(NumCubes*0.5)-selected_cube.z-1);

								rot_vec = Y_AXIS;
								break;
							case Z0:
								z_shift = 0.0;
								y_shift = -(selected_cube.y + 1.0);

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5));
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x);

								rot_vec = Z_AXIS;
								break;
							case Z1 :
								z_shift = 0.0;
								y_shift = -(selected_cube.y + 1.0);;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5) + 1);
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x - 1);

								rot_vec = Z_AXIS;
								break;
							case Z2 :
								z_shift = 0.0;
								y_shift = -selected_cube.y;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5) + 1);
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x - 1);

								rot_vec = Z_AXIS;
								break;
							case Z3 :
								z_shift = 0.0;
								y_shift = -selected_cube.y;

								if(selected_cube.x > (float)(NumCubes*0.5))
									x_shift = -(selected_cube.x- (float)(NumCubes*0.5));
								else
									x_shift = +((float)(NumCubes*0.5)-selected_cube.x);

								rot_vec = Z_AXIS;
								break;


							}

							translationMatrix = glm::translate(glm::mat4(),glm::vec3(x_shift,y_shift,z_shift));
							glUniformMatrix4fv(uniTranslationMatrix, 1, GL_FALSE, glm::value_ptr(translationMatrix));

							translationBackMatrix = glm::translate(glm::mat4(),glm::vec3(-x_shift,-y_shift,-z_shift));
							glUniformMatrix4fv(uniTranslationBackMatrix, 1, GL_FALSE, glm::value_ptr(translationBackMatrix));

							rotateCube(rot_vec,angle);

						}
						else
						{
							cout<< "x = : " << positions.x << " , y = : "<<positions.y<<" , z = : " <<positions.z<<endl;
							selected_cube = positions;
							highlightedPositions = getPossibleRotations(positions.x,positions.y,positions.z);
							highlightPossiblePositions(highlightedPositions);
						}
					}
				}

				glutPostRedisplay();
			}
			break;
		}
	}
}

int isHighlighted(int x, int y, int z)
{
	int floor,y_ws,x_ws;
	for(int i = 0 ; i < 18 ; i++)
	{
		floor = NumCubes-y-1;
		y_ws = z ;
		x_ws = x ;
		if(	highlightedPositions[i][0] == floor 
			&&	highlightedPositions[i][1] == y_ws
			&&	highlightedPositions[i][2] == x_ws)
			return i;
	}
	return -1;
}

void putHighlightedCubesInWorkspace()
{
	for(int i = 0 ; i < 18 ; i++)
	{
		if(highlightedPositions[i][0] >= 0 && highlightedPositions[i][1] >= 0 && highlightedPositions[i][2] >= 0 
		&& highlightedPositions[i][0] < NumCubes && highlightedPositions[i][1] < NumCubes && highlightedPositions[i][2] < NumCubes)
		{
			workspace[highlightedPositions[i][0]][highlightedPositions[i][1]][highlightedPositions[i][2]] =  new Cube(glm::vec4(0.0,0.0,0.0,0.0), 0, false);
		}
	}
}

void removeHighlightedCubesInWorkspace()
{
	for(int i = 0 ; i < 18 ; i++)
	{
		if(highlightedPositions[i][0] >= 0 && highlightedPositions[i][1] >= 0 && highlightedPositions[i][2] >= 0 
		&& highlightedPositions[i][0] < NumCubes && highlightedPositions[i][1] < NumCubes && highlightedPositions[i][2] < NumCubes)
		{
			workspace[highlightedPositions[i][0]][highlightedPositions[i][1]][highlightedPositions[i][2]] = NULL;
		}
	}
}

void rotateCube(glm::vec3 rot_vec, GLfloat angle)
{
	if(angle==-180.0||angle==+180.0)
		PlaySound("Robot.wav", NULL, SND_ASYNC|SND_FILENAME);
	else
		PlaySound("Robot_small.wav", NULL, SND_ASYNC|SND_FILENAME);
	animating = true;
	// Hide Highlighting
	HLM_1= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_1, 1, GL_FALSE, glm::value_ptr(HLM_1));

	HLM_2= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_2, 1, GL_FALSE, glm::value_ptr(HLM_2));

	HLM_3= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_3, 1, GL_FALSE, glm::value_ptr(HLM_3));

	HLM_4= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_4, 1, GL_FALSE, glm::value_ptr(HLM_4));

	HLM_5= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_5, 1, GL_FALSE, glm::value_ptr(HLM_5));

	HLM_6= glm::translate(glm::mat4(),glm::vec3(-5000,-5000,-5000));
	glUniformMatrix4fv(uniHLM_6, 1, GL_FALSE, glm::value_ptr(HLM_6));

	rotation_vector = rot_vec;
	rotation_angle = angle;
	if(angle>0)
	{
		applyRotation(0);
	}
	else
	{
		applyReverseRotation(0);
	}
}

void adjustBuffers()
{
	int floor,y_ws,x_ws;
		floor = (int)NumCubes - selected_cube.y - 1;
		y_ws =  (int)selected_cube.z;
		x_ws =  (int)selected_cube.x;
		int cube_index = workspace[floor][y_ws][x_ws]-> getPointsIndex() ;

		for(int i = cube_index ; i<cube_index + cube_points_num ; i++)
		{
			glm::vec4 temp = translationBackMatrix * glm::rotate(glm::mat4(1.0),rotation_angle,rotation_vector)* translationMatrix * glm::vec4(vertices.at(i),1.0);
			vertices.at(i).x = temp.x;
			vertices.at(i).y = temp.y;
			vertices.at(i).z = temp.z;

			glm::vec4 temp_1 =  translationBackMatrix * glm::rotate(glm::mat4(1.0),rotation_angle,rotation_vector)* translationMatrix * glm::vec4(normals.at(i),1.0);
			normals.at(i).x = temp_1.x;
			normals.at(i).y = temp_1.y;
			normals.at(i).z = temp_1.z;
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		//glBufferSubData(GL_ARRAY_BUFFER, cube_index,cube_points_num*sizeof(glm::vec3), &vertices[cube_index]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


		glBindBuffer( GL_ARRAY_BUFFER, normals_buffer );
		//glBufferSubData(GL_ARRAY_BUFFER, cube_index,cube_points_num*sizeof(glm::vec3), &normals[cube_index]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		

		animationMatrix = glm::mat4();
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animationMatrix));

		translationMatrix = glm::mat4();
		glUniformMatrix4fv(uniTranslationMatrix, 1, GL_FALSE, glm::value_ptr(translationMatrix));

		translationBackMatrix = glm::mat4();
		glUniformMatrix4fv(uniTranslationBackMatrix, 1, GL_FALSE, glm::value_ptr(translationBackMatrix));

		int floor_src,y_ws_src,x_ws_src;
		floor_src = (int)NumCubes - selected_cube.y - 1;
		y_ws_src =  (int)selected_cube.z;
		x_ws_src =  (int)selected_cube.x;

		int floor_dest,y_ws_dest,x_ws_dest;
		floor_dest = (int)NumCubes - dest_cube.y - 1;
		y_ws_dest =  (int)dest_cube.z;
		x_ws_dest =  (int)dest_cube.x;

		playCube(floor_src,y_ws_src,x_ws_src,floor_dest,y_ws_dest,x_ws_dest);

		selected_cube = dest_cube;
		dest_cube = glm::vec3(-1,-1,-1);

		highlightedPositions = getPossibleRotations(selected_cube.x,selected_cube.y,selected_cube.z);
		highlightPossiblePositions(highlightedPositions);
		glutPostRedisplay();
}


void sleepForAWhile(int sleeping_period)
{
	animating = true;
	sleeping(sleeping_period);
}

void sleeping(int value)
{
	if(value > 0)
	{
		glutTimerFunc(500, sleeping, value-10);
	}
	else
	{
		genRand(0);
		animating = false;

		
		std::vector<glm::vec3> new_vertices;
		std::vector<glm::vec4> new_colors;
		std::vector<glm::vec2> new_uvs;
		std::vector<glm::vec3> new_normals; 
		
		vertices = new_vertices;
		colors = new_colors;
		uvs = new_uvs;
		normals = new_normals;

		int current_cube = 0;
		for (int i = 0; i < NumCubes; i++)
		{
			for (int j = 0; j < NumCubes; j++)
			{
				for (int l = 0; l < NumCubes; l++)
				{
					if(workspace[i][j][l] != NULL)
					{
						//yzx
						glm::mat4 translation_matrix = glm::translate(glm::mat4(),glm::vec3(l*translation_amount,(NumCubes - i - 1)*translation_amount,j*translation_amount));
						workspace[i][j][l]->setPointsIndex(current_cube*cube_points_num);
						current_cube++;
						createCube(translation_matrix, workspace[i][j][l]->getColor());
					}
				}
			}
		}
	
	initHeiglightCubes();
	initDesk();

	// Create and initialize a buffer object for vertices
	glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Create and initialize a buffer object for vertices
	glBindBuffer( GL_ARRAY_BUFFER, uv_buffer );
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	// Create and initialize a buffer object for colors
	glBindBuffer( GL_ARRAY_BUFFER, colors_buffer );
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

	// Create and initialize a buffer object for normals
	glBindBuffer( GL_ARRAY_BUFFER, normals_buffer );
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	display();
	}
}

void applyRotation(int value)
{
	if(value <= rotation_angle)
	{

		float angle = (float)value;
		animationMatrix = glm::rotate(glm::mat4(1.0),angle,rotation_vector);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animationMatrix));
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, applyRotation, value + ROTATION_STEP);
	}
	else
	{
		animating = false;
		adjustBuffers();
	}
}

void applyReverseRotation(int value)
{
	if(value >= rotation_angle)
	{
		float angle = (float)value;
		animationMatrix = glm::rotate(glm::mat4(1.0),angle,rotation_vector);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animationMatrix));
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, applyReverseRotation, value - ROTATION_STEP);
	}
	else
	{
		animating = false;
		adjustBuffers();
	}
}

/*
Mouse Motion Callback Method
*/

void mouseMotion(int x, int y )
{
	if(px != 0)
	{
		glm::mat4 aux_mat = glm::rotate(
			glm::mat4(),
			((px - x) / 600.0f) * 180.0f,
			glm::vec3(0.0, 1.0, 0.0)
			);

		// Calculate transformation
		cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
		cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

		view = glm::lookAt(
			glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
			glm::vec3(x_coi, y_coi, z_coi),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	}
	//if(py != 0)
	//{
	//	glm::vec3 rot_axis = glm::cross(cam_up, cam_pos);
	//	glm::mat4 aux_mat = glm::rotate(
	//		glm::mat4(),
	//		((py - y) / 600.0f) * 180.0f,
	//		rot_axis
	//		);

	//	// Calculate transformation
	//	cam_pos = glm::vec3(aux_mat*glm::vec4(cam_pos, 1.0));
	//	cam_up  = glm::vec3(aux_mat*glm::vec4(cam_up, 1.0));

	//	view = glm::lookAt(
	//		glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
	//		glm::vec3(x_coi, y_coi, z_coi),
	//		cam_up
	//		);
	//	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	//}
	//
	//py = y;
	px = x;
	glutPostRedisplay();
}

void 
	mouseWheel(int button , int dir , int x , int y){
		if (dir > 0)
		{
			if(zooming_value > MIN_ZOOMING_VALUE)
				zooming_value -= ZOOMING_STEP;

			view = glm::lookAt(
				glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
				glm::vec3(x_coi, y_coi, z_coi),
				cam_up
				);
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
			glutPostRedisplay();

		}
		else
		{
			if(zooming_value < MAX_ZOOMING_VALUE)
				zooming_value += ZOOMING_STEP;

			view = glm::lookAt(
				glm::vec3(cam_pos.x*zooming_value ,cam_pos.y*zooming_value ,cam_pos.z*zooming_value),
				glm::vec3(x_coi, y_coi, z_coi),
				cam_up
				);
			glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
			glutPostRedisplay();
		}
}
//------------------------------------------- Main Function -------------------------------------------------

int	main( int argc, char **argv )
{
	// Init OpenGl
	glutInit( &argc, argv );


	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH| GLUT_MULTISAMPLE);
	glutInitWindowSize( WindowWidth, WindowHeight);

	// Create Drawing Window
	glutCreateWindow( "Assemble as This : Game Play" );
	//glutFullScreen();
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Init Game
	init();

	// Set Display Main Function
	glutDisplayFunc( display );

	// Set Mouse Callback functions
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc( specialKey );
	glutKeyboardFunc( keyboard );
	//set timer callback function
	glutMainLoop();
	return 0;
}
