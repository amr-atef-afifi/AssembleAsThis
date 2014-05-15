in vec4 vPosition;
in vec2 UV;
out vec2 exUV;
in vec3 vNormal;
in vec4 vertexColor;
out vec4 vColor;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;

uniform mat4 Model; 
uniform mat4 Camera; 
uniform mat4 Projection;
uniform vec4 heighlightColor;
uniform float isSelected;

uniform mat4 HLM_1;
uniform mat4 HLM_2;
uniform mat4 HLM_3;
uniform mat4 HLM_4;
uniform mat4 HLM_5;
uniform mat4 HLM_6;

uniform mat4 AnimationMatrix;
uniform mat4 TranslationMatrix;
uniform mat4 TranslationBackMatrix;

uniform vec4 LightPosition;
uniform float Shininess;

void
main()
{
	mat4 ModelView = Camera * Model; 
	// Transform vertex position into eye coordinates 
	vec3 pos = (ModelView * vPosition).xyz; 
	
	// Compute viewer vector in eye coordinate 
	vec3 E = normalize( -pos );

	// Compute light vector in eye coordinate 
	vec3 L; 
	if (LightPosition.w == 0.0) 
		L = normalize( (Camera * LightPosition).xyz ); 
	else 
		L = normalize( (Camera * LightPosition).xyz - pos ); 
		
	// Compute halfway vector in eye coordinate 
	vec3 H = normalize( L + E ); // halfway vector 
	
	vec3 N ;
	if(isSelected > 0.5)
	{
		// Transform normal vector into eye coordinates 
		N = normalize( ModelView * TranslationBackMatrix * AnimationMatrix * TranslationMatrix * vec4(vNormal, 0.0) ).xyz;
	}
	else
	{
		// Transform normal vector into eye coordinates 
		N = normalize( ModelView * vec4(vNormal, 0.0) ).xyz;
	}

	// Compute terms in the illumination equation 
	vec4 ambient = AmbientProduct; 
	float Kd = max( dot(L, N), 0.0 ); 
	vec4 diffuse = Kd * DiffuseProduct; 
	float Ks = pow( max(dot(N, H), 0.0), Shininess ); 
	vec4 specular = Ks * SpecularProduct; 
	
	// discard the specular highlight if the light's behind the vertex 
	if ( dot(L, N) < 0.0 ) 
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	if(vertexColor.a == 0.42)
	{
		gl_Position = Projection *  Camera * Model * HLM_1 * vPosition; 
		vColor = ambient + diffuse + specular + heighlightColor; 
		vColor.a = heighlightColor.a;
	}
	else if(vertexColor.a == 0.41)
	{
		gl_Position = Projection *  Camera * Model * HLM_2 * vPosition; 
		vColor = ambient + diffuse + specular + heighlightColor; 
		vColor.a = heighlightColor.a;
	}
	else if(vertexColor.a == 0.40)
	{
		gl_Position = Projection *  Camera * Model * HLM_3 * vPosition; 
		vColor = ambient + diffuse + specular + heighlightColor; 
		vColor.a = heighlightColor.a;
	}
	else if(vertexColor.a == 0.39)
	{
		gl_Position = Projection *  Camera * Model * HLM_4 * vPosition;
		vColor = ambient + diffuse + specular + heighlightColor; 
		vColor.a = heighlightColor.a; 
	}
	else if(vertexColor.a == 0.38)
	{
		gl_Position = Projection *  Camera * Model * HLM_5 * vPosition;
		vColor = ambient + diffuse + specular + heighlightColor; 
		vColor.a = heighlightColor.a; 
	}
	else if(vertexColor.a == 0.37)
	{
		gl_Position = Projection *  Camera * Model * HLM_6 * vPosition;
		vColor = ambient + diffuse + specular + heighlightColor; 
		vColor.a = heighlightColor.a; 
	}
	else
	{
		if(isSelected > 0.5)
		{
			gl_Position = Projection * ModelView * TranslationBackMatrix * AnimationMatrix * TranslationMatrix *vPosition; 
			vColor = ambient + diffuse + specular + vertexColor+ vec4(0.35,0.35,0.35,0.35); 
		}
		else
		{
			gl_Position = Projection * ModelView * vPosition; 
			vColor = ambient + diffuse + specular + vertexColor; 	
		}
		vColor.a = vertexColor.a;
	}

	exUV = UV;
}
