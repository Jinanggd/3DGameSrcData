
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture_grass;
uniform sampler2D u_texture_rock;
uniform sampler2D u_texture_mask;
uniform float u_time;

void main()
{


	vec2 uv = v_uv;

	vec4 grass = texture2D(u_texture_grass,uv*10);
	vec4 rock = texture2D(u_texture_rock,uv*10);
	vec4 mask = texture2D(u_texture_mask,uv); 

	vec4 color = grass * mask.x + rock*(1.0-mask.x);

	gl_FragColor = color;
	
	
}
