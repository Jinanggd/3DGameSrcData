attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec4 a_color;


float density = 0.007;
float gradient = 1.5;


uniform vec3 u_camera_pos;

uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform sampler2D u_texture_mask;

//this will store the color for the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;
varying float visibility;


void main()
{	


	//calcule the normal in camera space (the NormalMatrix is like ViewMatrix but without traslation)
	v_normal = (u_model * vec4( a_normal, 0.0) ).xyz;
	
	//calcule the vertex in object space
	v_position = a_vertex;

	v_position.y = texture(u_texture_mask,a_uv).r*20; 

	v_world_position = (u_model * vec4( v_position, 1.0) ).xyz;

	float dist = length(u_viewprojection * vec4( v_world_position, 1.0 ));
	visibility = exp(-pow((dist*density),gradient));
	visibility = clamp(visibility,0,1);
	
	//store the color in the varying var to use it from the pixel shader
	v_color = a_color;

	//store the texture coordinates
	v_uv = a_uv;


	//calcule the position of the vertex using the matrices
	gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );


}