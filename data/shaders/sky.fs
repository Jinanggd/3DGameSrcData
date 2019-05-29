
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
	vec2 uv = v_uv;
	if(texture2D(u_texture,uv).w <0.3)
		discard;
	vec4 color = texture2D(u_texture,uv);
	//color = mix(vec4(0.2,0.2,0.2,1.0),color,visibility);
	gl_FragColor = u_color * color;
}
