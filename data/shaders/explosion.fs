
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;
varying float visibility;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{

	float R = 20.0;
	vec2 uv = v_uv;
	uv.x +=R*sin(u_time+uv.x*10)*0.01;
	vec4 color = texture2D(u_texture,uv);
	uv = v_uv;
	uv.y+=R*cos(u_time+uv.y*10)*0.01;
	vec4 color2 = texture2D(u_texture,uv);
	vec4 finalcolor = u_color*color*color2;
	if((finalcolor.w)<0.2)
		discard; 
		
	gl_FragColor = finalcolor;
	
	
}
