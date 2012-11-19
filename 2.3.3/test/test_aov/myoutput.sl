surface 
myoutput( float Ks=.5, Kd=.5, Ka=1, roughness=.1; color specularcolor=1;
	 output varying vector Nn = 0;
         output varying float Y = 0;
	 output varying float bright = 0;
	 output varying float    _zdepth = 0;
	 output varying color    _specular = 0;
	 )
{
    normal Nf;
    vector V;

    Nf = faceforward( normalize(N), I );
    V = -normalize(I);

    /* set the values of the output variables */
    Nn = normalize(Nf) ;

    /* pretend that we have plastic */
    Oi = Os;
    Ci = Os * ( Cs * (Ka*ambient() + Kd*diffuse(Nf)) +
                specularcolor * Ks * specular(Nf,V,roughness) );

    _specular = Os * specular(Nf,V,roughness);

    /* calculate a bright mask, giving positive values where
     * Tinny's surface is illuminated by various lights.
     */
    bright = 0.5 ;
    illuminance(P) {
	if (N . L > 0) {
	    bright += 0.25 ;
	}
    }

    /* we'd also just like to have the gray scale component, or
     * Y value of the output color.
     */
    Y = comp(ctransform("yiq", Ci), 0) ;

    _zdepth = (1.0 - depth(P)) ;
}