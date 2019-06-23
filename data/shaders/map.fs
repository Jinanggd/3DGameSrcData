
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;
varying float visibility;

uniform vec4 u_color;
uniform sampler2D u_texture_grass;
uniform sampler2D u_texture_rock;
uniform sampler2D u_texture_mask;
uniform sampler2D u_texture_water;
uniform sampler2D u_texture;
uniform float u_time;


void main()
{
	

	vec2 uv = v_uv;

	vec4 grass = texture2D(u_texture_grass,uv*2);
	vec4 rock = texture2D(u_texture_rock,uv*100);
	vec4 mask = texture2D(u_texture_mask,uv); 
	vec4 water = texture2D(u_texture_water,uv*30); 
	vec4 color = u_color;

	if(mask.x>0.3)
		color = rock * mask.x + grass*(1.0-mask.x);

	else color = mix(rock,water,0.8);

	color.w=0.8;
	
	gl_FragColor = color;
	
	

	
}
