
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec2 uv = v_uv;
	uv.x +=sin(u_time+uv.x*100)*0.01;
	vec4 color = texture2D(u_texture,uv*10);
	uv = v_uv;
	uv.y+=sin(u_time+uv.y*20)*0.1;
	vec4 color2 = texture2D(u_texture,uv*10);
	gl_FragColor = u_color * color * color2;
}
