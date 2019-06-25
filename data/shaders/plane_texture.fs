
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
uniform float u_time;



void main()
{
	

	vec2 uv = v_uv;

	vec4 grass = texture2D(u_texture_grass,uv*2);
	vec4 rock = texture2D(u_texture_rock,uv*100);
	vec4 mask = texture2D(u_texture_mask,uv); 
	vec4 water = texture2D(u_texture_water,uv*30); 
	vec4 color = vec4(1);

	if(mask.x>0.51)
		color = rock * mask.x + grass*(1.0-mask.x);

	else color = mix(rock,water,0.8);

	//color *= vec4(abs(cos(u_time/100)),abs(cos(u_time/100)),abs(cos(u_time/100)),1.0);
	
	color = mix(vec4(0.2,0.2,0.2,1.0),color,visibility);
	gl_FragColor = color;
	
	
}
